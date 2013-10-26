#include "NServer.h"

#include "RobotProtocol.h"
#include "NetworkProtocol.h"
#include "StrategyManager.h"
#include "NMicropather.h"

#include <QTcpSocket>
#include <QNetworkInterface>
#include <QCoreApplication>
#include <QProcess>
#include <QDesktopServices>

const int SIMULATION_REFRESH = 50; //ms
const double SIMULATION_SPEED = 0.4;

const QString NEOBOT_SERVER_DATA_DIR_NAME = "NeobotServerData";
const QString AX12_MOVEMENT_FILENAME = "Ax12Movements.nax";

const QString USE_AX12_CONTROLLER_KEY = "UseAX12Controller";
const QString AX12_UPDATE_INTERVAL_KEY = "AX12Updateinterval";

NServer::NServer(Tools::AbstractLogger *logger, QObject *parent) :
	QObject(parent), Tools::LoggerInterface(logger), _settings("Neobot", "NServer"), _tcpServer(0), _nextConnectionIndex(0), _robotInterface(0),
    _simulator(0), _strategyManager(0), _currentStrategy(0), _currentStrategyId(-1), _robotConnected(false), _ax12Manager(0), _ax12MovementRunner(0)
{
	_disconnectionMapper = new QSignalMapper(this);
	connect(_disconnectionMapper, SIGNAL(mapped(int)), this, SLOT(removeConnection(int)));

	_dataDirectory = Tools::getDataDirectory();
	_dataDirectory.mkdir(NEOBOT_SERVER_DATA_DIR_NAME);
	_dataDirectory.cd(NEOBOT_SERVER_DATA_DIR_NAME);

    initServerSettings();
}

NServer::~NServer()
{
	cleanRobotConnection();
}

void NServer::initServerSettings()
{
    if (!_settings.contains(USE_AX12_CONTROLLER_KEY))
        _settings.setValue(USE_AX12_CONTROLLER_KEY, true);

    if (!_settings.contains(AX12_UPDATE_INTERVAL_KEY))
        _settings.setValue(AX12_UPDATE_INTERVAL_KEY, 200);
}

bool NServer::start()
{
	delete _tcpServer;
	_tcpServer = new QTcpServer(this);
	connect(_tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));

	if (!_tcpServer->listen(QHostAddress::Any, 15042))
	{
		logger() << "Starting failed...." << Tools::endl;
		return false;
	}

	// Search for the ip address of the server, use the first non-localhost IPv4 address
	QString ipAddress;
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
	for (int i = 0; i < ipAddressesList.size(); ++i)
	{
		if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
			ipAddressesList.at(i).toIPv4Address())
		{
			ipAddress = ipAddressesList.at(i).toString();
			break;
		}
	}

	// If we did not find one, use IPv4 localhost
	if (ipAddress.isEmpty())
		ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

	logger() << QString("The server is running on\n\nIP: %1\nport: %2\n\n").arg(ipAddress).arg(_tcpServer->serverPort());

	//load ax12 movements
	QString ax12MovementFile = _dataDirectory.absoluteFilePath(AX12_MOVEMENT_FILENAME);
	if (!QFile::exists(ax12MovementFile))
	{
		QFile f(ax12MovementFile);
		f.open(QIODevice::WriteOnly);
	}
	_ax12Movements.readFromFile(ax12MovementFile);

	QTimer::singleShot(20 * 1000, this, SLOT(autoStart()));

    return true;
}

void NServer::startPing()
{
	//auto start
	int autoStrat = -1;
	bool enabled = false;
	bool simulation = false;
	bool mirrored = false;
	QString robotPort, ax12Port;
	askAutoStrategyInfo(enabled, autoStrat, robotPort, ax12Port, simulation, mirrored);
	if (enabled)
	{
		QByteArray message;
		if (connectToRobot(0, simulation, robotPort, ax12Port, message))
		{
			_robotInterface->ping();
			QTimer::singleShot(2*1000, this, SLOT(autoStart()));
		}
		else
		{
			_autoStartMessage = "Auto Start Failed: The connection to the robot has failed";
		}
	}

	if (!_autoStartMessage.isEmpty())
		logger() << _autoStartMessage;
}


