#include "NetworkCommInterface.h"
#include "Instructions.h"
#include "NetworkCommListener.h"
#include "AbstractLogger.h"

#include "Strategies/StrategyEnumerator.h"

#include <QSerialPortInfo>

using namespace Comm;

NetworkCommInterface::NetworkCommInterface(Comm::AbstractProtocol* protocol, Comm::RobotCommInterface* robotInterface, Tools::AbstractLogger* logger, QObject* parent) :
	AbstractCommInterface(protocol, logger, parent), _robotInterface(robotInterface), _listener(0), _connected(false)
{
}

void NetworkCommInterface::setListener(NetworkCommListener* listener)
{
	_listener = listener;
}

NetworkCommListener* NetworkCommInterface::getListener() const
{
	return _listener;
}

void NetworkCommInterface::setRobotInterface(RobotCommInterface *robotInterface)
{
	_robotInterface = robotInterface;
	if (_robotInterface)
		connect(_robotInterface->getProtocol(), SIGNAL(message(quint8,Comm::Data)), this, SLOT(readFromRobot(quint8,Comm::Data)));
	else
		_connected = false;
}

void NetworkCommInterface::sendAnnouncement(const QByteArray &message)
{
	Data d;
	d.append(message);
	getProtocol(0)->sendMessage(Comm::ANNOUNCEMENT, d);
}

void NetworkCommInterface::sendStrategies()
{
	QStringList strategies = StrategyEnumerator::getAvailableStrategies();
	QString text = strategies.join(";;");
	Data d;
	d.append(text.toLatin1());
	getProtocol(0)->sendMessage(Comm::SEND_STRATEGIES, d);
}

void NetworkCommInterface::sendFiles(quint8 category, const QStringList& filenames)
{
	quint8 count = qMin<int>(filenames.count(), 255);
	Data data;
	data.add(category);
	data.add(count);
	for(int i = 0; i < count; ++i)
	{
		QByteArray name = filenames.value(i).toLatin1().mid(0, 255);
		data.add(name);
	}

	getProtocol(0)->sendMessage(CONFIGURATION_FILES, data);
}

void NetworkCommInterface::sendFileData(quint8 category, const QString& filename, const QByteArray& fileData)
{
	Data data;
	data.add(category);

	QByteArray name = filename.toLatin1().mid(0, 255);
	data.add(name);
	data.append(fileData);

	getProtocol(0)->sendMessage(CONFIGURATION_FILE_DATA, data);
}

void NetworkCommInterface::sendFileEvent(quint8 category, const QString &filename, quint8 event)
{
	Data data;
	data.add(category);

	QByteArray name = filename.toLatin1().mid(0, 255);
	data.add(name);

	data.add(event);

	getProtocol(0)->sendMessage(CONFIGURATION_FILE_DATA, data);
}

void NetworkCommInterface::sendStrategyStatus(quint8 strategyNum, bool isRunning)
{
	Data data;
	data.add(strategyNum);
	data.add(isRunning);

	getProtocol(0)->sendMessage(STRATEGY_STATUS, data);
}

void NetworkCommInterface::sendAutoStrategyInfo(bool enabled, quint8 strategyNum, const QByteArray &robotPort, const QByteArray &ax12Port, bool simulation, bool mirror, quint8 delayInSeconds)
{
	Data data;
	data.add(strategyNum);
	data.add(robotPort);
	data.add(ax12Port);
	data.add(enabled, simulation, mirror);
	data.add(delayInSeconds);

	getProtocol(0)->sendMessage(AUTO_STRATEGY_INFO, data);
}

void NetworkCommInterface::sendAvailableSerialPorts()
{
	Data data;

	QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
	foreach(const QSerialPortInfo& port, ports)
	{
		QByteArray portName = port.portName().toLatin1();
		data.add(portName);
	}

	getProtocol(0)->sendMessage(SERIAL_PORTS, data);
}

void NetworkCommInterface::sendAx12Positions(const QList<quint8> ids, const QList<float> &positions, const QList<float> &loads)
{
	int nb = qMin(ids.count(), positions.count());
	Data data;
	data.add((quint8)nb);
	for(int i = 0; i < nb; ++i)
	{
		data.add(ids.value(i));
		data.add(positions.value(i));
		data.add(loads.value(i));
	}

	getProtocol(0)->sendMessage(AX12_POSITIONS, data);
}

void NetworkCommInterface::sendAx12Movements(const QByteArray& data)
{
	Data d;
	d.append(data);
	getProtocol(0)->sendMessage(AX12_MVT_FILE, d);
}

void NetworkCommInterface::ping()
{
	Data data;
	getProtocol(0)->sendMessage(PING, data);
	checkNoticeOfReceipt(PING, data, getProtocol(0));
}

