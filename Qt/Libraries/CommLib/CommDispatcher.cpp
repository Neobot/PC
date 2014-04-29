#include "CommDispatcher.h"

using namespace Comm;

CommDispatcher::CommDispatcher() : _main(0)
{
}


void Comm::CommDispatcher::setMainListener(Comm::CommListener *listener)
{
	_main = listener;
}

void Comm::CommDispatcher::registerResponder(Comm::CommListener *listener)
{
	_responders << listener;
}

void CommDispatcher::unregisterResponder(CommListener *listener)
{
	_responders.removeAll(listener);
}

void Comm::CommDispatcher::coordinates(qint16 x, qint16 y, double theta, quint8 forward)
{
	foreach(CommListener* responder, _responders)
		responder->coordinates(x, y, theta, forward);

	if (_main)
		_main->coordinates(x, y, theta, forward);
}

void Comm::CommDispatcher::objective(qint16 x, qint16 y, double theta)
{
	foreach(CommListener* responder, _responders)
		responder->objective(x, y, theta);

	if (_main)
		_main->objective(x, y, theta);
}

void Comm::CommDispatcher::avoidingSensors(const QList<quint8> &values)
{
	foreach(CommListener* responder, _responders)
		responder->avoidingSensors(values);

	if (_main)
		_main->avoidingSensors(values);
}

bool Comm::CommDispatcher::initDone()
{
	foreach(CommListener* responder, _responders)
		responder->initDone();

	if (_main)
		return _main->initDone();

	return true;
}

bool Comm::CommDispatcher::go(bool mirrored)
{
	foreach(CommListener* responder, _responders)
		responder->go(mirrored);

	if (_main)
		return _main->go(mirrored);

	return true;
}

bool Comm::CommDispatcher::pingReceived()
{
	foreach(CommListener* responder, _responders)
		responder->pingReceived();

	if (_main)
		return _main->pingReceived();

	return true;
}

void Comm::CommDispatcher::noticeOfReceipt(quint8 instruction, bool result)
{
	foreach(CommListener* responder, _responders)
		responder->noticeOfReceipt(instruction, result);

	if (_main)
		_main->noticeOfReceipt(instruction, result);
}

void Comm::CommDispatcher::opponentPosition(qint16 x, qint16 y)
{
	foreach(CommListener* responder, _responders)
		responder->opponentPosition(x, y);

	if (_main)
		_main->opponentPosition(x, y);
}

bool Comm::CommDispatcher::restart()
{
	foreach(CommListener* responder, _responders)
		responder->restart();

	if (_main)
		return _main->restart();

	return true;
}

void Comm::CommDispatcher::quit()
{
	foreach(CommListener* responder, _responders)
		responder->quit();

	if (_main)
		_main->quit();
}

void Comm::CommDispatcher::log(const QByteArray &text)
{
	foreach(CommListener* responder, _responders)
		responder->log(text);

	if (_main)
		_main->log(text);
}

void CommDispatcher::parameters(const QList<float> &values)
{
	foreach(CommListener* responder, _responders)
		responder->parameters(values);

	if (_main)
		_main->parameters(values);
}

void CommDispatcher::parameterNames(const QStringList &names)
{
	foreach(CommListener* responder, _responders)
		responder->parameterNames(names);

	if (_main)
		_main->parameterNames(names);
}

void CommDispatcher::event(RobotEvent event)
{
	foreach(CommListener* responder, _responders)
		responder->event(event);

	if (_main)
		_main->event(event);
}

void CommDispatcher::sensorEvent(SensorType type, int sensorId, int value)
{
	foreach(CommListener* responder, _responders)
		responder->sensorEvent(type, sensorId, value);

	if (_main)
		_main->sensorEvent(type, sensorId, value);
}