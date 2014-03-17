#include "AX12CommManager.h"
#include <math.h>

#include <QtGlobal>

using namespace Comm;

const float MAX_SPEED = 114.;
const float MAX_TORQUE = 100.;
const float MAX_ANGLE = 300;

AX12::AX12() :
	_CWAngleLimit(0), _CCWAngleLimit(300), _currentPosition(0), _currentSpeed(0), _currentLoad(0), _angleDiff(0), _goalChanged(false)
{
	setTimeout();
}

AX12::AX12(float minAngle, float maxAngle) :
	_CWAngleLimit(minAngle), _CCWAngleLimit(maxAngle), _currentPosition(0), _currentSpeed(0), _currentLoad(0), _angleDiff(0), _goalChanged(false)
{
	setTimeout();
}

void AX12::setGoal(float angle, float torque)
{
	if (angle > _CCWAngleLimit)
		angle = _CCWAngleLimit;
	else if (angle < _CWAngleLimit)
		angle = _CWAngleLimit;

	_goalPosition = angle;
	_angleDiff = fabs(_goalPosition - _currentPosition);
	_goalSpeed = 0;
	_maxTorque = torque;
	_goalChanged = true;
}

void AX12::setSpeed(float speed)
{
	_goalSpeed = speed;
}

float AX12::getLoad() const
{
	return _currentLoad;
}

float AX12::getPosition() const
{
	return _currentPosition;
}

float AX12::getSpeed() const
{
	return _currentSpeed;
}

void AX12::lockServo()
{
	_goalPosition = _currentPosition;
	_maxTorque = 100;
}

void AX12::releaseServo()
{
	_goalPosition = _currentPosition;
	_maxTorque = 0;
}

bool AX12::isTimeout() const
{
	return _currentPosition < 0;
}

void AX12::setTimeout()
{
	_currentPosition = -1;
}

void AX12::setState(float position, float speed, float load)
{
	_currentPosition = position;
	_currentSpeed = speed;
	_currentLoad = load;
	_angleDiff = fabs(_goalPosition - _currentPosition);
}

void AX12::getCommand(float &position, float &speed, float &torque) const
{
	position = _goalPosition;
	speed = _goalSpeed;
	torque = _maxTorque;
}

float AX12::getAngleDifference() const
{
	return _angleDiff;
}

bool AX12::goalHasChanged() const
{
	return _goalChanged;
}

bool AX12::isStopped() const
{
	return (_goalChanged && _goalSpeed == 0);
}

void AX12::notifyCommandSent()
{
	_goalChanged = false;
}

//--------------------------------------------------------

AX12CommManager::AX12CommManager()
	: _controllerMode(USB2AX_CONTROLLER), _protocol(0), _readingLoopMode(AUTO_MODE), _noControllerloopedRequestSent(0), _isBusy(false)
{
}

AX12CommManager::AX12CommManager(const QString &portname, qint32 baudrate, ControllerMode mode, Tools::AbstractLogger *logger) :
	Tools::LoggerInterface(logger), _controllerMode(mode), _autoReadingLoop(false), _readingLoopMode(AUTO_MODE), _noControllerloopedRequestSent(0), _isBusy(false)
{
	QSerialPort* port = new QSerialPort(portname);
	connect(port, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleSerialError(QSerialPort::SerialPortError)));
	_baudrate = baudrate;

	_protocol = new ProtocolAX12(port, logger, true);

	_protocol->beQuiet(true);
	_protocol->setDebugMode(true, false, false);

	_readTimer = new QTimer(this);
	connect(_readTimer, SIGNAL(timeout()), this, SLOT(requestAllServoStatusForReadingLoop()));

	_requestTimeoutTimer = new QTimer(this);
	_requestTimeoutTimer->setSingleShot(this);
	_requestTimeoutTimer->setInterval(3); //+2 with USB
	connect(_requestTimeoutTimer, SIGNAL(timeout()), this, SLOT(requestTimeout()));

	connect(this, SIGNAL(requestTimeoutReceived(QList<quint8>)), this, SLOT(sendNextMessage()));
}

