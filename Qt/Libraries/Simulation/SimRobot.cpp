#include "SimRobot.h"
#include "Instructions.h"
#include "ToolsLib.h"

#include <QtDebug>

SimRobot::SimRobot(SimProtocol* protocol, SimCommInterface* networkInterface)
    : AbstractCommInterface(0), _protocol(protocol), _interface(networkInterface)
{
    connect(_protocol, SIGNAL(message2(quint8, const Comm::Data&)), this, SLOT(read(quint8, const Comm::Data&)));

    disableNoticeOfReceiptChecking();
}

SimRobot::~SimRobot()
{
}

void SimRobot::read(quint8 instruction, const Comm::Data &data)
{
	Comm::Data d(data);

	if (!_interface)
		return;

	switch(instruction)
	{
		case Comm::SET_POS:
		{
			qint16 x, y, t;
			d.take(x).take(y).take(t);

			double theta = t / Comm::ANGLE_FACTOR;

			_interface->setPosition(x, y, theta);
			break;
		}
		case Comm::DEST_ADD:
		{
			qint16 x, y, t;
			quint8 type;
			quint8 deplacementType;
			quint8 speed;
			bool isStopPoint;

			d.take(x).take(y).take(t).take(type).take(deplacementType).take(speed).take(isStopPoint);

			double theta = t / Comm::ANGLE_FACTOR;
            _interface->addDestination(x, y, theta, type, isStopPoint, speed);
			break;
		}
		case Comm::ACTIONS:
			quint8 actionType;
			qint16 parameter;

			d.take(actionType).take(parameter);
			//_interface->executeAction(actionType, parameter);
			break;
		case Comm::FLUSH:
			_interface->flush();
			break;
		case Comm::PING:
			sendNoticeOfReceipt(Comm::PING, true);
			break;
		case Comm::ASK_PARAMETERS:
			_interface->askParameters();
			break;
		case Comm::SET_PARAMETERS:
			quint8 nb;
			d.take(nb);

			QList<float> values;
			for(int i = 0; i < nb; ++i)
			{
				float v;
				d.take(v);
				values << v;
			}

			_interface->setParameters(values);
			break;
	}
}

void SimRobot::sendCoordinates(double x, double y, double theta, bool forward)
{
	theta = Tools::angleInMinusPiPlusPi(theta);
    Comm::Data d;
	d.add((qint16)x).add((qint16)y).add((qint16)(theta * Comm::ANGLE_FACTOR)).add(forward ? (quint8)1 : (quint8)0);
	_protocol->sendMessage2(Comm::COORD, d);
}

void SimRobot::sendObjective(double x, double y, double theta)
{
	theta = Tools::angleInMinusPiPlusPi(theta);
	Comm::Data d;
    d.add((qint16)x).add((qint16)y).add((qint16)(theta * Comm::ANGLE_FACTOR));
	_protocol->sendMessage2(Comm::OBJECTIVE, d);
}

void SimRobot::sendAvoidingSensorsValues(const QList<quint8>& sharps)
{
    Comm::Data d;

    foreach(const quint8 v, sharps)
        d.add(v);

    _protocol->sendMessage2(Comm::AVOIDING_SENSORS, d);
}

void SimRobot::go(bool mirrored)
{
    Comm::Data d;
    d.add(mirrored);

    _protocol->sendMessage2(Comm::GO, d);
}

void SimRobot::sendNoticeOfReceipt(quint8 instruction, bool result)
{
    Comm::Data d;
    d.add(instruction).add(result);

    _protocol->sendMessage2(Comm::AR, d);
}

void SimRobot::sendArrived()
{
	sendEvent(Comm::EVENT_IS_ARRIVED);
}

void SimRobot::sendBlocked()
{
	sendEvent(Comm::EVENT_IS_BLOCKED);
}

void SimRobot::sendEvent(Comm::RobotEvent event)
{
	Comm::Data d;
	d.add((quint8)event);

	_protocol->sendMessage2(Comm::EVENT, d);
}

void SimRobot::sendSensorEvent(Comm::SensorType type, int sensorId, int value)
{
	Comm::Data d;
	d.add((quint8)type).add((quint8)sensorId).add((quint8)value);

	_protocol->sendMessage2(Comm::SENSOR_EVENT, d);
}

void SimRobot::restart()
{
	sendEvent(Comm::EVENT_RESTART);
}

void SimRobot::sendLog(const QByteArray &log)
{
	Comm::Data d;
	foreach(char c, log)
	{
		quint8 v = c;
		d.add(v);
	}

	_protocol->sendMessage2(Comm::LOG, d);
}

void SimRobot::sendParameters(const QList<float> &values, const QList<QByteArray> &names)
{
	Comm::Data dValues;
	dValues.add((quint8)values.count());
	foreach(float v, values)
		dValues.add(v);

	Comm::Data dNames;
	dNames.add((quint8)names.count());
	QByteArray nameStr;
	foreach(const QByteArray& n, names)
	{
		if (!nameStr.isEmpty())
			nameStr.append(";;");

		nameStr.append(n);
	}
	dNames.append(nameStr);

	_protocol->sendMessage2(Comm::PARAMETERS, dValues);
	_protocol->sendMessage2(Comm::PARAMETER_NAMES, dNames);
}

SimCommInterface::SimCommInterface()
{
}

