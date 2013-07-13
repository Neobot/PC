#include "RobotCommInterface.h"
#include "Instructions.h"

using namespace Comm;
using namespace Tools;

RobotCommInterface::RobotCommInterface(AbstractProtocol* protocolCard, AX12CommManager* ax12Manager, AbstractLogger* logger, CommListener* listener, QObject* parent)
	: AbstractCommInterface(logger, parent), _listener(listener), _ax12(ax12Manager)
{
	addProtocol(protocolCard);
}

void RobotCommInterface::setListener(CommListener* listener)
{
	_listener = listener;
}

CommListener* RobotCommInterface::getListener() const
{
	return _listener;
}

AX12CommManager *RobotCommInterface::getAx12Manager() const
{
	return _ax12;
}

void RobotCommInterface::addPositionToData(const RMovement& pos, Data& data)
{
	RPoint d = pos.getDestination();

	data.add((quint16)d.getX());
	data.add((quint16)d.getY());
	data.add((quint16)(d.getTheta() * ANGLE_FACTOR));
	data.add((quint8)pos.getType());
	data.add((quint8)pos.getDeplacement());
    data.add((quint8)pos.getSpeed());
	data.add(pos.isStopPoint());
}

void RobotCommInterface::ping()
{
	Data data;
	getProtocol(0)->sendMessage(PING, data);
	checkNoticeOfReceipt(PING, data, getProtocol(0));
}

void RobotCommInterface::setPosition(qint16 x, qint16 y, double theta)
{
	Data data;
	data.add(x);
	data.add(y);
	data.add((quint16)(theta * ANGLE_FACTOR));
	getProtocol(0)->sendMessage(SET_POS, data);
	checkNoticeOfReceipt(SET_POS, data, getProtocol(0));
}

void RobotCommInterface::setDestination(RMovement pos)
{
	Data data;
	addPositionToData(pos, data);
	getProtocol(0)->sendMessage(DEST_ADD, data);
	checkNoticeOfReceipt(DEST_ADD, data, getProtocol(0));
}

void RobotCommInterface::replaceDestination(QList<RMovement> positions)
{
	Data data;
	data.add((quint8)positions.count());
	for(QList<RMovement>::const_iterator it = positions.constBegin(); it != positions.constEnd(); ++it)
		addPositionToData(*it, data);

	getProtocol(0)->sendMessage(DEST_REPLACE, data);
	checkNoticeOfReceipt(DEST_REPLACE, data, getProtocol(0));
}

void RobotCommInterface::flush()
{
	Data data;

	getProtocol(0)->sendMessage(FLUSH, data);
	checkNoticeOfReceipt(FLUSH, data, getProtocol(0));
}

void RobotCommInterface::moveAX12(quint8 id, float angle, float speed)
{
	_ax12->setGoal(id, angle, speed, true);
}

void RobotCommInterface::getSensor(quint8 id)
{
	Data data;
	data.add(id);
	getProtocol(0)->sendMessage(GET_SENSOR, data);
}

void RobotCommInterface::setParameters(const QList<float> &values)
{
	Data data;
	data.add((quint8)values.count());
	foreach(float v, values)
		data.add(v);

	getProtocol(0)->sendMessage(SET_PARAMETERS, data);
}

void RobotCommInterface::askParameters()
{
	getProtocol(0)->sendMessage(ASK_PARAMETERS, Data());
}

void RobotCommInterface::sendNoticeOfReceipt(quint8 instruction, bool result)
{
	Data data;
	data.add(instruction);
	data.add(result);
	getProtocol(0)->sendMessage(AR, data);
}

void RobotCommInterface::moveServo(quint8 servo, quint8 position)
{
	Data data;
	data.add(servo);
	data.add(position);
	getProtocol(0)->sendMessage(ACTIONS, data);
}

void RobotCommInterface::read(quint8 instruction, const Data& data)
{
	if (_listener)
	{
		Data d(data);
		switch(instruction)
		{
			case COORD:
			{
				qint16 x, y, theta;
				quint8 direction;
				d.take(x).take(y).take(theta).take(direction);
				_listener->coordinates(x, y, (double)theta / ANGLE_FACTOR, direction);
				break;
			}
			case OPPONENT:
			{
				quint16 x, y;
				d.take(x).take(y);
				_listener->opponentPosition(x, y);
				break;
			}
			case IS_ARRIVED:
			{
				_listener->isArrived();
				break;
			}
			case IS_BLOCKED:
			{
				_listener->isBlocked();
				break;
			}
			case AVOIDING_SENSORS:
			{
				quint8 value;
				QList<quint8> values;
				while(!d.isEmpty())
				{
					d.take(value);
					values << value;
				}
				_listener->avoidingSensors(values);
				break;
			}
			case OTHER_SENSORS:
			{
				quint8 value;
				QList<quint8> values;
				while(!d.isEmpty())
				{
					d.take(value);
					values << value;
				}
				_listener->otherSensors(values);
				break;
			}
			case MICROSWITCHS:
			{
				quint8 value;
				QList<quint8> values;
				while(!d.isEmpty())
				{
					d.take(value);
					values << value;
				}
				_listener->microswitchs(values);
				break;
			}
			case INIT_DONE:
				sendNoticeOfReceipt(INIT_DONE, _listener->initDone());
				break;
			case GO:
				bool mirrored;
				d.take(mirrored);
				sendNoticeOfReceipt(GO, _listener->go(mirrored));
				break;
			case PING:
				sendNoticeOfReceipt(PING, _listener->pingReceived());
				break;
			case AR:
				quint8 inst;
				bool result;
				d.take(inst).take(result);
				noticeOfReceiptReceived(inst);
				_listener->noticeOfReceipt(inst, result);
				break;
			case RESTART:
				sendNoticeOfReceipt(RESTART, _listener->restart());
				break;
			case QUIT:
				_listener->quit();
				break;
			case OBJECTIVE:
			{
				qint16 x, y, theta;
				d.take(x).take(y).take(theta);
				_listener->objective(x, y, (double)theta / ANGLE_FACTOR);
				break;
			}
			case LOG:
			{
				QByteArray text;
				while(!d.isEmpty())
				{
					quint8 c;
					d.take(c);
					text.append((char)c);
				}
				_listener->log(text);
				break;
			}
			case PARAMETERS:
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
				_listener->parameters(values);
				break;
			}
			case PARAMETER_NAMES:
			{
				quint8 size;
				d.take(size);

				QString nameStr = d;
				QStringList names = nameStr.split(";;");

				_listener->parameterNames(names);
			}
			default:
				logger() << "Unkow instruction : " << instruction << Tools::endl;


		}
	}
}