void NServer::autoStart()
{
	int autoStrat = -1;
	bool enabled = false;
	bool simulation = false;
	bool mirrored = false;
	QString robotPort, ax12Port;
	askAutoStrategyInfo(enabled, autoStrat, robotPort, ax12Port, simulation, mirrored);
	if (enabled)
	{
		QByteArray message;
		if (connectToRobot(0, simulation, robotPort, ax12Port, message))
		{
			if (startStrategy(autoStrat, mirrored))
				_autoStartMessage = "Auto Strategy Started";
			else
				_autoStartMessage = "Auto Strategy Failed: Strategy not found";
		}
		else
		{
			_autoStartMessage = "Auto Strategy Failed: The connection to the robot has failed";
		}
	}

	if (!_autoStartMessage.isEmpty())
		logger() << _autoStartMessage;
}

void NServer::sendGlobalAnnoucement(const QByteArray &message)
{
	for(QMap<unsigned int, NetworkCommInterface*>::const_iterator it = _connections.constBegin(); it != _connections.constEnd(); ++it)
	{
		NetworkCommInterface* i = *it;
		i->sendAnnouncement(message);
	}
}

void NServer::removeConnection(int index)
{
    NetworkCommInterface* networkInterface = _connections.value(index, 0);
    if (networkInterface)
	{
		_connections.remove(index);
        if (_ax12Requests.contains(networkInterface))
            delete _ax12Requests.take(networkInterface);

        Comm::AbstractProtocol* protocol = networkInterface->getProtocol();
		QIODevice* connection = protocol->getIODevice();

		protocol->disconnect();
		protocol->close();

		logger() << "Connection(" << index << ") closed!" << Tools::endl;
			
        delete networkInterface;
		delete protocol;
		connection->deleteLater();
	}
}

void NServer::newConnection()
{
	QTcpSocket* connection = _tcpServer->nextPendingConnection();

	Comm::NetworkProtocol* protocol = new Comm::NetworkProtocol(connection, &logger());
	protocol->setLogEnabled(false);

    NetworkCommInterface* networkInterface = new NetworkCommInterface(protocol, _robotInterface, &logger());
    networkInterface->setListener(this);
	
	while (_connections.contains(_nextConnectionIndex))
		++_nextConnectionIndex;
		
    _connections[_nextConnectionIndex] = networkInterface;
	_disconnectionMapper->setMapping(connection, _nextConnectionIndex);
	connect(connection, SIGNAL(disconnected()), _disconnectionMapper, SLOT(map()));

	logger() << "New connection(" << _nextConnectionIndex << ") with " << connection->peerAddress().toString() << " established..." << Tools::endl;
	
	++_nextConnectionIndex;

    networkInterface->sendAnnouncement("Welcome to Neobot server version 3.42 Please be gentle with the robot...");
	if (!_autoStartMessage.isEmpty())
		networkInterface->sendAnnouncement(_autoStartMessage);
}

void NServer::cleanRobotConnection()
{
	if (_robotInterface)
	{
		delete _currentStrategy;
		_currentStrategy = 0;

		delete _strategyManager;
		_strategyManager = 0;

		delete _pather;
		_pather = 0;

		QIODevice* device = _robotInterface->getProtocol()->getIODevice();
		Comm::AbstractProtocol* p = _robotInterface->getProtocol();
		delete _robotInterface;
		delete p;
		if (!_simulator)
			delete device;
		_robotInterface = 0;
	}

	delete _simulator;
	_simulator = 0;

    foreach(ServerAX12RequestManager* request, _ax12Requests)
        request->setAX12CommManager(0);

	delete _ax12Manager;
	_ax12Manager = 0;

	delete _ax12MovementRunner;
	_ax12MovementRunner = 0;
}