AX12CommManager::~AX12CommManager()
{
	if (_protocol)
	{
		_messagePendingList.clear();
		_readTimer->stop();
		_requestTimeoutTimer->stop();

		_protocol->close();
		delete _protocol->getIODevice();
	}

	delete _protocol;
}

bool AX12CommManager::open()
{
	bool ok = _protocol->open();

	if (ok)
	{
		if (!connect(_protocol, SIGNAL(message(quint8, const Comm::Data&, quint8)), this, SLOT(messageReceived(quint8, const Comm::Data&, quint8))))
			logger() << "The connection to the slot messageReceived() failed !" << Tools::endl;

		if (ok)
		{
			QSerialPort* port = static_cast<QSerialPort*>(_protocol->getIODevice());
			port->setBaudRate(_baudrate);
			port->setFlowControl(QSerialPort::NoFlowControl);
			port->setParity(QSerialPort::NoParity);
			port->setDataBits(QSerialPort::Data8);
			port->setStopBits(QSerialPort::OneStop);
		}
	}
	else
	{
		logger() << "Warning: AX12 port cannot be opened. AX12 management is disabled." << Tools::endl;
	}

	return ok;
}

bool AX12CommManager::isOpened() const
{
	return _protocol && _protocol->isOpened();
}

void AX12CommManager::askStartReadingLoop()
{
	if (!isOpened())
		return;

	switch(_readingLoopMode)
	{
		case TIMER_MODE:
			_readTimer->start();
			break;
		case AUTO_MODE:
			_autoReadingLoop = true;
			break;
	}

	requestAllServoStatusForReadingLoop();
}

void AX12CommManager::askStopReadingLoop()
{
	_autoReadingLoop = false;
	_readTimer->stop();
}

void AX12CommManager::setReadingLoopMode(AX12CommManager::ReadingLoopMode mode)
{
	_readingLoopMode = mode;
}

void AX12CommManager::setTimerReadingLoopInterval(int ms)
{
	_readTimer->setInterval(ms);
}

bool AX12CommManager::isReadingLoopStarted() const
{
	return _autoReadingLoop || _readTimer->isActive();
}

void AX12CommManager::setRequestTimeout(int ms)
{
	_requestTimeoutTimer->setInterval(ms);
}

ProtocolAX12* AX12CommManager::getProtocol() const
{
	return _protocol;
}

void AX12CommManager::setControllerMode(AX12CommManager::ControllerMode mode)
{
	_controllerMode = mode;
}

AX12CommManager::ControllerMode AX12CommManager::getControllerMode() const
{
	return _controllerMode;
}

void AX12CommManager::resetServo(quint8 id, float minAngle, float maxAngle)
{
	_servos.insert(id, AX12(minAngle, maxAngle));
}

bool AX12CommManager::isServoTimeout(quint8 id) const
{
	if (_servos.contains(id))
		return _servos[id].isTimeout();
	else
		return false;
}

float AX12CommManager::getServoPosition(quint8 id) const
{
	if (_servos.contains(id))
		return _servos[id].getPosition();
	else
		return 0;
}

float AX12CommManager::getServoLoad(quint8 id) const
{
	if (_servos.contains(id))
		return _servos[id].getLoad();
	else
		return 0;
}

const AX12 AX12CommManager::getAX12(quint8 id) const
{
	if (_servos.contains(id))
		return _servos[id];

	return AX12();
}

bool Comm::AX12CommManager::contains(quint8 id) const
{
	return _servos.contains(id);
}

QList<quint8> AX12CommManager::getAllServos() const
{
	return _servos.keys();
}

void AX12CommManager::lockServo(quint8 id, bool synchronous)
{
	// lock the servo in its current position
	if (_servos.contains(id))
	{
		_servos[id].lockServo();
		if (synchronous)
			synchronize(id);
	}
}

