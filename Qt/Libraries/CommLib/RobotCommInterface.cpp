#include "RobotCommInterface.h"
#include "Instructions.h"

#include <QColor>

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

    data.add((qint16)d.getX());
    data.add((qint16)d.getY());
    data.add((qint16)(d.getTheta() * ANGLE_FACTOR));
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

void RobotCommInterface::askGraphs()
{
	getProtocol(0)->sendMessage(ASK_GRAPHS, Data());
}

void RobotCommInterface::sendNoticeOfReceipt(quint8 instruction, bool result)
{
	Data data;
	data.add(instruction);
	data.add(result);
	getProtocol(0)->sendMessage(AR, data);
}

void RobotCommInterface::executeAction(quint8 actionId, quint8 parameter)
{
	Data data;
	data.add(actionId);
	data.add(parameter);
    getProtocol(0)->sendMessage(ACTIONS, data);
}

void RobotCommInterface::enableSensor(SensorType type, quint8 sensorId)
{
	Data data;
	data.add((quint8)type);
	data.add(sensorId);
    getProtocol(0)->sendMessage(ENABLE_SENSOR, data);
}

void RobotCommInterface::disableSensor(SensorType type, quint8 sensorId)
{
	Data data;
	data.add((quint8)type);
	data.add(sensorId);
    getProtocol(0)->sendMessage(DISABLE_SENSOR, data);
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
                qint16 x, y;
				d.take(x).take(y);
				_listener->opponentPosition(x, y);
				break;
			}
			case ACTIVATED_SENSORS:
			{
				quint8 value;
				QList<quint8> values;
				while(!d.isEmpty())
				{
					d.take(value);
					values << value;
				}

				_listener->activatedSensors(values);
				break;
			}
			case SENSOR_EVENT:
			{
				qint8 type, sensorId, value;
				d.take(type).take(sensorId).take(value);

				_listener->sensorEvent((SensorType)type, sensorId, value);
				break;
			}
			case EVENT:
			{
				qint8 type;
				d.take(type);

				_listener->robotEvent((RobotEvent)type);
				break;
			}
			case GO:
				bool mirrored;
				d.take(mirrored);
				sendNoticeOfReceipt(GO, _listener->go(mirrored));
				break;
			case INIT:
				_listener->initReceived();
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
                break;
			}
			case REGISTER_GRAPH:
			{
				quint8 id, type, nbParam;
				QByteArray name;
				QStringList paramNames;
				d.take(id);
				d.take(type);
				d.take(name);
				d.take(nbParam);

				for(int i = 0; i < nbParam; ++i)
				{
					QByteArray paramName;
					d.take(paramName);
					paramNames << paramName;
				}

				_listener->registerGraph(id, (GraphType)type, name, paramNames);
				break;
			}
			case GRAPH_VALUES:
			{
				quint8 id;
				QList<float> values;
				d.take(id);
				while(!d.isEmpty())
				{
					float v = 0.0;
					d.take(v);
					values << v;
				}

				_listener->graphValues(id, values);
				break;
			}
			case GRAPH_SINGLE_VALUE:
			{
				quint8 id, paramNum;
				d.take(id).take(paramNum);

				float value;
				d.take(value);

				_listener->graphSingleValues(id, paramNum, value);
				break;
			}
			default:
				logger() << "Unkow instruction : " << instruction << Tools::endl;
		}
	}
}