bool NServer::pingReceived()
{
	return true;
}

void NServer::noticeOfReceipt(quint8 instruction, bool result)
{
	Q_UNUSED(instruction);
	Q_UNUSED(result);
}

bool NServer::connectToRobot(NetworkCommInterface* networkInterface, bool simulation, const QString &robotPort, const QString &ax12Port, QByteArray &message)
{
	bool restarted = false;
	if (!_robotConnected || (bool)_simulator != simulation)
	{
		restarted = true;
		cleanRobotConnection();

		Comm::AbstractProtocol* p = 0;
		if (!simulation)
		{
			QextSerialPort* port = new QextSerialPort(robotPort,  QextSerialPort::EventDriven);
			port->setBaudRate(BAUD115200);
			port->setFlowControl(FLOW_OFF);
			port->setParity(PAR_NONE);
			port->setDataBits(DATA_8);
			port->setStopBits(STOP_1);

			connect(port, SIGNAL(dsrChanged(bool)), this, SLOT(serialPortStatusChanged(bool)));

			Comm::RobotProtocol* protocol = new Comm::RobotProtocol(port);
			p = protocol;

			_simulator = 0;

			if (!protocol->open())
			{
				message = "The robot is not available right now. Try again later or use the simulation mode...";
				return false;
			}
		}
		else
		{
			SimProtocol* simulationProtocol = new SimProtocol;
			p = simulationProtocol;

			_simulator = new Simulator(simulationProtocol, SIMULATION_REFRESH, SIMULATION_SPEED);
		}

        Comm::AX12CommManager::ControllerMode ax12ControllerMode = Comm::AX12CommManager::NO_CONTROLLER;
        if (_settings.value(USE_AX12_CONTROLLER_KEY).toBool())
            ax12ControllerMode = Comm::AX12CommManager::USB2AX_CONTROLLER;

        _ax12Manager = new Comm::AX12CommManager(ax12Port, BAUD115200, ax12ControllerMode, &logger());
		if (!_ax12Manager->open())
		{
			message = "The AX-12 are not available...";
		}
		else
        {
			_ax12Manager->setRequestTimeout(200);
			_ax12Manager->setReadingLoopMode(Comm::AX12CommManager::AUTO_MODE);
			_ax12Manager->setTimerReadingLoopInterval(200);

			_ax12MovementRunner = new Comm::Ax12MovementRunner(_ax12Manager, &_ax12Movements);
            connect(_ax12MovementRunner, SIGNAL(movementFinished(bool,QString,QString)), this, SLOT(ax12MovementFinished()));
		}

        foreach(ServerAX12RequestManager* request, _ax12Requests)
            request->setAX12CommManager(_ax12Manager);

		_robotInterface = new Comm::RobotCommInterface(p, _ax12Manager, 0);
		_robotInterface->disableNoticeOfReceiptChecking();

		_pather = new NMicropather(0, NMicropather::Euclidean, 1000.0);
		_strategyManager = new StrategyManager(_robotInterface, _pather, 0);
		connect(_strategyManager, SIGNAL(strategyFinished()), this, SLOT(strategyFinished()));
		_strategyManager->setAx12MovementManager(&_ax12Movements);
	}

	updateRobotConnection();
	_robotConnected = true;
	_robotConnections << networkInterface;

	if (restarted && _simulator)
	{
		_simulator->start();
		_simulator->setPosition(0, 0, 0.0);
	}


	return true;
}

bool NServer::disconnectToRobot(NetworkCommInterface* networkInterface)
{
	_robotConnections.removeAll(networkInterface);
	if (_robotConnections.isEmpty())
	{
		cleanRobotConnection();
		_robotConnected = false;
		sendGlobalAnnoucement("Robot Connection Closed!");
	}

	return true;
}

bool NServer::updateServer(const QByteArray &data)
{
	return false; //not implemented yet

	QString newAppPath = "NServer-new";
	QFile f(newAppPath);
	if (!f.open(QIODevice::WriteOnly))
		return false;

	f.write(data);

	QStringList arguments;
	arguments << newAppPath << qApp->applicationFilePath();
	QProcess::startDetached("./update_server.sh", arguments);

	qApp->quit();

	return true;
}

