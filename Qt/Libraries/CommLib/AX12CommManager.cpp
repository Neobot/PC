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

AX12CommManager::AX12CommManager() : _protocol(0), _nbReadingLoopRequest(0), _readingLoopMode(AUTO_MODE), _isBusy(false)
{
}

AX12CommManager::AX12CommManager(const QString &portname, BaudRateType baudrate, Tools::AbstractLogger *logger) :
	Tools::LoggerInterface(logger), _autoReadingLoop(false), _nbReadingLoopRequest(0), _readingLoopMode(AUTO_MODE), _isBusy(false)
{
	QextSerialPort* port = new QextSerialPort(portname,  QextSerialPort::EventDriven);
	port->setBaudRate(baudrate);
	port->setFlowControl(FLOW_OFF);
	port->setParity(PAR_NONE);
	port->setDataBits(DATA_8);
	port->setStopBits(STOP_1);

	_protocol = new ProtocolAX12(port, logger, true);

	_protocol->beQuiet(true);
	_protocol->setDebugMode(true, false, false);

	_readTimer = new QTimer(this);
	connect(_readTimer, SIGNAL(timeout()), this, SLOT(requestAllServoStatus()));

	_requestTimeoutTimer = new QTimer(this);
	_requestTimeoutTimer->setSingleShot(this);
	_requestTimeoutTimer->setInterval(3); //+2 with USB
	connect(_requestTimeoutTimer, SIGNAL(timeout()), this, SLOT(requestTimeout()));

	connect(this, SIGNAL(requestTimeoutReceived(quint8)), this, SLOT(sendNextMessage()));
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

	if (!ok)
		logger() << "Warning: AX12 port cannot be opened. AX12 management is disabled." << Tools::endl;


	if (!connect(_protocol, SIGNAL(message(quint8, const Comm::Data&, quint8)), this, SLOT(messageReceived(quint8, const Comm::Data&, quint8))))
		logger() << "The connection to the slot messageReceived() failed !" << Tools::endl;

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

	if (_nbReadingLoopRequest == 0)
	{
		switch(_readingLoopMode)
		{
			case TIMER_MODE:
				_readTimer->start();
				break;
			case AUTO_MODE:
				_autoReadingLoop = true;
				break;
		}
		requestAllServoStatus();
	}

	++_nbReadingLoopRequest;
}

void AX12CommManager::askStopReadingLoop()
{
	--_nbReadingLoopRequest;

	if (_nbReadingLoopRequest == 0)
	{
		_messagePendingList.clear();
		_autoReadingLoop = false;
		_readTimer->stop();
	}
}

void AX12CommManager::forceStopReadingLoop()
{
	_nbReadingLoopRequest = 1;
	askStopReadingLoop();
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

void AX12CommManager::clearServoList()
{
	_servos.clear();
}

ProtocolAX12* AX12CommManager::getProtocol() const
{
	return _protocol;
}

AX12 *AX12CommManager::addServo(quint8 id, float minAngle, float maxAngle)
{
	_servos.insert(id, AX12(minAngle, maxAngle));

	if (_autoReadingLoop)
		requestServoStatus(id);

	return &_servos[id];
}

void AX12CommManager::removeServo(quint8 id)
{
	_servos.remove(id);
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
	// ignore messages sent to AX-12 (if RX & TX are connected together)

	if (instruction == 0 && _servos.contains(id))	// status instruction
	{
		_requestTimeoutTimer->stop();

		// extract position, speed and load from data
		Comm::Data d(data);
		quint16 pos, speed, load;
		d.take(pos).take(speed).take(load);

		float currentPos, currentSpeed, currentLoad;
		currentPos = (float)pos * 300. / 1023.;

		if (speed > 1023)
			speed -= 1024;
		currentSpeed = (float)speed * 114. / 1023.;

		if (load > 1023)
			load -= 1024;
		currentLoad = (float)load * 100. / 2047.;

		_servos[id].setState(currentPos, currentSpeed, currentLoad);

		QTimer::singleShot(1, this, SLOT(sendNextMessage()));
		emit servosStatusUpdated(id);
	}
}

void AX12CommManager::setGoal(quint8 servoId, float angle, float speed, float torque, bool synchronous)
{
	if (_servos.contains(servoId))
	{
		AX12& servo = _servos[servoId];

		servo.setGoal(angle, torque);
		servo.setSpeed(speed);

		if (synchronous)
			synchronize(servoId);
	}
}

double AX12CommManager::calculateSmoothServosSpeed(const QList<quint8>& servoIds, float maxSpeed)
{
	maxSpeed = qBound(0.f, maxSpeed, 114.f);
	
	float maxAngleDiff = 0;
	foreach(quint8 id, servoIds)
	{
		if (_servos.contains(id))
		{
			AX12& servo = _servos[id];
			if (servo.getPosition() >= 0 && servo.isStopped())	// ignore if current position hasn't been received yet
			{
				float angleDiff = servo.getAngleDifference();

				if (angleDiff > maxAngleDiff)
					maxAngleDiff = angleDiff;
			}
		}
	}

	float movementTime = (maxAngleDiff > 0 ? maxAngleDiff : 1) / maxSpeed;
	double movementTimeMs = (movementTime * 60. * 1000.) / 360.;
	
	foreach(quint8 id, servoIds)
	{
		if (_servos.contains(id))
		{
			AX12& servo = _servos[id];
			if (servo.isStopped())
				servo.setSpeed(servo.getAngleDifference() / movementTime);
		}
	}
	
	return movementTimeMs;
}

double AX12CommManager::calculateSmoothServosSpeed(float maxSpeed)
{
	return calculateSmoothServosSpeed(_servos.keys(), maxSpeed);
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
		if (_servos.contains(id))
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
	}

	_protocol->sendMessage(0xfe, ProtocolAX12::SYNC_WRITE, data);
}

