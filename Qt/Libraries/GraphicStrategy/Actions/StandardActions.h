#ifndef STANDARDACTIONS_H
#define STANDARDACTIONS_H

#include "AbstractAction.h"
#include "Sensor.h"

#include <RPoint.h>
#include <Curve.h>
#include <NGrid.h>
#include <Instructions.h>
#include <RMovement.h>

#include <QTimer>

class TrajectoryFinder;
class StrategyMap;

namespace Comm
{
	class RobotCommInterface;
	class Ax12MovementRunner;
}

namespace Tools
{
	class Ax12MovementManager;
}


class WaitAction : public AbstractAction
{
public:
	WaitAction(int ms, QObject* parent = 0);

	void execute();
	void stop();

	QString getActionName() const;
private:
	int _ms;
	QTimer* _timer;
};

class TeleportAction  : public AbstractAction
{
public:
	TeleportAction(const Tools::RPoint& point, Comm::RobotCommInterface* robot, QObject* parent = 0);

	void execute();
	QString getActionName() const;

private:
	Tools::RPoint _point;
	Comm::RobotCommInterface* _robot;
};

class MoveAction : public AbstractAction
{
public:
	MoveAction(Tools::NGridNode* destination, int speed, bool forceForward, bool forceBackward, Tools::Deplacement deplacementType, TrajectoryFinder* finder, QObject* parent = 0);
	MoveAction(Tools::NGridArea* destination, int speed, bool forceForward, bool forceBackward, Tools::Deplacement deplacementType, TrajectoryFinder* finder, QObject* parent = 0);
    MoveAction(Tools::NGridArea* destination, Tools::NGridNode* targetNode, int speed, bool forceForward, bool forceBackward, Tools::Deplacement deplacementType, TrajectoryFinder* finder, QObject* parent = 0);

	void execute();
	void end();
	void stop();

	QString getActionName() const;

private:
	TrajectoryFinder*	_finder;
	Tools::NGridNode*	_destination;
	Tools::NGridArea*   _destinationArea;
	bool		_forceForward;
	bool		_forceBackward;
	Tools::Deplacement _deplacementType;
	int _speed;
};

class ManualMoveAction : public AbstractAction
{
public:
	ManualMoveAction(const Tools::Trajectory& trajectory, int speed, Tools::Movement movement, Tools::Deplacement deplacementType, TrajectoryFinder* finder, QObject* parent = 0);

	void execute();
	void end();
	void stop();

	QString getActionName() const;

private:
	TrajectoryFinder*	_finder;
	Tools::Trajectory	_trajectory;
	Tools::Movement	_movement;
	Tools::Deplacement _deplacementType;
	int _speed;
};

class RelativeMoveAction : public AbstractAction
{
public:
	RelativeMoveAction(double distance, int speed, TrajectoryFinder* finder, StrategyMap* map, QObject* parent = 0);

	void execute();
	void end();
	void stop();

	QString getActionName() const;

private:
	TrajectoryFinder*	_finder;
	StrategyMap*        _map;
	double              _distance;
	int _speed;
};

class RotationAction : public AbstractAction
{
public:
	RotationAction(double absoluteAngle, int speed, TrajectoryFinder* finder, StrategyMap* map, QObject* parent = 0);

	void execute();
	void end();
	void stop();

	QString getActionName() const;

private:
	TrajectoryFinder*	_finder;
	StrategyMap*        _map;
	double              _angle;
	int _speed;
};

class ActuatorAction : public AbstractAction
{
public:
	ActuatorAction(Comm::RobotAction action, int parameter, int estimatedDurationMs, Comm::RobotCommInterface* robot, QObject* parent = 0);

	void execute();
	QString getActionName() const;

private:
	Comm::RobotAction _action;
	int _parameter;
	int _duration;
	Comm::RobotCommInterface*	_robot;
};

class AX12Action : public AbstractAction
{
public:
	AX12Action(const QList<quint8>& ids, const QList<float>& angles, const QList<float>& speeds, Comm::RobotCommInterface* robot, QObject* parent = 0);

	void execute();
	void end();
	QString getActionName() const;

private:
	QList<quint8> _idList;
	QList<float> _angleList;
	QList<float> _speedList;

	Comm::RobotCommInterface*	_robot;
};

class AX12MovementAction : public AbstractAction
{
	Q_OBJECT
public:
	AX12MovementAction(const QString& group, const QString& movement, float speedLimit, bool synchrone, Comm::RobotCommInterface* robot, Tools::Ax12MovementManager* movementManager, QObject* parent = 0);

	void execute();
	void end();
	void stop();

	QString getActionName() const;

private:
	QString _group;
	QString _mvt;
	float _speedLimit;
	bool _synchrone;

	Comm::RobotCommInterface*	_robot;
	Tools::Ax12MovementManager* _movementManager;
	Comm::Ax12MovementRunner* _runner;
};

class WaitUntilSensorAction : public AbstractAction
{
    Q_OBJECT
public:
	WaitUntilSensorAction(int sensorId, Comm::SensorType type, int timeoutMs, const QList<int>& states, StrategyManager* manager, QObject* parent = 0);

    void execute();
    void end();
    void stop();
    QString getActionName() const;

private:
   int _sensorId;
   Comm::SensorType _sensorType;

   QList<int> _states;

   StrategyManager*	_manager;
   QTimer* _timeout;

private slots:
	void testSensor(Comm::SensorType type, int sensorId, int state);
};

class SetParameterAction : public AbstractAction
{
	Q_OBJECT
public:
	SetParameterAction(int parameterId, double value, StrategyManager* manager, QObject* parent = 0);

	void execute();
	QString getActionName() const;

private:
	int _parameterId;
	double _value;

	StrategyManager* _manager;
};

class SetSensorEnabledAction : public AbstractAction
{
public:
	SetSensorEnabledAction(Comm::SensorType type, int sensorId, bool enabled, Comm::RobotCommInterface* robot, QObject* parent = 0);

	void execute();
	QString getActionName() const;

private:
	Comm::SensorType _type;
	int _id;
	bool _enabled;
	Comm::RobotCommInterface*	_robot;
};



#endif // STANDARDACTIONS_H
