#ifndef ACTIONFACTORY_H
#define ACTIONFACTORY_H

class StrategyManager;
class TrajectoryFinder;
class StrategyMap;

#include "Actions/StandardActions.h"
#include "Actions/ContainerActions.h"
#include "Actions/ConditionalActions.h"
#include "Actions/PrehistobotActions.h"

namespace Comm {class RobotCommInterface;}
namespace Tools {class Ax12MovementManager;}

class ActionFactory
{
public:
    ActionFactory(StrategyManager* manager, TrajectoryFinder* finder, StrategyMap* map, Comm::RobotCommInterface* robot, Tools::Ax12MovementManager* ax12MovementsManager);
	void setMaxMovementSpeed(int maxSpeed);

	ActionGroup* actionList(const QList<AbstractAction*>& actions = QList<AbstractAction*>()) const;
	AsynchroneActionGroup* asynchroneActionList(const QList<AbstractAction*>& actions = QList<AbstractAction*>(), AsynchroneActionGroup::StopCondition stopCondition = AsynchroneActionGroup::OneActionFinished) const;

	OrientationSwitchCaseAction* orientationSwitchCaseAction() const;
	SensorSwitchCaseAction* colorSensorSwitchCaseAction(int colorSensorId) const;
	SensorSwitchCaseAction* sharpSwitchCaseAction(int sharpId) const;
	SensorSwitchCaseAction* microswitchSwitchCaseAction(int microswicthId) const;
	PositionSwitchCaseAction* positionSwitchCaseAction() const;

    AbstractAction* waitAction(int ms) const;

    AbstractAction* moveAction(Tools::NGridNode* destination, int speed, bool forceForward = false, bool forceBackward = false, Tools::Deplacement deplacementType = Tools::TURN_THEN_MOVE) const;
    AbstractAction* moveAction(const QPointF& destinationPoint, int speed, bool forceForward = false, bool forceBackward = false, Tools::Deplacement deplacementType = Tools::TURN_THEN_MOVE) const;
    AbstractAction* moveAction(const QString& destinationAlias, int speed, bool forceForward = false, bool forceBackward = false, Tools::Deplacement deplacementType = Tools::TURN_THEN_MOVE) const;
    AbstractAction* moveAction(Tools::NGridArea* destination, int speed, bool forceForward = false, bool forceBackward = false, Tools::Deplacement deplacementType = Tools::TURN_THEN_MOVE) const;
    AbstractAction* moveAction(Tools::NGridArea* destinationArea, int speed, Tools::NGridNode* targetNode, bool forceForward = false, bool forceBackward = false, Tools::Deplacement deplacementType = Tools::TURN_THEN_MOVE) const;

    AbstractAction* manualMoveAction(const Tools::Trajectory& trajectory, int speed, Tools::Movement movement, Tools::Deplacement deplacementType = Tools::TURN_THEN_MOVE) const;
    AbstractAction* manualMoveToPointAction(const Tools::RPoint& point, int speed, bool forward = true, Tools::Deplacement deplacementType = Tools::TURN_THEN_MOVE) const;
    AbstractAction* manualBackwardMoveAction(double mm, int speed) const;
    AbstractAction* manualForwardMoveAction(double mm, int speed) const;
    AbstractAction* manualTurnToAction(const Tools::RPoint& point, int speed, bool clockwize = false) const;
    AbstractAction* manualAbsoluteTurnMoveAction(double radian, int speed) const;

	AbstractAction* actuatorAction(Comm::RobotAction action, int parameter, int estimatedDurationMs) const;

    AbstractAction* ax12Action(quint8 id, float angle, float speed) const;
    AbstractAction* ax12Action(quint8 id1, float angle1, quint8 id2, float angle2, float speed) const;
	
	AbstractAction* ax12Movement(const QString& group, const QString& movement, float speedLimit = 110.f) const;
	AbstractAction* ax12AsynchroneMovement(const QString& group, const QString& movement, float speedLimit = 110.f) const;

    AbstractAction* waitForSharpToActivate(int otherSharpId, int timeoutInMs) const;
    AbstractAction* waitForSharpToDesactivate(int otherSharpId, int timeoutInMs) const;
	
	AbstractAction* waitForColor(int colorSensorId, int color, int timeoutInMs) const;
	
	AbstractAction* waitForMicroswitchToActivate(int microswitchId, int timeoutInMs) const;
    AbstractAction* waitForMicroswitchToDesactivate(int microswitchId, int timeoutInMs) const;

	ColorScanAction* colorScanAction(Tools::NGridNode* destination, int speed, int timeoutMs, int ourColor, int opponentColor, int leftColorSensorId, int rightColorSensorId,
															   AbstractAction* startAction,
															   AbstractAction* leftOpponentColorAction, AbstractAction* rightOpponentColorAction,
															   AbstractAction* leftOurColorAction, AbstractAction* rightOurColorAction,
															   AbstractAction* endAction);
	AbstractAction* startPumpAction(int id) const;
	AbstractAction* stopPumpAction(int id) const;

	AbstractAction* enableColorSensorAction(int colorSensorId) const;
	AbstractAction* disableColorSensorAction(int colorSensorId) const;

	AbstractAction* enableSharpAction(int sharpId) const;
	AbstractAction* disableSharpAction(int sharpId) const;

	AbstractAction* enableMicroswitchAction(int microswitchId) const;
	AbstractAction* disableMicroswitchAction(int microswitchId) const;

	AbstractAction* morseLetterColor(int colorSensorId, int nbSignal, int duration) const;
	AbstractAction* sosColor(int colorSensorId) const;

private:
    StrategyManager*	_manager;
    TrajectoryFinder*	_finder;
    StrategyMap*	_map;
    Comm::RobotCommInterface*	_robot;
	Tools::Ax12MovementManager* _ax12MovementsManager;
	int _maxMovementSpeed;
	
	int getRealSpeed(int requestedSpeed) const;

};

#endif // ACTIONFACTORY_H
