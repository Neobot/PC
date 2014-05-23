#include "NetworkClientCommInterface.h"

#include <QFile>
#include <QStringList>

using namespace Comm;


NetworkClientCommInterface::NetworkClientCommInterface(AbstractProtocol *protocol, Tools::AbstractLogger *logger, CommListener *commListener, NetworkClientCommListener *networkListener, QObject *parent)
	: RobotCommInterface(protocol, new AX12CommManager, logger, commListener, parent), _listener(networkListener)
{
	disableNoticeOfReceiptChecking();
}


void NetworkClientCommInterface::setNetworkListener(NetworkClientCommListener *listener)
{
	_listener = listener;
}

void NetworkClientCommInterface::connectToRobot(bool simulation, const QString &robotPort, const QString &ax12Port)
{
	Data d;
	d.add(simulation);
	d.add(robotPort.toLatin1());
	d.add(ax12Port.toLatin1());

	getProtocol()->sendMessage(CONNECT, d);
}

void NetworkClientCommInterface::disconnectFromRobot()
{
	getProtocol()->sendMessage(DISCONNECT, Data());
}

bool NetworkClientCommInterface::updateServer(const QString &serverExecutablePath)
{
	QFile serverExe(serverExecutablePath);
	if (!serverExe.open(QIODevice::ReadOnly))
		return false;

	QByteArray content = serverExe.readAll();
	Data d;
	d.append(content);

	getProtocol()->sendMessage(UPDATE_SERVER, d);

	return true;
}

void NetworkClientCommInterface::sendPingToServer()
{
	getProtocol()->sendMessage(PING_SERVER, Data());
}

void NetworkClientCommInterface::askStrategies()
{
	getProtocol()->sendMessage(ASK_STRATEGIES, Data());
}

void NetworkClientCommInterface::askStrategyFiles(quint8 strategyNum)
{
	Data d;
	d.add(strategyNum);
	getProtocol()->sendMessage(ASK_STRATEGY_FILES, d);
}

void NetworkClientCommInterface::askStrategyFileData(quint8 strategyNum, const QString &filename)
{
	Data d;
	d.add(strategyNum);
	d.append(filename.toLatin1());
	getProtocol()->sendMessage(ASK_STRATEGY_FILE_DATA, d);
}

void NetworkClientCommInterface::sendStrategyFileData(quint8 strategyNum, const QString &filename, const QByteArray &data)
{
	Data d;
	d.add(strategyNum);

	QByteArray name = filename.toLatin1().mid(0, 255);
	d.add(name);
	d.append(data);

	getProtocol()->sendMessage(SET_STRATEGY_FILE_DATA, d);
}

void NetworkClientCommInterface::resetStrategyFileData(quint8 strategyNum, const QString &filename)
{
	Data d;
	d.add(strategyNum);
	d.append(filename.toLatin1());
	getProtocol()->sendMessage(RESET_STRATEGY_FILE, d);
}

void NetworkClientCommInterface::startStrategy(quint8 strategyNum, bool mirror)
{
	Data d;
	d.add(strategyNum);
	d.add(mirror);
	getProtocol()->sendMessage(START_STRATEGY, d);
}

void NetworkClientCommInterface::stopStrategy()
{
	getProtocol()->sendMessage(STOP_STRATEGY, Data());
}

void NetworkClientCommInterface::askStrategyStatus()
{
	getProtocol()->sendMessage(ASK_STRATEGY_STATUS, Data());
}

void NetworkClientCommInterface::askAutoStrategyInfo()
{
	getProtocol()->sendMessage(ASK_AUTO_STRATEGY_INFO, Data());
}

void NetworkClientCommInterface::setAutoStrategy(bool enabled, quint8 strategyNum, const QString &robotPort, const QString &ax12Port, bool simulation, bool mirroredSimulation, quint8 startDelayInSecond)
{
	Data d;
	d.add(strategyNum);
	d.add(robotPort.toLatin1());
	d.add(ax12Port.toLatin1());
	d.add(enabled, simulation | mirroredSimulation, mirroredSimulation);
	d.add(startDelayInSecond);
	
	getProtocol()->sendMessage(SET_AUTO_STRATEGY, d);
}

void NetworkClientCommInterface::askSerialPorts()
{
	getProtocol()->sendMessage(ASK_SERIAL_PORTS, Data());
}

void NetworkClientCommInterface::askAx12Positions(const QList<int> &ids, bool recursive)
{
	Data d;
	d.add((quint8)ids.count());
	foreach(int id, ids)
		d.add((quint8)id);

    d.add(recursive);

	getProtocol()->sendMessage(ASK_AX12_POSITIONS, d);
}

void NetworkClientCommInterface::moveAx12(const Comm::Ax12Info &ax12Info)
{
	QList<Comm::Ax12Info> ax12List;
	ax12List << ax12Info;

	moveAx12(ax12List);
}

