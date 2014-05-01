#include "ActionFactory.h"
#include "StrategyManager.h"

ActionFactory::ActionFactory(StrategyManager* manager, TrajectoryFinder* finder, StrategyMap* map, 
							 Comm::RobotCommInterface* robot, Tools::Ax12MovementManager* ax12MovementsManager)
{
    _manager = manager;
    _finder = finder;
    _map = map;
    _robot = robot;
	_ax12MovementsManager = ax12MovementsManager;
	_maxMovementSpeed = 100;
}

void ActionFactory::setMaxMovementSpeed(int maxSpeed)
{
	_maxMovementSpeed = maxSpeed;
}

int ActionFactory::getRealSpeed(int requestedSpeed) const
{
	return qBound(0, requestedSpeed, _maxMovementSpeed);
}

AbstractAction * ActionFactory::waitAction(int ms) const
{
    return new WaitAction(ms);
}

AbstractAction * ActionFactory::moveAction(Tools::NGridNode* destination, int speed, bool forceForward, bool forceBackward, Tools::Deplacement deplacementType) const
{
    return new MoveAction(destination, getRealSpeed(speed), forceForward, forceBackward, deplacementType, _finder);
}

AbstractAction *ActionFactory::moveAction(const QPointF &destinationPoint, int speed, bool forceForward, bool forceBackward, Tools::Deplacement deplacementType) const
{
    return new MoveAction(_manager->getGrid()->getNearestNode(destinationPoint), getRealSpeed(speed), forceForward, forceBackward, deplacementType, _finder);
}

AbstractAction *ActionFactory::moveAction(const QString &destinationAlias, int speed, bool forceForward, bool forceBackward, Tools::Deplacement deplacementType) const
{
    Tools::NGridNode* destination = _manager->getGrid()->getNode(destinationAlias);
    if (destination)
        return new MoveAction(destination, getRealSpeed(speed), forceForward, forceBackward, deplacementType, _finder);

    return 0;
}

AbstractAction *ActionFactory::moveAction(Tools::NGridArea *destination, int speed, bool forceForward, bool forceBackward, Tools::Deplacement deplacementType) const
{
    return new MoveAction(destination, getRealSpeed(speed), forceForward, forceBackward, deplacementType, _finder);
}

AbstractAction *ActionFactory::moveAction(Tools::NGridArea *destinationArea, int speed, Tools::NGridNode *targetNode, bool forceForward, bool forceBackward, Tools::Deplacement deplacementType) const
{
    return new MoveAction(destinationArea, targetNode, getRealSpeed(speed), forceForward, forceBackward, deplacementType, _finder);
}

AbstractAction * ActionFactory::manualMoveAction(const Tools::Trajectory &trajectory, int speed, Tools::Movement movement, Tools::Deplacement deplacementType) const
{
    return new ManualMoveAction(trajectory, getRealSpeed(speed), movement, deplacementType, _finder);
}

AbstractAction* ActionFactory::manualMoveToPointAction(const Tools::RPoint& point, int speed, bool forward, Tools::Deplacement deplacementType) const
{
    Tools::Trajectory t;
    t << Tools::RPoint(point);
    return new ManualMoveAction(t, getRealSpeed(speed), forward ? Tools::AVANT_XY : Tools::ARRIERE_XY, deplacementType, _finder);
}

AbstractAction * ActionFactory::manualBackwardMoveAction(double mm, int speed) const
{
    return new RelativeMoveAction(-mm, getRealSpeed(speed), _finder, _map);
}

AbstractAction * ActionFactory::manualForwardMoveAction(double mm, int speed) const
{
    return new RelativeMoveAction(mm, getRealSpeed(speed), _finder, _map);
}

AbstractAction * ActionFactory::manualTurnMoveAction(double radian, int speed) const
{
    Tools::Trajectory t;
    t << Tools::RPoint(0.0, 0.0, radian);
    return new ManualMoveAction(t, getRealSpeed(speed), Tools::TOURNE_RADIAN, Tools::TURN_THEN_MOVE, _finder);
}

AbstractAction * ActionFactory::manualTurnToAction(const Tools::RPoint& point, int speed, bool clockwize) const
{
    Tools::Trajectory t;
    t << Tools::RPoint(point);
    return new ManualMoveAction(t, getRealSpeed(speed), clockwize ? Tools::TOURNE_VERS_XY : Tools::TOURNE_VERS_XY, Tools::TURN_THEN_MOVE, _finder);
}

AbstractAction * ActionFactory::manualAbsoluteTurnMoveAction(double radian, int speed) const
{
    Tools::Trajectory t;
    t << Tools::RPoint(0.0, 0.0, radian);
    return new ManualMoveAction(t, getRealSpeed(speed), Tools::ROTATE_TO_ABSOLUTE_ANGLE, Tools::TURN_THEN_MOVE, _finder);
}

AbstractAction * ActionFactory::actuatorAction(Comm::RobotAction action, int parameter, int estimatedDurationMs) const
{
	return new ActuatorAction(action, parameter, estimatedDurationMs, _robot);
}

ActionGroup * ActionFactory::actionList(const QList<AbstractAction *> &actions) const
{
	return new ActionGroup(actions);
}

AsynchroneActionGroup *ActionFactory::asynchroneActionList(const QList<AbstractAction *> &actions, AsynchroneActionGroup::StopCondition stopCondition) const
{
	return new AsynchroneActionGroup(actions, stopCondition);
}