void AX12CommManager::releaseServo(quint8 id, bool synchronous)
{
	if (_servos.contains(id))
	{
		_servos[id].releaseServo();
		if (synchronous)
			synchronize(id);
	}
}

void AX12CommManager::messageReceived(quint8 instruction, const Comm::Data& data, quint8 id)
{
	Data d(data);
	if (instruction == 0 && id == 0xFD && _controllerMode == USB2AX_CONTROLLER)
	{
		_requestTimeoutTimer->stop();
		foreach(quint8 id, _currentMessage.ids)
		{
			readReceivedData(id, d);
		}

		QTimer::singleShot(1, this, SLOT(sendNextMessage()));
		emit servosStatusUpdated(_currentMessage.ids);
	}

	else if (instruction == 0)
	{
		// ignore messages sent to AX-12 (if RX & TX are connected together)
		//standard reading
		_requestTimeoutTimer->stop();
		readReceivedData(id, d);

		QTimer::singleShot(1, this, SLOT(sendNextMessage()));

		QList<quint8> ids;
		ids << id;
		emit servosStatusUpdated(ids);
	}
}

void AX12CommManager::readReceivedData(quint8 id, Comm::Data& data)
{
	// extract position, speed and load from data

	quint16 pos, speed, load;
	data.take(pos).take(speed).take(load);

	float currentPos, currentSpeed, currentLoad;
	currentPos = (float)pos * 300. / 1023.;

	if (speed > 1023)
		speed -= 1024;
	currentSpeed = (float)speed * 114. / 1023.;

	if (load > 1023)
		load -= 1024;
	currentLoad = (float)load * 100. / 2047.;

	_servos[id].setState(currentPos, currentSpeed, currentLoad);
}

void AX12CommManager::setGoal(quint8 servoId, float angle, float speed, float torque, bool synchronous)
{
	AX12& servo = _servos[servoId];

	servo.setGoal(angle, torque);
	servo.setSpeed(speed);

	if (synchronous)
		synchronize(servoId);
}

double AX12CommManager::calculateSmoothServosSpeed(const QList<quint8>& servoIds, float maxSpeed)
{
	maxSpeed = qBound(0.f, maxSpeed, 114.f);

	float maxAngleDiff = 0;
	foreach(quint8 id, servoIds)
	{
		AX12& servo = _servos[id];
		if (servo.getPosition() >= 0 && servo.isStopped())	// ignore if current position hasn't been received yet
		{
			float angleDiff = servo.getAngleDifference();

			if (angleDiff > maxAngleDiff)
				maxAngleDiff = angleDiff;
		}
	}

	float movementTime = (maxAngleDiff > 0 ? maxAngleDiff : 1) / maxSpeed;
	double movementTimeMs = (movementTime * 60. * 1000.) / 360.;

	foreach(quint8 id, servoIds)
	{
		AX12& servo = _servos[id];
		if (servo.isStopped())
			servo.setSpeed(servo.getAngleDifference() / movementTime);
	}

	return movementTimeMs;
}

void AX12CommManager::addServoCommandData(AX12& ax12, Data& data)
{
	float position, speed, torque;
	ax12.getCommand(position, speed, torque);

	quint16 a = (quint16) position * 1023. / MAX_ANGLE;
	quint16 s = (quint16) speed * 1023. / MAX_SPEED;
	quint16 t = (quint16) torque * 1023. / MAX_TORQUE;

	data.add(a);
	data.add(s);
	data.add(t);
}

void AX12CommManager::sendServoMultiSynchronizeMessage(const QList<quint8>& ids)
{
	if (!_protocol || !_protocol->isOpened())
		return;

	// Note: cannot send more than 143 bytes at once!

	Data data(Comm::LittleEndian);
	data.add((quint8)0x1e);	// AX-12 memory address to write to
	data.add((quint8)6);	// write 6 bytes per servo

	int i=0;
	foreach(quint8 id, ids)
	{
		AX12& servo = _servos[id];

		if (i > 9)		// send data if reach max length in one frame
		{
			_protocol->sendMessage(0xfe, ProtocolAX12::SYNC_WRITE, data);
			data.clear();
			data.add((quint8)0x1e);
			data.add((quint8)6);

			i = 0;
		}

		data.add(id);
		addServoCommandData(servo, data);

		servo.notifyCommandSent();
	}

	_protocol->sendMessage(0xfe, ProtocolAX12::SYNC_WRITE, data);
}

