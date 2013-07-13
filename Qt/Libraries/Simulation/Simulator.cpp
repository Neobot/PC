#include "Simulator.h"
#include <QtDebug>
#include <QVector2D>
#include <math.h>
#include "vmath.h"
#include "Instructions.h"

using namespace Tools;

Simulator::RSubMovement::RSubMovement(const Tools::RPoint& destination, SubMovement type, int speed)
    : _destination(destination), _type(type), _speed(speed) {}

Simulator::RSubMovement::RSubMovement(const RSubMovement& M)
    : _destination(M._destination), _type(M._type), _speed(M._speed) {}

const Tools::RPoint& Simulator::RSubMovement::getDestination() const
{
	return _destination;
}

SubMovement Simulator::RSubMovement::getType() const
{
    return _type;
}

int Simulator::RSubMovement::getSpeed() const
{
    return _speed;
}

Simulator::Simulator(SimProtocol* protocol, int interval, double speed, AbstractLogger* logger)
	: LoggerInterface(logger), _defaultRobotPos(-1000, -1000), _defaultOpponentPos(-1000, -1000), _isBlocked(false), _moveForward(true)
{
	connect(&_timer, SIGNAL(timeout()), this, SLOT(simulationTimerEvent()));
	_timer.setInterval(interval);

	_interval = interval;
	_color = Red;

	setSpeed(speed);

	_simRobot = new SimRobot(protocol, this);
	_map = new SimulationMap();
	initSimulation();
}

Simulator::~Simulator()
{
	delete _map;
	delete _simRobot;
}

void Simulator::setPosition(qint16 x, qint16 y, double theta)
{
	_map->moveRobot(RPoint(x, y, theta));
	logger() << "Simulator: Init received "  << x << "," << y << "," << theta << Tools::endl;
	_simRobot->sendLog("Teleportation!!!!");
}

void Simulator::flush()
{
	_fifo.clear();
    _subMovements.clear();
	_destination = RMovement();
	_simRobot->sendLog("<crissement de frein>");
}

void Simulator::askParameters()
{
	QList<float> params;
	QList<QByteArray> names;

	params << getSpeed() << 42.2 << 33.222029;
	names << "Simulated robot speed" << "Test 1" << "Nombre au pif";

	_simRobot->sendParameters(params, names);
}

void Simulator::setParameters(const QList<float> &values)
{
	if (values.count() > 0)
	{
		setSpeed(values[0]);
		_simRobot->sendLog("Changement de vitesse !!!");
	}
}

void Simulator::addDestination(qint16 x, qint16 y, double theta, quint8 type, bool isStopPoint, int speed)
{
	if (_isBlocked)
		_isBlocked = false;

    _fifo.enqueue(RMovement(RPoint(x, y, theta), type, Tools::TURN_THEN_MOVE, isStopPoint, speed));
}

void Simulator::executeAction(int actionName, quint16 parameter)
{
	if (_isBlocked)
		_isBlocked = false;

	logger() << "Simulator: Executing action" << Tools::endl;
	_currentAction = actionName;
	_currentSubActionDuration = 0;
	_subActionList.clear();

	switch (_currentAction)
	{
		default:
			;
	}
}

