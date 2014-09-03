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
	_isPatherEnabled = true;
}

void ActionFactory::setMaxMovementSpeed(int maxSpeed)
{
	_maxMovementSpeed = maxSpeed;
}

void ActionFactory::setPatherEnabled(bool value)
{
	_isPatherEnabled = value;
}

int ActionFactory::getRealSpeed(int requestedSpeed) const
{
	return qBound(0, requestedSpeed, _maxMovementSpeed);
}

AbstractAction * ActionFactory::waitAction(int ms) const
{
	return new WaitAction(ms);
}

AbstractAction *ActionFactory::teleportAction(const Tools::RPoint &point) const
{
	return new TeleportAction(point, _robot);
}

AbstractAction * ActionFactory::moveAction(Tools::NGridNode* destination, int speed, bool forceForward, bool forceBackward, Tools::Deplacement deplacementType) const
{
	if (_isPatherEnabled)
		return new MoveAction(destination, getRealSpeed(speed), forceForward, forceBackward, deplacementType, _finder);
	else
		return manualMoveToPointAction(destination->getPosition(), speed, forceForward, forceBackward, deplacementType);
}

AbstractAction *ActionFactory::moveAction(const QPointF &destinationPoint, int speed, bool forceForward, bool forceBackward, Tools::Deplacement deplacementType) const
{
	if (_isPatherEnabled)
		return new MoveAction(_manager->getGrid()->getNearestNode(destinationPoint), getRealSpeed(speed), forceForward, forceBackward, deplacementType, _finder);
	else
		return manualMoveToPointAction(destinationPoint, speed, forceForward, forceBackward, deplacementType);
}

AbstractAction *ActionFactory::moveAction(const QString &destinationAlias, int speed, bool forceForward, bool forceBackward, Tools::Deplacement deplacementType) const
{
    Tools::NGridNode* destination = _manager->getGrid()->getNode(destinationAlias);
    if (destination)
		return moveAction(destination, getRealSpeed(speed), forceForward, forceBackward, deplacementType);

    return 0;
}

AbstractAction *ActionFactory::moveAction(Tools::NGridArea *destination, int speed, bool forceForward, bool forceBackward, Tools::Deplacement deplacementType) const
{
	if (_isPatherEnabled)
		return new MoveAction(destination, getRealSpeed(speed), forceForward, forceBackward, deplacementType, _finder);
	else
		return manualMoveToPointAction(destination->getCentralNode()->getPosition(), speed, forceForward, forceBackward, deplacementType);
}

AbstractAction *ActionFactory::moveAction(Tools::NGridArea *destinationArea, int speed, Tools::NGridNode *targetNode, bool forceForward, bool forceBackward, Tools::Deplacement deplacementType) const
{
	if (_isPatherEnabled)
		return new MoveAction(destinationArea, targetNode, getRealSpeed(speed), forceForward, forceBackward, deplacementType, _finder);
	else
		return manualMoveToPointAction(targetNode->getPosition(), speed, forceForward, forceBackward, deplacementType);
}

AbstractAction * ActionFactory::manualMoveAction(const Tools::Trajectory &trajectory, int speed, Tools::Movement movement, Tools::Deplacement deplacementType) const
{
    return new ManualMoveAction(trajectory, getRealSpeed(speed), movement, deplacementType, _finder);
}

AbstractAction* ActionFactory::manualMoveToPointAction(const Tools::RPoint& point, int speed, bool forceForward, bool forceBackward, Tools::Deplacement deplacementType) const
{
    Tools::Trajectory t;
    t << Tools::RPoint(point);

	Tools::Movement mvtType = Tools::AUTO;
	if (forceForward)
		mvtType = Tools::AVANT_XY;
	else if (forceBackward)
		mvtType = Tools::ARRIERE_XY;

	return new ManualMoveAction(t, getRealSpeed(speed), mvtType, deplacementType, _finder);
}

AbstractAction * ActionFactory::manualBackwardMoveAction(double mm, int speed) const
{
    return new RelativeMoveAction(-mm, getRealSpeed(speed), _finder, _map);
}

AbstractAction * ActionFactory::manualForwardMoveAction(double mm, int speed) const
{
    return new RelativeMoveAction(mm, getRealSpeed(speed), _finder, _map);
}

AbstractAction * ActionFactory::manualTurnToAction(const Tools::RPoint& point, int speed, bool clockwize) const
{
    Tools::Trajectory t;
    t << Tools::RPoint(point);
	return new ManualMoveAction(t, getRealSpeed(speed), clockwize ? Tools::AVANT_XY : Tools::ARRIERE_XY, Tools::TURN_ONLY, _finder);
}