QStringList NServer::askStrategyFiles(int strategyNum)
{
	return _strategiesEnumerator.askStrategyFiles((StrategyEnumerator::Strategy)strategyNum);
}

QByteArray NServer::askStrategyFileData(int strategyNum, const QString &filename)
{
	return _strategiesEnumerator.askStrategyFileData((StrategyEnumerator::Strategy)strategyNum, filename);
}

void NServer::setStrategyFileData(int strategyNum, const QString &filename, const QByteArray &data)
{
	_strategiesEnumerator.setStrategyFileData((StrategyEnumerator::Strategy)strategyNum, filename, data);
}

void NServer::resetStrategyFile(int strategyNum, const QString &filename)
{
	_strategiesEnumerator.resetStrategyFile((StrategyEnumerator::Strategy)strategyNum, filename);
}

void NServer::askAx12Positions(NetworkCommInterface *networkInterface, const QList<quint8> &ids, bool recursive)
{
    if (!_ax12Manager)
		return;

    if (!_ax12Requests.contains(networkInterface))
    {
        int interval = _settings.value(AX12_UPDATE_INTERVAL_KEY).toInt();
        _ax12Requests[networkInterface] = new ServerAX12RequestManager(_ax12Manager, networkInterface, interval);
    }

    _ax12Requests[networkInterface]->setRequest(ids, recursive);
}

void NServer::moveAx12(float maxSpeed, QList<Comm::Ax12Info> &ax12s)
{
	if (!_ax12Manager || _ax12MovementRunner->isRunnning())
		return;

    QList<quint8> ids;
	foreach(const Comm::Ax12Info& info, ax12s)
	{
        ids << info.id;
		_ax12Manager->setGoal(info.id, info.angle, info.speed, info.torque, false);
	}

	if (maxSpeed > 0)
        _ax12Manager->calculateSmoothServosSpeed(ids, maxSpeed);
		
    _ax12Manager->synchronize(ids);
}

void NServer::lockAx12(const QMap<quint8, bool> &servoLockInfo)
{
	if (!_ax12Manager || _ax12MovementRunner->isRunnning())
		return;

    QList<quint8> ids;
	for(QMap<quint8, bool>::const_iterator it = servoLockInfo.constBegin(); it != servoLockInfo.constEnd(); ++it)
	{
		quint8 id = it.key();
		bool locked = it.value();

        ids << id;

		if (locked)
			_ax12Manager->lockServo(id, false);
		else
			_ax12Manager->releaseServo(id, false);
	}

    _ax12Manager->synchronize(ids);
}

QByteArray NServer::askAx12MovementFile()
{
	QString result;
	QTextStream stream(&result);
	_ax12Movements.writeToStream(stream);

	return result.toAscii();
}

void NServer::setAx12MovementFile(const QByteArray& data)
{
	QString strData(data);
	QTextStream stream(&strData);
	_ax12Movements.readFromStream(stream);

	QString ax12MovementFile = _dataDirectory.absoluteFilePath(AX12_MOVEMENT_FILENAME);
	_ax12Movements.writeToFile(ax12MovementFile);
}

void NServer::runAx12Movement(const QString& group, const QString& movement, float speedLimit)
{
	if (_ax12MovementRunner && !_ax12MovementRunner->isRunnning())
	{
		Q_ASSERT(!_ax12Manager->isReadingLoopStarted());

		_ax12MovementRunner->startMovement(group, movement, speedLimit);
	}
}

void NServer::ax12MovementFinished()
{

}

void NServer::strategyFinished()
{
	int currentStrat = 0;
	bool ok = stopStrategy(currentStrat);
	if (ok)
	{
		for(QMap<unsigned int, NetworkCommInterface*>::const_iterator it = _connections.constBegin(); it != _connections.constEnd(); ++it)
		{
			NetworkCommInterface* i = *it;
			i->sendStrategyStatus(currentStrat, false);
		}
	}
}