void Simulator::simulationTimerEvent()
{
	if (_subMovements.isEmpty())
		prepareNextMovement();

	if (!_subMovements.isEmpty())
	{
		updateRobotPosition();

		if (_subMovements.empty())
		{
			_simRobot->sendArrived();
		}
	}
	else if(_currentAction != Comm::NO_ACTION)
		processAction();

	RPoint robot = _map->getRobotPosition();
	_simRobot->sendCoordinates(robot.getX(), robot.getY(), robot.getTheta(), _moveForward);
	_simRobot->sendObjective(_currentObjectve.getX(), _currentObjectve.getY(), _currentObjectve.getTheta());

	QList<quint8> avoidingSharpsValues;
	QList<DataObject *> avoidingSharpList = _map->getObjects(SimulationMap::AvoidingSharpGroup);
	for (QList<DataObject *>::iterator object = avoidingSharpList.begin(); object != avoidingSharpList.end(); ++object)
	{
		quint8 v = (quint8)(static_cast<SharpObject*>(*object))->getValue();
		avoidingSharpsValues.append(v);
	}
	_simRobot->sendAvoidingSensorsValues(avoidingSharpsValues);

	QList<quint8> otherSharpsValues;
	QList<DataObject *> otherSharpList = _map->getObjects(SimulationMap::AvoidingSharpGroup);
	for (QList<DataObject *>::iterator object = otherSharpList.begin(); object != otherSharpList.end(); ++object)
	{
		quint8 v = (quint8)(static_cast<SharpObject*>(*object))->getValue();
		otherSharpsValues.append(v);
	}
	_simRobot->sendOtherSensorsValues(otherSharpsValues);

	_clawsSharp = (_map->getSharpInClaw()->getDistance() < 30);


	quint8 contactorsValues = 0;
	QList<DataObject *> contactorList = _map->getObjects(SimulationMap::ContactorGroup);

	for (int i = 0, n = 1; i < 8 && !contactorList.empty(); i++)
	{
		contactorsValues += static_cast<ContactorObject*>(contactorList.takeFirst())->getStatus() * n;
		n *= 2;
	}

	_simRobot->sendContactorsValues(contactorsValues);

	if (!_opponentMovements.empty())
		updateOpponentPosition();

	_map->updateSensors();
}

void Simulator::updateRobotPosition()
{
	if (_isBlocked)
		return;

	RPoint destination(_subMovements.first().getDestination());
	RPoint currentPos(_map->getRobotPosition());
	SubMovement type(_subMovements.first().getType());

	double diffAngle = currentPos.diffAngle(RPoint(0,0,currentPos.angle(destination)));
	QVector2D u(QPointF(destination.getX() - currentPos.getX(), destination.getY() - currentPos.getY()));
	u.normalize();

	_currentObjectve = destination;
	_currentObjectve.setTheta(currentPos.angle(destination));

    double speed = qBound(10, _subMovements.first().getSpeed(), 100);
    double r = (_r * speed) / 100.0;
    double d = (_d * speed) / 100.0;


	switch(type)
	{

		case MOVEMENT_ROTATE_FORWARD:
            if (diffAngle > r)
                currentPos.rotateBy(-r);
            else if (diffAngle < -r)
                currentPos.rotateBy(r);
			else
			{
				currentPos.setTheta(currentPos.angle(destination));
				_subMovements.dequeue();
			}
			break;

		case MOVEMENT_ROTATE_BACKWARD:
			diffAngle = angleInMinusPiPlusPi(diffAngle + pi);

            if (diffAngle > r)
                currentPos.rotateBy(-r);
            else if (diffAngle < -r)
                currentPos.rotateBy(r);
			else
			{
				currentPos.setTheta(angleInMinusPiPlusPi(currentPos.angle(destination) + pi));
				_subMovements.dequeue();
			}
			break;

		case MOVEMENT_MOVE_FORWARD:
            if (currentPos.isInCircle(destination, d))
			{
				currentPos.setX(destination.getX());
				currentPos.setY(destination.getY());
				_subMovements.dequeue();
			}
			else
			{
				currentPos.setTheta(currentPos.angle(destination));
                currentPos += RPoint(d * u.x(), d * u.y());
			}
			_moveForward = true;
			break;

		case MOVEMENT_MOVE_BACKWARD:
            if (currentPos.isInCircle(destination, d))
			{
				currentPos.setX(destination.getX());
				currentPos.setY(destination.getY());
				_subMovements.dequeue();
			}
			else
			{
				currentPos.setTheta(angleInMinusPiPlusPi(currentPos.angle(destination) + pi));
                currentPos += RPoint(d * u.x(), d * u.y());
			}
			_moveForward = false;
			break;

		default:

			;
	}

	_map->moveRobot(currentPos);
}