void AX12CommManager::sendServoSynchronizeMessage(quint8 id)
{
	if (!_protocol || !_protocol->isOpened())
		return;

	if (_servos.contains(id))
	{
		AX12& servo = _servos[id];
		float position, speed, torque;
		servo.getCommand(position, speed, torque);

		Data data(Comm::LittleEndian);
		data.add((quint8)0x1e);	// AX-12 memory address to write to
		addServoCommandData(servo, data);

		_protocol->sendMessage(id, ProtocolAX12::WRITE_DATA, data);

		servo.notifyCommandSent();
	}
}


void AX12CommManager::sendServoRequestStatusMessage(quint8 id)
{
	if (!_protocol || !_protocol->isOpened())
		return;
		
	if (_servos.contains(id))
	{
		Data data(Comm::LittleEndian);

		data.add((quint8)0x24);	// read from address 0x24
		data.add((quint8)6);	// read 6 bytes (current position, speed & load)

		_protocol->sendMessage(id, ProtocolAX12::READ_DATA, data);
		_requestTimeoutTimer->start();
	}
}

void AX12CommManager::requestServoStatus(quint8 id)
{
	CommMessage msg(id, CommMessage::StatusRequest);
	_messagePendingList << msg;
	askSendNextMessage();
}

void AX12CommManager::requestAllServoStatus()
{
	QList<quint8> ids = _servos.keys();
	foreach(quint8 id, ids)
	{
		CommMessage msg(id, CommMessage::StatusRequest);
		_messagePendingList << msg; //StatusRequest message support only one servo
	}
	askSendNextMessage();
}

void AX12CommManager::synchronize(quint8 id)
{
	CommMessage msg(id, CommMessage::Synchronization);
	_messagePendingList << msg;
	askSendNextMessage();
}

void AX12CommManager::synchronize(const QList<quint8>& servoIds)
{
	CommMessage msg(servoIds, CommMessage::Synchronization);
	_messagePendingList << msg;
	askSendNextMessage();
}

void AX12CommManager::synchronize()
{
	synchronize(_servos.keys());
}

void AX12CommManager::askSendNextMessage()
{
	if (!_isBusy)
		sendNextMessage();
}

void AX12CommManager::sendNextMessage()
{
	if (!_messagePendingList.isEmpty())
	{
		_isBusy = true;
		_currentMessage = _messagePendingList.takeFirst();
		if (!_currentMessage.ids.isEmpty())
		{
			switch(_currentMessage.type)
			{
				case CommMessage::StatusRequest:
				{
					quint8 id = _currentMessage.ids.first();
					sendServoRequestStatusMessage(id);
					if (_autoReadingLoop && contains(id))
						requestServoStatus(id);
					break;
				}
				case CommMessage::Synchronization:
				{
					if (_currentMessage.ids.count() > 1)
					{
						double estimatedTime = 1 + 0.7 * _currentMessage.ids.count() + 2;
						sendServoMultiSynchronizeMessage(_currentMessage.ids);
						QTimer::singleShot(3*estimatedTime, this, SLOT(sendNextMessage()));
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
	if (!_currentMessage.ids.isEmpty())
    {
        int id = _currentMessage.ids.first();
        if (_servos.contains(id))
        {
            AX12& ax12 = _servos[id];
            ax12.setTimeout();
        }
        emit requestTimeoutReceived(id);
    }
}
