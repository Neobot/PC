#include "StandardActions.h"

#include "TrajectoryFinder.h"
#include "StrategyManager.h"
#include "Ax12MovementRunner.h"
#include "ToolsLib.h"
#include "Sharp.h"

#include <QTimer>
#include <QtDebug>
#include <cmath>

WaitAction::WaitAction(int ms, QObject *parent) : AbstractAction(parent), _ms(ms)
{
	_timer = new QTimer(parent);
	_timer->setSingleShot(true);
	connect(_timer, SIGNAL(timeout()), this, SLOT(succeed()));
}

void WaitAction::execute()
{
	_timer->start(_ms);
}

void WaitAction::stop()
{
	_timer->stop();
	failed();
}

QString WaitAction::getActionName() const
{
	return QString("Waiting ").append(QString::number(_ms)).append(" ms");
}

//--------------------------------------------------------------------------------

MoveAction::MoveAction(Tools::NGridNode* destination, int speed, bool forceForward, bool forceBackward, Tools::Deplacement deplacementType, TrajectoryFinder *finder, QObject *parent)
	: AbstractAction(parent), _finder(finder), _destination(destination), _destinationArea(0), 
	_forceForward(forceForward), _forceBackward(forceBackward), _deplacementType(deplacementType), _speed(speed)
{
}

MoveAction::MoveAction(Tools::NGridArea *destination, int speed, bool forceForward, bool forceBackward, Tools::Deplacement deplacementType, TrajectoryFinder *finder, QObject *parent)
    : AbstractAction(parent), _finder(finder), _destination(0), _destinationArea(destination),
      _forceForward(forceForward), _forceBackward(forceBackward), _deplacementType(deplacementType), _speed(speed)
{
}

MoveAction::MoveAction(Tools::NGridArea* destination, Tools::NGridNode* targetNode, int speed, bool forceForward, bool forceBackward, Tools::Deplacement deplacementType, TrajectoryFinder* finder, QObject* parent)
    : AbstractAction(parent), _finder(finder), _destination(targetNode), _destinationArea(destination),
      _forceForward(forceForward), _forceBackward(forceBackward), _deplacementType(deplacementType), _speed(speed)
{
}

void MoveAction::execute()
{
	Tools::Direction direction = Tools::Unknown;
	if (_forceForward)
		direction = Tools::Forward;
	else if (_forceBackward)
		direction = Tools::Backward;

    _finder->findTrajectory(_destinationArea, _destination, _speed, direction, _deplacementType);
	bool ok = connect(_finder, SIGNAL(objectiveReached()), this, SLOT(succeed()));
	ok = connect(_finder, SIGNAL(objectiveCanceled()), this, SLOT(failed())) && ok;
	if (!ok)
		failed();
}

void MoveAction::end()
{
	disconnect();
}

void MoveAction::stop()
{
	_finder->cancel();
}

QString MoveAction::getActionName() const
{
	return QString("Moving to Node(").append(QString::number(_destination->getPosition().x())).append(",").append(QString::number(_destination->getPosition().y())).append(")");
}


//--------------------------------------------------------------------------------

ManualMoveAction::ManualMoveAction(const Tools::Trajectory& trajectory, int speed, Tools::Movement movement, Tools::Deplacement deplacementType, TrajectoryFinder *finder, QObject *parent)
	: AbstractAction(parent), _finder(finder), _trajectory(trajectory), _movement(movement), _deplacementType(deplacementType), _speed(speed)
{
}

void ManualMoveAction::execute()
{
	_finder->setManualTrajectory(_trajectory, _speed, _movement, _deplacementType);
	if (!connect(_finder, SIGNAL(objectiveReached()), this, SLOT(succeed())))
		failed();
}

void ManualMoveAction::end()
{
	disconnect();
}

void ManualMoveAction::stop()
{
	_finder->cancel();
}

QString ManualMoveAction::getActionName() const
{
	return QString("Manual action of type ").append(QString::number(_movement));
}

//--------------------------------------------------------------------------------

RelativeMoveAction::RelativeMoveAction(double distance, int speed, TrajectoryFinder* finder, StrategyMap* map, QObject* parent)
	: AbstractAction(parent), _finder(finder), _map(map), _distance(distance), _speed(speed)
{
}