void AX12CommManager::sendServoSynchronizeMessage(quint8 id)
{
	if (!_protocol || !_protocol->isOpened())
		return;

	AX12& servo = _servos[id];
	float position, speed, torque;
	servo.getCommand(position, speed, torque);

	Data data(Comm::LittleEndian);
	data.add((quint8)0x1e);	// AX-12 memory address to write to
	addServoCommandData(servo, data);

	_protocol->sendMessage(id, ProtocolAX12::WRITE_DATA, data);

	servo.notifyCommandSent();
}


void AX12CommManager::sendServoRequestStatusMessage(quint8 id)
{
	if (!_protocol || !_protocol->isOpened())
		return;

	Data data(Comm::LittleEndian);

	data.add((quint8)0x24);	// read from address 0x24
	data.add((quint8)6);	// read 6 bytes (current position, speed & load)

	_protocol->sendMessage(id, ProtocolAX12::READ_DATA, data);
	_requestTimeoutTimer->start();
}

void AX12CommManager::sendServoMultiRequestStatusMessage(const QList<quint8> &ids)
{
	if (!_protocol || !_protocol->isOpened())
		return;

	Q_ASSERT(_controllerMode == USB2AX_CONTROLLER);

	Data data(Comm::LittleEndian);

	quint8 commID = 0xFD;
	data.add((quint8)0x24);	// read from address 0x24
	data.add((quint8)6);	// read 6 bytes (current position, speed & load)

	foreach(quint8 id, ids)
		data.add(id);

	_protocol->sendMessage(commID, ProtocolAX12::SYNC_READ, data);
	_requestTimeoutTimer->start();
}

void AX12CommManager::requestServoStatus(quint8 id, bool loop)
{
	if (!contains(id))
		_servos.insert(id, AX12());

	if (loop)
	{
		_loopDemandCount[id]++;

		if (!isReadingLoopStarted())
			askStartReadingLoop();
	}
	else
	{
		CommMessage msg(id, CommMessage::StatusRequest);
		_messagePendingList << msg;
		askSendNextRequestMessage();
	}
}

void AX12CommManager::requestServoStatus(const QList<quint8> &ids, bool loop)
{
	if (loop)
	{
		foreach(quint8 id, ids)
		{
			if (!contains(id))
				_servos.insert(id, AX12());
			_loopDemandCount[id]++;
		}

		if (!isReadingLoopStarted())
			askStartReadingLoop();
	}
	else
	{
		CommMessage msg(ids, CommMessage::StatusRequest);
		_messagePendingList << msg;
		askSendNextRequestMessage();
	}
}

void AX12CommManager::requestAllServoStatusForReadingLoop()
{
	if (_noControllerloopedRequestSent > 0 && _controllerMode == NO_CONTROLLER)
	{
		logger() << "Loop timer is too fast, a cycle has been skipped." << Tools::endl;
		return;
	}

	QList<quint8> ids;
	for(QHash<quint8, int>::const_iterator it = _loopDemandCount.constBegin(); it != _loopDemandCount.constEnd(); ++it)
	{
		quint8 id = it.key();
		int nb = *it;

		if (nb > 0)
		{
			if (_controllerMode == NO_CONTROLLER)
			{
				CommMessage msg(id, CommMessage::LoopedStatusRequest);
				_messagePendingList << msg;
			}

			ids << id;
		}
	}

	if (_controllerMode == USB2AX_CONTROLLER)
	{
		CommMessage msg(ids, CommMessage::LoopedStatusRequest);
		_messagePendingList << msg;
	}
	else
	{
		_noControllerloopedRequestSent = ids.count();
	}

	askSendNextRequestMessage();
}