void NetworkCommInterface::sendNoticeOfReceipt(quint8 instruction, bool result)
{
	Data data;
	data.add(instruction);
	data.add(result);
	getProtocol(0)->sendMessage(AR, data);
}

void NetworkCommInterface::read(quint8 instruction, const Comm::Data &data)
{
	Data d(data);
	switch(instruction)
	{
		case ASK_AX12_POSITIONS:
		{
			quint8 nb;
			d.take(nb);

			QList<quint8> ids;
			for(int i = 0; i < nb; ++i)
			{
				quint8 id;
				d.take(id);

				ids << id;
			}

            bool recursive;
            d.take(recursive);

            _listener->askAx12Positions(this, ids, recursive);
			break;
		}
		case MOVE_AX12:
		{
			quint8 nb;
			float maxSpeed;
			d.take(maxSpeed);
			d.take(nb);

			QList<Comm::Ax12Info> infos;
			for(int i = 0; i < nb; ++i)
			{
				quint8 id;
				float angle, speed, torque;

				d.take(id);
				d.take(angle).take(speed).take(torque);


				Comm::Ax12Info info;
				info.id = id;
				info.angle = angle;
				info.speed = speed;
				info.torque = torque;
				infos << info;

			}

			_listener->moveAx12(maxSpeed, infos);
			break;
		}
		case LOCK_AX12:
		{
			quint8 nb;
			d.take(nb);
			QMap<quint8, bool> lockInfo;
			for(int i = 0; i < nb; ++i)
			{
				quint8 id;
				bool locked = false;
				d.take(id).take(locked);
				lockInfo[id] = (locked != 0);
			}
			_listener->lockAx12(lockInfo);
			break;
		}
		
		case ASK_AX12_MVT_FILE:
		{
			QByteArray fileData = _listener->askAx12MovementFile();
            sendAx12Movements(fileData);
			break;
		}
		case SET_AX12_MVT_FILE:
		{
			_listener->setAx12MovementFile(data);
			break;
		}
		case RUN_AX12_MVT:
		{
			QByteArray group, movement;
			float speedLimit;
            qint8 lastPositionIndex;
            d.take(group).take(movement).take(speedLimit).take(lastPositionIndex);
			
            _listener->runAx12Movement(group, movement, speedLimit, lastPositionIndex);
			
			break;
		}
		case ASK_AUTO_STRATEGY_INFO:
		{
			int currentStratNum = 0;
			QString robotPort, ax12Port;
			bool isEnabled = false;
			bool simu = false;
			bool mirror = false;
			int delay = 20;
			_listener->askAutoStrategyInfo(isEnabled, currentStratNum, robotPort, ax12Port, simu, mirror, delay);

			sendAutoStrategyInfo(isEnabled, currentStratNum, robotPort.toLatin1(), ax12Port.toLatin1(), simu, mirror, delay);
			break;
		}
		case SET_AUTO_STRATEGY:
		{
			quint8 currentStratNum = 0;
			QByteArray robotPort, ax12Port;
			bool isEnabled = false;
			bool simu = false;
			bool mirror = false;
			quint8 delay = 20;
			d.take(currentStratNum).take(robotPort).take(ax12Port).take(isEnabled, simu, mirror).take(delay);

			_listener->setAutoStrategy(isEnabled, currentStratNum, robotPort, ax12Port, simu, mirror, delay);
			break;
		}

		case START_STRATEGY:
		{
			quint8 stratNum;
			bool mirror;
			d.take(stratNum).take(mirror);

            _listener->startStrategy(stratNum, mirror);
			break;
		}
		case STOP_STRATEGY:
		{
			int currentStratNum = 0;
            _listener->stopStrategy(currentStratNum);
			break;
		}
		case ASK_STRATEGY_STATUS:
		{
			int currentStratNum = 0;
			bool isRunning = false;
			_listener->askStrategyStatus(currentStratNum, isRunning);
			sendStrategyStatus(currentStratNum, isRunning);
			break;
		}
		case ASK_STRATEGIES:
			sendStrategies();
			break;

		case SET_FILE_DATA:
		{
			quint8 cat, filenameSize;
			d.take(cat);
			d.take(filenameSize);

			QByteArray fileName = d.left(filenameSize);
			QByteArray fileData = d.mid(filenameSize);

			_listener->setFileData(cat, fileName, fileData, this);
			break;
		}
		case ASK_FILE_DATA:
		{
			quint8 cat;
			d.take(cat);

			QString fileName = d;
			QByteArray data = _listener->askFileData(cat, fileName);
			sendFileData(cat, fileName, data);

			break;
		}

		case ASK_FILES:
		{
			quint8 cat;
			d.take(cat);

			QStringList files = _listener->askFiles(cat);
			sendFiles(cat, files);

			break;
		}
		case RESET_FILE:
		{
			quint8 cat;
			d.take(cat);

			QString fileName = d;
			_listener->resetFile(cat, fileName, this);
			break;
		}

		case ASK_SERIAL_PORTS:
			sendAvailableSerialPorts();
			break;

		case CONNECT:
		{
			bool simu = false;
			d.take(simu);

			QByteArray robotPort, ax12Port;
			d.take(robotPort).take(ax12Port);

			QByteArray message;
			_connected =  _listener->connectToRobot(this, simu, robotPort, ax12Port, message);

			if (!message.isEmpty())
				sendAnnouncement(message);
			sendNoticeOfReceipt(CONNECT, _connected);
			break;
		}
		case DISCONNECT:
			_connected = !_listener->disconnectToRobot(this);
			if (!_connected)
				sendAnnouncement("At last, you finished! Well, I hope you didn't break him already.");
			sendNoticeOfReceipt(DISCONNECT, !_connected);
			break;
		case UPDATE_SERVER:
			_listener->updateServer(d);
			sendAnnouncement("What are you doing with me ? Noooooooo, not my head! Aaaaaaaaah!!!.");
			sendAnnouncement("You are lucky that it is not working yet... I would have killed you for that!");
			break;
		case PING_SERVER:
			sendNoticeOfReceipt(PING_SERVER, _listener->networkPingReceived());
			break;
		case AR:
			quint8 inst;
			bool result;
			d.take(inst).take(result);
			noticeOfReceiptReceived(inst);
			_listener->networkNoticeOfReceipt(inst, result);
			break;
		case SET_PARAMETERS:
		{
			quint8 size = 0;
			d.take(size);

			QList<float> values;
			for(int i = 0; i < size; ++i)
			{
				float v = 0.0;
				d.take(v);
				values << v;
			}

			_listener->saveParameters(values);
			if (_connected && _robotInterface)
				_robotInterface->setParameters(values);
			break;
		}
		case RESET_PARAMETERS:
		{
			_listener->resetParameters();
			_robotInterface->askParameters();

			break;
		}
		case RUN_SCRIPT:
		{
			if (!_listener->runScript(data))
				sendNoticeOfReceipt(RUN_SCRIPT, false);
			break;
		}
		default:
			if (Instruction::pcToRobotInstructions().contains(instruction) || Instruction::globalInstructions().contains(instruction))
			{
				if (_connected && _robotInterface)
					_robotInterface->getProtocol()->sendMessage(instruction, data);
			}
			else
				logger() << "Unkow instruction: " << instruction << Tools::endl;
	}
}