void RelativeMoveAction::execute()
{
	Tools::Trajectory t;

	t << Tools::RPoint(_map->getRobotPosition().getX() + _distance * cos(_map->getRobotPosition().getTheta()),
					   _map->getRobotPosition().getY() + _distance * sin(_map->getRobotPosition().getTheta()),
					   _map->getRobotPosition().getTheta());

	_finder->setManualTrajectory(t, _speed, _distance > 0 ? Tools::AVANT_XY : Tools::ARRIERE_XY);
	if (!connect(_finder, SIGNAL(objectiveReached()), this, SLOT(succeed())))
		failed();
}

void RelativeMoveAction::end()
{
	disconnect();
}

void RelativeMoveAction::stop()
{
	_finder->cancel();
}

QString RelativeMoveAction::getActionName() const
{
	return QString("Relative move of ").append(QString::number(_distance)).append("mm");
}

//actuator--------------------------------------------------------------------------------

ActuatorAction::ActuatorAction(Comm::ServoId servoId, Comm::ServoPosition position, int estimatedDuration, Comm::RobotCommInterface* robot, QObject *parent)
    : AbstractAction(parent), _id(servoId), _position(position), _duration(estimatedDuration), _robot(robot)
{
}

void ActuatorAction::execute()
{
	_robot->moveServo(_id, _position);
	QTimer::singleShot(_duration, this, SLOT(succeed()));
}

QString ActuatorAction::getActionName() const
{
	return QString("Move servo ").append(QString::number(_id).append(" in position ").append(_position));
}

//AX-12--------------------------------------------------------------------------------

AX12Action::AX12Action(const QList<quint8>& ids, const QList<float>& angles, const QList<float>& speeds, Comm::RobotCommInterface* robot, QObject *parent)
	: AbstractAction(parent), _idList(ids), _angleList(angles), _speedList(speeds), _robot(robot)
{
}

void AX12Action::execute()
{
	int count = _idList.count();
	for(int i = 0; i < count; ++i)
	{
		quint8 id = _idList.value(i);
		float angle = _angleList.value(i, 300);
		float speed = _speedList.value(i, 0);
		_robot->moveAX12(id, angle, speed);
	}

	succeed();
}

void AX12Action::end()
{
}

QString AX12Action::getActionName() const
{
	return "Multiple AX-12 action";
}

//AX-12 movement-----------------------------------------------------------------------

AX12MovementAction::AX12MovementAction(const QString& group, const QString& movement, float speedLimit, Comm::RobotCommInterface* robot, Tools::Ax12MovementManager* movementManager, QObject *parent)
	: AbstractAction(parent), _group(group), _mvt(movement), _speedLimit(speedLimit), _robot(robot), _movementManager(movementManager)
{
	_runner = new Comm::Ax12MovementRunner(_robot->getAx12Manager(), _movementManager, this);
}

void AX12MovementAction::execute()
{
	connect(_runner, SIGNAL(movementFinished(bool, QString, QString)), this, SIGNAL(finished(bool)));
	bool ok = _runner->startMovement(_group, _mvt, _speedLimit);
	if (!ok)
		failed();
}

void AX12MovementAction::end()
{
	disconnect();
}

void AX12MovementAction::stop()
{
	_runner->stop();
}

QString AX12MovementAction::getActionName() const
{
	return QString("AX-12 movement: ").append(_group).append("/").append(_mvt);
}

//Wait until sharp-----------------------------------------------------------------------


WaitUntilSensorAction::WaitUntilSensorAction(const Sensor *sensor, Sensor::SensorFamily family, int timeoutMs, double threshold, int thresholdOperations, StrategyManager *manager, QObject *parent)
    : AbstractAction(parent), _sensor(sensor), _family(family), _threshold(threshold), _thresholdOperations(thresholdOperations), _manager(manager), _timeout(0)
{
    if (timeoutMs > 0)
    {
        _timeout = new QTimer(this);
        _timeout->setSingleShot(true);
        _timeout->setInterval(timeoutMs);
        connect(_timeout, SIGNAL(timeout()), this, SLOT(failed()));
    }
}

void WaitUntilSensorAction::execute()
{
    connect(_manager, SIGNAL(otherSensorsReceived()), this, SLOT(testSharp()));
    if (_timeout)
        _timeout->start();
}

void WaitUntilSensorAction::end()
{
    if (_timeout)
        _timeout->stop();

    disconnect();
}

void WaitUntilSensorAction::stop()
{
	if (_timeout)
		_timeout->stop();

	failed();
}

QString WaitUntilSensorAction::getActionName() const
{
    return QString("Scanning sensor");
}

void WaitUntilSensorAction::testSensor(Sensor::SensorFamily family)
{
    if (_family == family)
    {
        if (_sensor->testThreshold(_threshold, _thresholdOperations))
            succeed();
    }
}