void Simulator::updateOpponentPosition()
{
	QPointF destination = _opponentMovements.first();
	RPoint currentPos(_map->getOpponentPosition());
	QVector2D u(QPoint(destination.x() - currentPos.getX(), destination.y() - currentPos.getY()));
	u.normalize();

	if (currentPos.isInCircle(Point(destination.x(), destination.y()), _d))
	{
		currentPos.setX(destination.x());
		currentPos.setY(destination.y());
		_opponentMovements.dequeue();
	}
	else
	{
		currentPos += RPoint(_d * u.x(), _d * u.y());
	}

	_map->moveOpponent(currentPos);
}

void Simulator::processAction()
{
	if (!_subActionList.isEmpty())
	{
		SubAction currentSubAction = _subActionList.first();

		if (currentSubAction != NO_SUBACTION)
		{
			// timed sub-action

			_currentSubActionDuration++;

			if (_currentSubActionDuration >= _subActionDuration[currentSubAction])
			{
				// sub-action finished
				// ...


				// go to next sub-action
				_subActionList.removeFirst();
				_currentSubActionDuration = 0;
			}
		}
		else if (currentSubAction == MOVING)
		{
			// move until the sharp detect an element in the claws

			RPoint currentPos(_map->getRobotPosition());
			QVector2D u(QPointF(cos(currentPos.getTheta()), sin(currentPos.getTheta())));
			u.normalize();

			if (_actionDistanceLeft == 0 || _clawsSharp == true)
			{
				_subActionList.removeFirst();
				_currentSubActionDuration = 0;
			}
			else
			{
				if (_actionDistanceLeft > _d)
				{
					currentPos += RPoint(_d * u.x(), _d * u.y());
					_actionDistanceLeft -= _d;
				}
				else if (_actionDistanceLeft > 0)
				{
					currentPos += RPoint(_actionDistanceLeft * u.x(), _actionDistanceLeft * u.y());
					_actionDistanceLeft = 0;
				}
			}

			_map->moveRobot(currentPos);
		}
	}

	if (_subActionList.isEmpty())
	{
		// action finished
		_currentSubActionDuration = 0;
		_currentAction = Comm::NO_ACTION;

		_simRobot->sendNoticeOfReceipt(Comm::ACTIONS, true);
	}
}