void AX12CommManager::cancelLoopStatusRequest(quint8 id)
{
	_loopDemandCount[id]--;
	if (_loopDemandCount[id] <= 0)
		_loopDemandCount.remove(id);

	if (_loopDemandCount.isEmpty())
		askStopReadingLoop();
}

void AX12CommManager::cancelLoopStatusRequest(const QList<quint8> &ids)
{
	foreach(quint8 id, ids)
		cancelLoopStatusRequest(id);
}

void AX12CommManager::synchronize(quint8 id)
{
	CommMessage msg(id, CommMessage::Synchronization);
	_messagePendingList << msg;
	askSendNextSynchroMessage();
}

void AX12CommManager::synchronize(const QList<quint8>& servoIds)
{
	CommMessage msg(servoIds, CommMessage::Synchronization);
	_messagePendingList << msg;
	askSendNextSynchroMessage();
}

void AX12CommManager::askSendNextRequestMessage()
{
	if (!_isBusy)
		sendNextMessage();
}

void AX12CommManager::askSendNextSynchroMessage()
{
	if (_controllerMode != NO_CONTROLLER || !_isBusy)
		sendNextMessage();
}

void AX12CommManager::sendNextMessage()
{
	if (!_messagePendingList.isEmpty())
	{
		bool isLoopMessage = false;
		_isBusy = true;
		_currentMessage = _messagePendingList.takeFirst();
		if (!_currentMessage.ids.isEmpty())
		{
			switch(_currentMessage.type)
			{
				case CommMessage::LoopedStatusRequest:
					isLoopMessage = true; //fall through
				case CommMessage::StatusRequest:
				{
					if (_controllerMode == NO_CONTROLLER)
					{
						quint8 id = _currentMessage.ids.first();
						sendServoRequestStatusMessage(id);
						if (isLoopMessage)
						{
							--_noControllerloopedRequestSent;
							if (_autoReadingLoop && _noControllerloopedRequestSent <= 0)
								requestAllServoStatusForReadingLoop();
						}
					}
					else
					{
						sendServoMultiRequestStatusMessage(_currentMessage.ids);
						if (isLoopMessage && _autoReadingLoop)
							requestAllServoStatusForReadingLoop();
					}

					break;
				}
				case CommMessage::Synchronization:
				{
					if (_currentMessage.ids.count() > 1)
					{
						double estimatedTime = 0;
						if (_controllerMode == NO_CONTROLLER)
							estimatedTime = 1 + 0.7 * _currentMessage.ids.count() + 2;

						sendServoMultiSynchronizeMessage(_currentMessage.ids);
						QTimer::singleShot(1*estimatedTime, this, SLOT(sendNextMessage()));
					}
					else
					{
						sendServoSynchronizeMessage(_currentMessage.ids.first());
						QTimer::singleShot(2, this, SLOT(sendNextMessage())); //Estimated time to send an instruction
					}
					break;
				}
				case CommMessage::None:
					break;
			}
		}
		else
		{
			sendNextMessage();
		}
	}
	else
	{
		_isBusy = false;
		emit allMessagesSent();
	}

	if (_messagePendingList.count() > _servos.count() * 3)
	{
		//warning
		logger() << "Warning: Possible overflow of AX-12 message pending list.\n";
	}
}

void AX12CommManager::requestTimeout()
{
	foreach(quint8 id, _currentMessage.ids)
	{
		AX12& ax12 = _servos[id];
		ax12.setTimeout();
	}

	emit requestTimeoutReceived(_currentMessage.ids);
}

void AX12CommManager::handleSerialError(QSerialPort::SerialPortError error)
{
	switch(error)
	{
		case QSerialPort::ResourceError:
			_protocol->close();
			break;
		case QSerialPort::NoError:
			break;
		default:
			logger() << "Warning: Serial Error: " << _protocol->getIODevice()->errorString() << Tools::endl;
	}
}
