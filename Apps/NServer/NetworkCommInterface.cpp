#include "NetworkCommInterface.h"
#include "Instructions.h"
#include "NetworkCommListener.h"

#include "Strategies/StrategyEnumerator.h"

#include <qextserialenumerator.h>

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
	d.append(text.toAscii());
	getProtocol(0)->sendMessage(Comm::SEND_STRATEGIES, d);
}

void NetworkCommInterface::sendStrategyFiles(quint8 strategyNum, const QStringList& filenames)
{
	quint8 count = qMin<int>(filenames.count(), 255);
	Data data;
	data.add(strategyNum);
	data.add(count);
	for(int i = 0; i < count; ++i)
	{
		QByteArray name = filenames.value(i).toAscii().mid(0, 255);
		data.add(name);
	}

	getProtocol(0)->sendMessage(SEND_STRATEGY_FILES, data);
}

void NetworkCommInterface::sendStrategyFileData(quint8 strategyNum, const QString& filename, const QByteArray& fileData)
{
	Data data;
	data.add(strategyNum);

	QByteArray name = filename.toAscii().mid(0, 255);
	data.add(name);
	data.append(fileData);

	getProtocol(0)->sendMessage(SEND_STRATEGY_FILE_DATA, data);
}

void NetworkCommInterface::sendStrategyStatus(quint8 strategyNum, bool isRunning)
{
	Data data;
	data.add(strategyNum);
	data.add(isRunning);

	getProtocol(0)->sendMessage(STRATEGY_STATUS, data);
}

void NetworkCommInterface::sendAutoStrategyInfo(bool enabled, quint8 strategyNum, const QByteArray &robotPort, const QByteArray &ax12Port, bool simulation, bool mirror)
{
	Data data;
	data.add(strategyNum);
	data.add(robotPort);
	data.add(ax12Port);
	data.add(enabled, simulation, mirror);

	getProtocol(0)->sendMessage(AUTO_STRATEGY_INFO, data);
}

void NetworkCommInterface::sendAvailableSerialPorts()
{
	Data data;

	QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
	foreach(const QextPortInfo& port, ports)
	{
		QByteArray portName = port.portName.toAscii();
		data.add(portName);
	}

	getProtocol(0)->sendMessage(SERIAL_PORTS, data);
}

void NetworkCommInterface::sendAx12Positions(const QList<quint8> ids, const QList<float> &positions)
{
	int nb = qMin(ids.count(), positions.count());
	Data data;
	data.add((quint8)nb);
	for(int i = 0; i < nb; ++i)
	{
		data.add(ids.value(i));
		data.add(positions.value(i));
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

			_listener->askAx12Positions(this, ids);
			break;
		}
		case MOVE_AX12:
		{
			quint8 nb;
			d.take(nb);

			QList<Comm::Ax12Info> infos;
			float maxSpeed;
			for(int i = 0; i < nb; ++i)
			{
				quint8 id;
				float angle, speed, torque;
				d.take(maxSpeed);
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
				quint8 id, locked;
				d.take(id).take(locked);
				lockInfo[id] = (locked != 0);
			}
			_listener->lockAx12(lockInfo);
			break;
		}
		
		case ASK_AX12_MVT_FILE:
		{
			QByteArray fileData = _listener->askAx12MovementFile();
			if(!fileData.isEmpty())
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
			d.take(group).take(movement).take(speedLimit);
			
			_listener->runAx12Movement(group, movement, speedLimit);
			
			break;
		}
		case ASK_AUTO_STRATEGY_INFO:
		{
			int currentStratNum = 0;
			QString robotPort, ax12Port;
			bool isEnabled = false;
			bool simu = false;
			bool mirror = false;
			_listener->askAutoStrategyInfo(isEnabled, currentStratNum, robotPort, ax12Port, simu, mirror);

			sendAutoStrategyInfo(isEnabled, currentStratNum, robotPort.toAscii(), ax12Port.toAscii(), simu, mirror);
			break;
		}
		case SET_AUTO_STRATEGY:
		{
			quint8 currentStratNum = 0;
			QByteArray robotPort, ax12Port;
			bool isEnabled = false;
			bool simu = false;
			bool mirror = false;
			d.take(currentStratNum).take(robotPort).take(ax12Port).take(isEnabled, simu, mirror);

			_listener->setAutoStrategy(isEnabled, currentStratNum, robotPort, ax12Port, simu, mirror);
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
		case SET_STRATEGY_FILE_DATA:
		{
			quint8 strNum, filenameSize;
			d.take(strNum);
			d.take(filenameSize);

			QByteArray fileName = d.left(filenameSize);
			QByteArray fileData = d.mid(filenameSize);

			_listener->setStrategyFileData(strNum, fileName, fileData);
			break;
		}

		case ASK_STRATEGY_FILE_DATA:
		{
			quint8 strNum;
			d.take(strNum);

			QString fileName = d;
			QByteArray data = _listener->askStrategyFileData(strNum, fileName);
			if (!data.isEmpty())
				sendStrategyFileData(strNum, fileName, data);

			break;
		}

		case ASK_STRATEGY_FILES:
		{
			quint8 strNum;
			d.take(strNum);

			QStringList files = _listener->askStrategyFiles(strNum);
			if (!files.isEmpty())
				sendStrategyFiles(strNum, files);

			break;
		}
		case ASK_STRATEGIES:
			sendStrategies();
			break;
		case RESET_STRATEGY_FILE:
		{
			quint8 strNum;
			d.take(strNum);

			QString fileName = d;
			_listener->resetStrategyFile(strNum, fileName);
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
			sendNoticeOfReceipt(PING_SERVER, _listener->pingReceived());
			break;
		case AR:
			quint8 inst;
			bool result;
			d.take(inst).take(result);
			noticeOfReceiptReceived(inst);
			_listener->noticeOfReceipt(inst, result);
			break;
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
	if (_connected)
		getProtocol()->sendMessage(instruction, data);
}