void NetworkCommInterface::readFromRobot(quint8 instruction, const Data &data)
{
	Data d(data);
	if (_connected)
	{
		if (instruction == PARAMETERS)
		{
			QList<float> existingValues = _listener->getParameters();

			quint8 size = 0;
			d.take(size);

			QList<float> values;
			for(int i = 0; i < size; ++i)
			{
				float v = 0.0;
				d.take(v);
				values << v;
			}

			bool changed = false;
			if (values.count() < existingValues.count())
			{
				changed = true;
				for(int i = values.count(); i < existingValues.count(); ++i)
					values.takeLast();
			}

			if (values.count() > existingValues.count())
			{
				changed = true;
				for(int i = existingValues.count(); i < values.count(); ++i)
					existingValues << values.value(i);
			}

			if (changed)
				_listener->saveParameters(existingValues);

			Data newData;
			newData.add((quint8)existingValues.count());
			foreach(float v, existingValues)
				newData.add(v);

			getProtocol()->sendMessage(instruction, newData);

		}
		else
			getProtocol()->sendMessage(instruction, data);
	}
}


CommLogger::CommLogger(NetworkCommInterface *comm) : _comm(comm)
{
}

void CommLogger::log(const char *text)
{
	if (!_comm)
		return;

	QByteArray s(text);
	int endlIndex = s.indexOf(Tools::endl);
	if (endlIndex >= 0)
	{
		if (endlIndex > 0)
			_str += s.left(endlIndex);

		_comm->sendAnnouncement(_str);
		_str.clear();

		if (endlIndex < s.count() - 1)
			_str = s.mid(endlIndex + 1);
	}
	else
		_str += s;
}

void CommLogger::log(char text)
{
	if (!_comm)
		return;

	if (text == Tools::endl)
	{
		_comm->sendAnnouncement(_str);
		_str.clear();
	}
	else
		_str += QString(text);
}

NetworkCommInterface *CommLogger::getComm() const
{
	return _comm;
}

void CommLogger::setComm(NetworkCommInterface *comm)
{
	_comm = comm;
}
