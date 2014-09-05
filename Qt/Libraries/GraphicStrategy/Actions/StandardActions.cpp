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

//Teleport-----------------------------------------------------------------------

TeleportAction::TeleportAction(const Tools::RPoint &point, Comm::RobotCommInterface *robot, QObject* parent)
	: AbstractAction(parent), _point(point), _robot(robot)
{
}

void TeleportAction::execute()
{
	_robot->setPosition(_point.getX(), _point.getY(), _point.getTheta());
	succeed();
}

QString TeleportAction::getActionName() const
{
	return QString("Teleport to ").append(QString::number(_point.getX())).append(",")
			.append(QString::number(_point.getY())).append(")")
			.append(QString::number(_point.getThetaDegree()))
			.append("°");
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
	bool ok = connect(_finder, SIGNAL(objectiveReached()), this, SLOT(succeed()));
	ok = connect(_finder, SIGNAL(objectiveCanceled()), this, SLOT(failed())) && ok;
	if (!ok)
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
	bool ok = connect(_finder, SIGNAL(objectiveReached()), this, SLOT(succeed()));
	ok = connect(_finder, SIGNAL(objectiveCanceled()), this, SLOT(failed())) && ok;
	if (!ok)
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

//--------------------------------------------------------------------------------

RotationAction::RotationAction(double absoluteAngle, int speed, TrajectoryFinder* finder, StrategyMap* map, QObject* parent)
	: AbstractAction(parent), _finder(finder), _map(map), _angle(absoluteAngle), _speed(speed)
{
}

void RotationAction::execute()
{
	Tools::Trajectory t;

	t << Tools::RPoint(_map->getRobotPosition().getX() + 10.0 * cos(_angle),
					   _map->getRobotPosition().getY() + 10.0 * sin(_angle),
					   _angle);

	_finder->setManualTrajectory(t, _speed, Tools::AUTO, Tools::TURN_ONLY);
	bool ok = connect(_finder, SIGNAL(objectiveReached()), this, SLOT(succeed()));
	ok = connect(_finder, SIGNAL(objectiveCanceled()), this, SLOT(failed())) && ok;
	if (!ok)
		failed();
}

void RotationAction::end()
{
	disconnect();
}

void RotationAction::stop()
{
	_finder->cancel();
}

QString RotationAction::getActionName() const
{
	return QString("Rotation to ").append(QString::number(_angle)).append("rad");
}

//actuator--------------------------------------------------------------------------------

ActuatorAction::ActuatorAction(Comm::RobotAction action, int parameter, int estimatedDurationMs, Comm::RobotCommInterface* robot, QObject *parent)
	: AbstractAction(parent), _action(action), _parameter(parameter), _duration(estimatedDurationMs), _robot(robot)
{
}

void ActuatorAction::execute()
{
	_robot->executeAction(_action, _parameter);
	QTimer::singleShot(_duration, this, SLOT(succeed()));
}

QString ActuatorAction::getActionName() const
{
	return QString("Execute action ").append(QString::number(_action));
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

AX12MovementAction::AX12MovementAction(const QString& group, const QString& movement, float speedLimit, bool synchrone, Comm::RobotCommInterface* robot, Tools::Ax12MovementManager* movementManager, QObject *parent)
	: AbstractAction(parent), _group(group), _mvt(movement), _speedLimit(speedLimit), _synchrone(synchrone), _robot(robot), _movementManager(movementManager)
{
	_runner = new Comm::Ax12MovementRunner(_robot->getAx12Manager(), _movementManager, this);
}

void AX12MovementAction::execute()
{
	if (_synchrone)
		connect(_runner, SIGNAL(movementFinished(bool, QString, QString)), this, SIGNAL(finished(bool)));
		
	bool ok = _runner->startMovement(_group, _mvt, _speedLimit);
	if (!ok)
		failed();
		
	else if (!_synchrone)
		succeed();
}

void AX12MovementAction::end()
{
	disconnect();
}

void AX12MovementAction::stop()
{
	if (_synchrone)
		_runner->stop();
}

QString AX12MovementAction::getActionName() const
{
	return QString("AX-12 movement: ").append(_group).append("/").append(_mvt);
}

//Wait until sharp-----------------------------------------------------------------------


WaitUntilSensorAction::WaitUntilSensorAction(int sensorId, Comm::SensorType type, int timeoutMs, const QList<int>& states, StrategyManager *manager, QObject *parent)
    : AbstractAction(parent), _sensorId(sensorId), _sensorType(type), _states(states), _manager(manager), _timeout(0)
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
	connect(_manager, SIGNAL(sensorStateChanged(Comm::SensorType, int, int)), this, SLOT(testSensor(Comm::SensorType, int, int)));
    if (_timeout)
        _timeout->start();
}

void WaitUntilSensorAction::end()
{
    if (_timeout)
        _timeout->stop();

   disconnect(_manager, SIGNAL(sensorStateChanged(Comm::SensorType, int, int)), this, SLOT(testSensor(Comm::SensorType, int, int)));
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

void WaitUntilSensorAction::testSensor(Comm::SensorType type, int sensorId, int state)
{
	if (sensorId == _sensorId && type == _sensorType)
	{
		if (_states.contains(state))
			succeed();
	}
}

//Set parameter-----------------------------------------------------------------------

SetParameterAction::SetParameterAction(int parameterId, double value, StrategyManager *manager, QObject *parent)
	: AbstractAction(parent), _parameterId(parameterId), _value(value), _manager(manager)
{
}

void SetParameterAction::execute()
{
	_manager->setParameter(_parameterId, _value);
	succeed();
}

QString SetParameterAction::getActionName() const
{
	return QString("Set %1 to '%2'").arg(_manager->getParameterName(_parameterId)).arg(_value);
}

//Set sensor enabled-----------------------------------------------------------------------

SetSensorEnabledAction::SetSensorEnabledAction(Comm::SensorType type, int sensorId, bool enabled, Comm::RobotCommInterface *robot, QObject *parent)
	: AbstractAction(parent), _type(type), _id(sensorId), _enabled(enabled), _robot(robot)
{

}

void SetSensorEnabledAction::execute()
{
	if (_enabled)
		_robot->enableSensor(_type, _id);
	else
		_robot->disableSensor(_type, _id);
	succeed();
}

QString SetSensorEnabledAction::getActionName() const
{
	return QString("%1 sensor of type %2 with id %3").arg(_enabled ? "Enable" : "Disable").arg(_type).arg(_id);
}