void Simulator::prepareNextMovement()
{
	if (!_fifo.isEmpty())
	{
		_destination = _fifo.dequeue();

		int movementType = _destination.getType();
		if (movementType == AUTO)
		{
			RPoint pos = _map->getRobotPosition();
			double angle = Tools::angleInMinusPiPlusPi(pos.angle(_destination.getDestination()) - pos.getTheta());
			if (angle <= pi/2 || angle > -pi/2)
				movementType = AVANT_XY;
			else
				movementType = ARRIERE_XY;
		}

		switch(movementType)
		{
			case AVANT_XY:
                _subMovements.enqueue(RSubMovement(_destination.getDestination(), MOVEMENT_ROTATE_FORWARD, _destination.getSpeed()));
                _subMovements.enqueue(RSubMovement(_destination.getDestination(), MOVEMENT_MOVE_FORWARD, _destination.getSpeed()));

				break;

			case ARRIERE_XY:
                _subMovements.enqueue(RSubMovement(_destination.getDestination(), MOVEMENT_ROTATE_BACKWARD, _destination.getSpeed()));
                _subMovements.enqueue(RSubMovement(_destination.getDestination(), MOVEMENT_MOVE_BACKWARD, _destination.getSpeed()));

				break;

			case TOURNE_VERS_XY:
                _subMovements.enqueue(RSubMovement(_destination.getDestination(), MOVEMENT_ROTATE_FORWARD, _destination.getSpeed()));

				break;

			case TOURNE_VERS_XY_AR:
                _subMovements.enqueue(RSubMovement(_destination.getDestination(), MOVEMENT_ROTATE_BACKWARD, _destination.getSpeed()));

				break;

			case TOURNE_RADIAN:
			{
				// convert the movement to an absolute movement by calculating destination coordinates
				double angle = _destination.getDestination().getTheta() - _map->getRobotPosition().getTheta();
				RPoint destinationCoord(_map->getRobotPosition().getX() - cos(angle),
										_map->getRobotPosition().getY() - sin(angle),
										_map->getRobotPosition().getTheta());

                _subMovements.enqueue(RSubMovement(destinationCoord, MOVEMENT_ROTATE_FORWARD, _destination.getSpeed()));
			}

				break;

			case AVANCE_MM:
			{
				// convert the movement to an absolute movement by calculating destination coordinates
				double distance = _destination.getDestination().getX();
				RPoint destinationCoord(_map->getRobotPosition().getX() + distance * cos(_map->getRobotPosition().getTheta()),
										_map->getRobotPosition().getY() + distance * sin(_map->getRobotPosition().getTheta()),
										_map->getRobotPosition().getTheta());

				if (distance > 0)
                    _subMovements.enqueue(RSubMovement(destinationCoord, MOVEMENT_MOVE_FORWARD, _destination.getSpeed()));
				else
                    _subMovements.enqueue(RSubMovement(destinationCoord, MOVEMENT_MOVE_BACKWARD, _destination.getSpeed()));
			}

				break;

			case ROTATE_TO_ABSOLUTE_ANGLE:
			{
				// convert the movement to an absolute movement by calculating destination coordinates
				double angle = _destination.getDestination().getTheta();
				RPoint destinationCoord(_map->getRobotPosition().getX() + cos(angle),
										_map->getRobotPosition().getY() + sin(angle),
										_map->getRobotPosition().getTheta());

                _subMovements.enqueue(RSubMovement(destinationCoord, MOVEMENT_ROTATE_FORWARD, _destination.getSpeed()));
			}

				break;

			default:

				;

		}
	}
	else
		_destination = RMovement();
}

void Simulator::initSimulation()
{
	_isAtBeginning = true;

	_fifo.clear();
	_subMovements.clear();

	_currentAction = Comm::NO_ACTION;
	_subActionList.clear();
	_currentSubActionDuration = 0;
	_actionDistanceLeft = 0;

	_clawsSharp = false;

	_opponentMovements.clear();

	//_map->moveOpponent(_defaultOpponentPos);
	_map->moveRobot(_defaultRobotPos);
	_map->updateSensors();
}

void Simulator::start()
{
	_timer.start();

	if (_isAtBeginning)
	{
		_simRobot->go(_color == Red);
		_simRobot->sendLog("I'm alive, alive!");

		_isAtBeginning = false;
	}
}

void Simulator::pause()
{
	_timer.stop();
}

void Simulator::stop()
{
	_timer.stop();
	initSimulation();
	_simRobot->restart();
}

void Simulator::block()
{
	_isBlocked = true;
	_simRobot->sendBlocked();
}

void Simulator::setConfiguration(const ChessUpConfiguration &config)
{
	if (_isAtBeginning) //Cannot change the config during a run
		_map->setConfiguration(config);
	//else
	//	config.deleteConfigurationObjects();
}

RPoint Simulator::getDefaultRobotPosition()
{
	return _defaultRobotPos;
}


double Simulator::getSpeed() const
{
	return _d / _interval;
}

void Simulator::setSpeed(double speed)
{
	_d = speed * _interval;
	_r = _d / 200;
}

SimulationMap * Simulator::getMap() const
{
	return _map;
}

void Simulator::setRobotDefaultColor(PlayableColor color)
{
	_color = color;

	_map->moveRobot(_defaultRobotPos);
	_map->moveOpponent(_defaultOpponentPos);
}

void Simulator::setRobotDefaultPosition(bool mirrored)
{
	setRobotDefaultColor(mirrored ? Red : Blue);
}

void Simulator::addOpponentMovement(const QPointF &point)
{
	_opponentMovements << point;
}