AbstractAction * ActionFactory::manualAbsoluteTurnMoveAction(double radian, int speed) const
{
	return new RotationAction(radian, getRealSpeed(speed), _finder, _map);
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

AbstractAction *ActionFactory::ifOrientationAction(double minAngle, double maxAngle, AbstractAction *thenAction, AbstractAction *elseAction)
{
	OrientationSwitchCaseAction* switchAction = orientationSwitchCaseAction();
	switchAction->addCase(qMakePair(minAngle, maxAngle), thenAction);
	switchAction->setDefaultAction(elseAction);
	return switchAction;
}

OrientationSwitchCaseAction *ActionFactory::orientationSwitchCaseAction() const
{
	return new OrientationSwitchCaseAction(_manager);
}

AbstractAction *ActionFactory::ifColorSensorAction(int colorSensorId, int value, AbstractAction *thenAction, AbstractAction *elseAction)
{
	SensorSwitchCaseAction* switchAction = colorSensorSwitchCaseAction(colorSensorId);
	switchAction->addCase(value, thenAction);
	switchAction->setDefaultAction(elseAction);
	return switchAction;
}

SensorSwitchCaseAction *ActionFactory::colorSensorSwitchCaseAction(int colorSensorId) const
{
	return new SensorSwitchCaseAction(colorSensorId, Comm::ColorSensor, _manager);
}

AbstractAction *ActionFactory::ifSharpAction(int sharpId, int value, AbstractAction *thenAction, AbstractAction *elseAction)
{
	SensorSwitchCaseAction* switchAction = sharpSwitchCaseAction(sharpId);
	switchAction->addCase(value, thenAction);
	switchAction->setDefaultAction(elseAction);
	return switchAction;
}

SensorSwitchCaseAction *ActionFactory::sharpSwitchCaseAction(int sharpId) const
{
	return new SensorSwitchCaseAction(sharpId, Comm::SharpSensor, _manager);
}

AbstractAction *ActionFactory::ifMicroswitchAction(int microswicthId, int value, AbstractAction *thenAction, AbstractAction *elseAction)
{
	SensorSwitchCaseAction* switchAction = microswitchSwitchCaseAction(microswicthId);
	switchAction->addCase(value, thenAction);
	switchAction->setDefaultAction(elseAction);
	return switchAction;
}

SensorSwitchCaseAction* ActionFactory::microswitchSwitchCaseAction(int microswicthId) const
{
	return new SensorSwitchCaseAction(microswicthId, Comm::MicroswitchSensor, _manager);
}

AbstractAction *ActionFactory::ifPositionAction(const QRectF &rect, AbstractAction *thenAction, AbstractAction *elseAction)
{
	PositionSwitchCaseAction* switchAction = positionSwitchCaseAction();
	switchAction->addCase(rect, thenAction);
	switchAction->setDefaultAction(elseAction);
	return switchAction;
}

PositionSwitchCaseAction *ActionFactory::positionSwitchCaseAction() const
{
	return new PositionSwitchCaseAction(_manager);
}

AbstractAction *ActionFactory::ifOpponentAction(const QRectF &rect, AbstractAction *thenAction, AbstractAction *elseAction)
{
	OpponentSwitchCaseAction* switchAction = opponentSwitchCaseAction();
	switchAction->addCase(rect, thenAction);
	switchAction->setDefaultAction(elseAction);
	return switchAction;
}

OpponentSwitchCaseAction *ActionFactory::opponentSwitchCaseAction() const
{
	return new OpponentSwitchCaseAction(_manager);
}

AbstractAction *ActionFactory::ifStrategyReversedAction(AbstractAction *thenAction, AbstractAction *elseAction)
{
	StrategyReversedSwitchCaseAction* switchAction = new StrategyReversedSwitchCaseAction(_manager);
	switchAction->addCase(true, thenAction);
	switchAction->setDefaultAction(elseAction);
	return switchAction;
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
	return new AX12MovementAction(group, movement, speedLimit, true, _robot, _ax12MovementsManager);
}

AbstractAction* ActionFactory::ax12AsynchroneMovement(const QString& group, const QString& movement, float speedLimit) const
{
	return new AX12MovementAction(group, movement, speedLimit, false, _robot, _ax12MovementsManager);
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

AbstractAction *ActionFactory::morseLetterColor(int colorSensorId, int nbSignal, int duration) const
{
	QList<AbstractAction*> list;
	for(int i = 0; i < nbSignal; ++i)
	{
		list	<< enableColorSensorAction(colorSensorId)
				<< waitAction(duration)
				<< disableColorSensorAction(colorSensorId);

		if (i != nbSignal - 1)
			list << waitAction(duration);
	}

	return actionList(list);
}

AbstractAction *ActionFactory::sosColor(int colorSensorId) const
{
	return actionList({
						  morseLetterColor(colorSensorId, 3, 1000),	//S
						  waitAction(4000),
						  morseLetterColor(colorSensorId, 2, 2000),	//O
						  waitAction(4000),
						  morseLetterColor(colorSensorId, 3, 1000)	//S
					  });
}

AbstractAction *ActionFactory::setParameterAction(int parameterId, double value) const
{
	return new SetParameterAction(parameterId, value, _manager);
}
