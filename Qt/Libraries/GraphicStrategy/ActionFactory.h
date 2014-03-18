#ifndef ACTIONFACTORY_H
#define ACTIONFACTORY_H

class StrategyManager;
class TrajectoryFinder;
class StrategyMap;

#include "Actions/StandardActions.h"
#include "Actions/ContainerActions.h"
#include "Actions/ConditionalActions.h"

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
    AbstractAction* manualTurnMoveAction(double radian, int speed) const;
    AbstractAction* manualTurnToAction(const Tools::RPoint& point, int speed, bool clockwize = false) const;
    AbstractAction* manualAbsoluteTurnMoveAction(double radian, int speed) const;

	AbstractAction* actuatorAction(Comm::ServoId servoId, Comm::ServoPosition position, int estimatedDuration) const;

    AbstractAction* ax12Action(quint8 id, float angle, float speed) const;
    AbstractAction* ax12Action(quint8 id1, float angle1, quint8 id2, float angle2, float speed) const;
	
	AbstractAction* ax12Movement(const QString& group, const QString& movement, float speedLimit = 110.f) const;

    AbstractAction* waitForSharpToActivate(int otherSharpId, int timeoutInMs) const;
    AbstractAction* waitForSharpToActivateWithCustomValue(int otherSharpId, double threshold, int timeoutInMs) const;
    AbstractAction* waitForSharpToDesactivate(int otherSharpId, int timeoutInMs) const;
    AbstractAction* waitForSharpToDesactivateWithCustomValue(int otherSharpId, double threshold, int timeoutInMs) const;

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