void NServer::serialPortStatusChanged(bool status)
{
	if (!status)
	{
		cleanRobotConnection();
		_robotConnected = false;

		for(QMap<unsigned int, NetworkCommInterface*>::iterator it = _connections.begin(); it != _connections.end(); ++it)
		{
			NetworkCommInterface* networkConnection = *it;
			networkConnection->setRobotInterface(0);
		}

		sendGlobalAnnoucement("The connection to the robot has been lost!");
		qDebug("The connection to the robot has been lost");
	}
}

void NServer::updateRobotConnection()
{
	for(QMap<unsigned int, NetworkCommInterface*>::iterator it = _connections.begin(); it != _connections.end(); ++it)
	{
		NetworkCommInterface* networkConnection = *it;
		networkConnection->setRobotInterface(_robotInterface);
	}
}

bool NServer::startStrategy(int strategyNum, bool mirror)
{
	if (_robotConnected && !_currentStrategy)
	{
		//start the strategy
		_currentStrategy = _strategiesEnumerator.start((StrategyEnumerator::Strategy)strategyNum);
		if (_currentStrategy)
		{
			if (_simulator)
				_simulator->stop();

			_strategyManager->setStrategy(_currentStrategy);
			_strategyManager->connectToRobot();

			if (_simulator)
			{
				_simulator->setRobotDefaultPosition(mirror);
				_simulator->start();
			}
		}

		if (_currentStrategy)
        {
			_currentStrategyId = strategyNum;
			for(QMap<unsigned int, NetworkCommInterface*>::const_iterator it = _connections.constBegin(); it != _connections.constEnd(); ++it)
            {
				NetworkCommInterface* i = *it;

                int strNum;
                bool isRunning;
                askStrategyStatus(strNum, isRunning);
                i->sendStrategyStatus(strNum, isRunning);
            }
        }
	}

	return _currentStrategy;
}

bool NServer::stopStrategy(int &currentStrategyNum)
{
	bool result = false;

	if (_robotConnected && _currentStrategy)
	{
		_strategyManager->setStrategy(0);
		_strategyManager->disconnectFromRobot();
		delete _currentStrategy;
		_currentStrategy = 0;

		for(QMap<unsigned int, NetworkCommInterface*>::const_iterator it = _connections.constBegin(); it != _connections.constEnd(); ++it)
        {
			NetworkCommInterface* i = *it;

            int strNum;
            bool isRunning;
            askStrategyStatus(strNum, isRunning);
            i->sendStrategyStatus(strNum, isRunning);
        }

		result = true;
	}

	currentStrategyNum = _currentStrategyId;

	return result;
}

void NServer::askStrategyStatus(int &currentStrategyNum, bool &isRunning)
{
	currentStrategyNum = _currentStrategyId;
	isRunning = _currentStrategy;
}

void NServer::askAutoStrategyInfo(bool& isEnabled, int& strategyNum, QString &robotPort, QString &ax12Port, bool& simulation, bool& mirror)
{
	strategyNum = _settings.value("AutoStrategyNum").toInt();
	isEnabled = _settings.value("autoStrategyEnabled").toBool();
	robotPort = _settings.value("robotPort").toString();
	ax12Port = _settings.value("ax12Port").toString();
	simulation = _settings.value("simulation").toBool();
	mirror = _settings.value("mirror").toBool();
}

void NServer::setAutoStrategy(bool enabled, int strategyNum, const QString &robotPort, const QString &ax12Port, bool simulation, bool mirror)
{
	_settings.setValue("AutoStrategyNum", strategyNum);
	_settings.setValue("autoStrategyEnabled", enabled);
	_settings.setValue("robotPort", robotPort);
	_settings.setValue("ax12Port", ax12Port);
	_settings.setValue("simulation", simulation);
	_settings.setValue("mirror", mirror);
}