void NetworkClientCommInterface::moveAx12Smoothed(float maxSpeed, const QList<Comm::Ax12Info> &ax12Infos)
{
	Data d;
	d.add(maxSpeed);
	d.add((quint8)ax12Infos.count());
	foreach(const Ax12Info& info, ax12Infos)
	{
		d.add(info.id);
		d.add(info.angle);
		d.add(info.speed);
		d.add(info.torque);
	}

	getProtocol()->sendMessage(MOVE_AX12, d);
}

void NetworkClientCommInterface::lockAx12(const QMap<int, bool> &lockedServoInfo)
{
	Data d;
	d.add((quint8)lockedServoInfo.count());
	for(QMap<int, bool>::const_iterator it = lockedServoInfo.constBegin(); it != lockedServoInfo.constEnd(); ++it)
	{
		d.add((quint8)it.key());
		d.add(it.value());
	}

	getProtocol()->sendMessage(LOCK_AX12, d);
}

void NetworkClientCommInterface::moveAx12(const QList<Comm::Ax12Info> &ax12Infos)
{
	moveAx12Smoothed(-1, ax12Infos);
}

void NetworkClientCommInterface::askAx12Movements()
{
	getProtocol()->sendMessage(ASK_AX12_MVT_FILE, Data());
}

void NetworkClientCommInterface::setAx12Movements(const QByteArray& data)
{
	Data d;
	d.append(data);
	getProtocol()->sendMessage(SET_AX12_MVT_FILE, d);
}

void NetworkClientCommInterface::runAx12Movement(const QString& group, const QString& movement, float speedLimit, int lastPositionIndex)
{
	Data d;
	d.add(group.toLatin1());
	d.add(movement.toLatin1());
	d.add(speedLimit);
    d.add((qint8)lastPositionIndex);
	
	getProtocol()->sendMessage(RUN_AX12_MVT, d);
}

void NetworkClientCommInterface::read(quint8 instruction, const Comm::Data &data)
{
	Data d(data);
	switch(instruction)
	{
		case AX12_POSITIONS:
		{
			quint8 nbAx12;
			d.take(nbAx12);

			QList<float> positions;
			QList<float> loads;
			QList<int> ids;
			for(int i = 0; i < nbAx12; ++i)
			{
				quint8 id;
				float pos, load;
				d.take(id).take(pos).take(load);
				positions << pos;
				loads << load;
				ids << id;
			}

			_listener->setAx12Positions(ids, positions, loads);
			break;
		}
		case AX12_MVT_FILE:
		{
			_listener->setAx12MovementsFile(data);
			break;
		}
		case AUTO_STRATEGY_INFO:
		{
			quint8 currentStratNum = 0;
			QByteArray robotPort, ax12Port;
			bool isEnabled = false;
			bool simu = false;
			bool mirror = false;
			quint8 delay;
			d.take(currentStratNum).take(robotPort).take(ax12Port).take(isEnabled, simu, mirror).take(delay);

			_listener->autoStrategyInfo(isEnabled, currentStratNum, robotPort, ax12Port, simu, mirror, delay);
			break;
		}
		case STRATEGY_STATUS:
		{
			quint8 currentStratNum = 0;
			bool isRunning = false;
			d.take(currentStratNum).take(isRunning);
			_listener->strategyStatusUpdated(currentStratNum, isRunning);
			break;

		}
		case SEND_STRATEGY_FILE_DATA:
		{
			quint8 stratNum, filenameSize;
			d.take(stratNum).take(filenameSize);

			QByteArray filename;
			d.take(filename, filenameSize);

			QByteArray fileData = d;

			_listener->setStrategyFileData(stratNum, filename, fileData);

			break;
		}
		case SEND_STRATEGY_FILES:
		{
			quint8 stratNum, nbFiles;
			d.take(stratNum).take(nbFiles);

			QStringList files;
			for(int i = 0; i < nbFiles; ++i)
			{
				QByteArray filename;
				d.take(filename);

				files << filename;
			}

			_listener->setStrategyFiles(stratNum, files);

			break;
		}
		case SEND_STRATEGIES:
		{
			QString stratgiesStr = d;
			QStringList strategies = stratgiesStr.split(";;", QString::SkipEmptyParts);

			_listener->setStrategies(strategies);

			break;
		}

		case SERIAL_PORTS:
		{
			QStringList ports;
			while (!d.isEmpty())
			{
				QByteArray p;
				d.take(p);
				if (!p.isEmpty())
					ports << p;
			}

			_listener->setSerialPorts(ports);

			break;
		}

		case ANNOUNCEMENT:
		{
			QByteArray text;
			while(!d.isEmpty())
			{
				quint8 c;
				d.take(c);
				text.append((char)c);
			}
			_listener->serverAnnouncement(text);
			break;
		}
		case AR:
		{
			quint8 inst;
			bool result;
			d.take(inst).take(result);
			if (Instruction::clientToServerInstructions().contains(inst))
			{
				noticeOfReceiptReceived(inst);
				_listener->networkNoticeOfReceipt(inst, result);
			}
			else
				RobotCommInterface::read(instruction, data);
			break;
		}
		default:
			RobotCommInterface::read(instruction, data);
	}
}