OrientationSwitchCaseAction *ActionFactory::orientationSwitchCaseAction() const
{
	return new OrientationSwitchCaseAction(_manager);
}

SensorSwitchCaseAction *ActionFactory::colorSensorSwitchCaseAction(int colorSensorId) const
{
	return new SensorSwitchCaseAction(colorSensorId, Comm::ColorSensor, _manager);
}

SensorSwitchCaseAction *ActionFactory::sharpSwitchCaseAction(int sharpId) const
{
	return new SensorSwitchCaseAction(sharpId, Comm::SharpSensor, _manager);
}

SensorSwitchCaseAction* ActionFactory::microswitchSwitchCaseAction(int microswicthId) const
{
	return new SensorSwitchCaseAction(microswicthId, Comm::MicroswitchSensor, _manager);
}

PositionSwitchCaseAction *ActionFactory::positionSwitchCaseAction() const
{
	return new PositionSwitchCaseAction(_manager);
}

AbstractAction *ActionFactory::ax12Action(quint8 id, float angle, float speed) const
{
    QList<quint8> idList;
    idList << id;
    QList<float> angleList;
    angleList << angle;
    QList<float> speedList;
    speedList << speed;

    return new AX12Action(idList, angleList, speedList, _robot);
}

AbstractAction *ActionFactory::ax12Action(quint8 id1, float angle1, quint8 id2, float angle2, float speed) const
{
    QList<quint8> idList;
    idList << id1 << id2;
    QList<float> angleList;
    angleList << angle1 << angle2;
    QList<float> speedList;
    speedList << speed << speed;

    return new AX12Action(idList, angleList, speedList, _robot);
}

AbstractAction* ActionFactory::ax12Movement(const QString& group, const QString& movement, float speedLimit) const
{
	return new AX12MovementAction(group, movement, speedLimit, _robot, _ax12MovementsManager);
}

AbstractAction *ActionFactory::waitForSharpToActivate(int otherSharpId, int timeoutInMs) const
{
	return new WaitUntilSensorAction(otherSharpId, Comm::SharpSensor, timeoutInMs, {Comm::SharpObjectDetected}, _manager);
}

AbstractAction *ActionFactory::waitForSharpToDesactivate(int otherSharpId, int timeoutInMs) const
{
	return new WaitUntilSensorAction(otherSharpId, Comm::SharpSensor, timeoutInMs, {Comm::SharpNothingDetected}, _manager);
}

AbstractAction* ActionFactory::waitForColor(int colorSensorId, int color, int timeoutInMs) const
{
	return new WaitUntilSensorAction(colorSensorId, Comm::ColorSensor, timeoutInMs, {color}, _manager);
}
	
AbstractAction* ActionFactory::waitForMicroswitchToActivate(int microswitchId, int timeoutInMs) const
{
	return new WaitUntilSensorAction(microswitchId, Comm::MicroswitchSensor, timeoutInMs, {Comm::MicroswicthOn}, _manager);
}

AbstractAction* ActionFactory::waitForMicroswitchToDesactivate(int microswitchId, int timeoutInMs) const
{
	return new WaitUntilSensorAction(microswitchId, Comm::MicroswitchSensor, timeoutInMs, {Comm::MicroswicthOff}, _manager);
}

ColorScanAction *ActionFactory::colorScanAction(Tools::NGridNode *destination, int speed, int timeoutMs, int ourColor, int opponentColor, int leftColorSensorId, int rightColorSensorId,
																		  AbstractAction *startAction, AbstractAction *leftOpponentColorAction, AbstractAction *rightOpponentColorAction,
																		  AbstractAction *leftOurColorAction, AbstractAction *rightOurColorAction, AbstractAction *endAction)
{
	return new ColorScanAction(destination, speed, timeoutMs, ourColor, opponentColor, leftColorSensorId, rightColorSensorId, startAction,
													 leftOpponentColorAction, rightOpponentColorAction, leftOurColorAction, rightOurColorAction, endAction,
													 _manager, _finder);
}

AbstractAction *ActionFactory::startPumpAction(int id) const
{
	return actuatorAction(Comm::ACTION_START_PUMP, id, 3000);
}

AbstractAction *ActionFactory::stopPumpAction(int id) const
{
	return actuatorAction(Comm::ACTION_STOP_PUMP, id, 3000);
}

AbstractAction *ActionFactory::enableColorSensorAction(int colorSensorId) const
{
	return new SetSensorEnabledAction(Comm::ColorSensor, colorSensorId, true, _robot);
}

AbstractAction *ActionFactory::disableColorSensorAction(int colorSensorId) const
{
	return new SetSensorEnabledAction(Comm::ColorSensor, colorSensorId, false, _robot);
}

AbstractAction *ActionFactory::enableSharpAction(int sharpId) const
{
	return new SetSensorEnabledAction(Comm::SharpSensor, sharpId, true, _robot);
}

AbstractAction *ActionFactory::disableSharpAction(int sharpId) const
{
	return new SetSensorEnabledAction(Comm::SharpSensor, sharpId, false, _robot);
}

AbstractAction *ActionFactory::enableMicroswitchAction(int microswitchId) const
{
	return new SetSensorEnabledAction(Comm::MicroswitchSensor, microswitchId, true, _robot);
}

AbstractAction *ActionFactory::disableMicroswitchAction(int microswitchId) const
{
	return new SetSensorEnabledAction(Comm::MicroswitchSensor, microswitchId, false, _robot);
}
