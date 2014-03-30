#ifndef PREHISTOBOTACTIONS_H
#define PREHISTOBOTACTIONS_H

#include "AbstractAction.h"
#include "Sensor.h"
#include "NGrid.h"

class ColorSensor;
class TrajectoryFinder;
namespace Tools
{
	class Ax12MovementManager;
}
namespace Comm
{
	class RobotCommInterface;
	class Ax12MovementRunner;
}

/**
 * @brief A group of synchrone actions.
 * Subactions are launched order they have been added. The group takes their ownership and is in charge of their destruction.
 */
class PrehistobotScanAndTurnOverFiresAction : public AbstractAction
{
	Q_OBJECT
public:
	PrehistobotScanAndTurnOverFiresAction(Tools::NGridNode* destination, int speed, int ourColor, int opponentColor, ColorSensor* leftSensor, ColorSensor* rightSensor,
										  AbstractAction* startAction,
										  AbstractAction* leftTurnOverAction, AbstractAction* rightTurnOverAction,
										  AbstractAction* leftMoveAction, AbstractAction* rightMoveAction,
										  AbstractAction* endAction,
										  StrategyManager* manager, TrajectoryFinder* finder, QObject* parent = 0);

	~PrehistobotScanAndTurnOverFiresAction();

	void execute();
	void stop();
	void end();

	QString getActionName() const;

private:
	StrategyManager* _manager;
	TrajectoryFinder* _finder;

	bool _destinationReached;
	Tools::NGridNode* _destination;
	int _speed;

	int _ourColor;
	int _opponentColor;

	ColorSensor* _leftSensor;
	ColorSensor* _rightSensor;

	AbstractAction* _startAction;

	AbstractAction* _leftTurnOverFireAction;
	AbstractAction* _rightTurnOverFireAction;

	AbstractAction* _leftMoveFireAction;
	AbstractAction* _rightMoveFireAction;

	AbstractAction* _endAction;

	enum ArmState
	{
		Unknown,
		Scanning,
		Moving
	};

	ArmState _leftArmState;
	ArmState _rightArmState;

	QList<AbstractAction*> _runningActions;
	bool _stopped;

	void checkColor(ArmState &armState, int sensorColor, AbstractAction* turnOverFireAction, AbstractAction* moveFireAction);
	void armActionFinsihed(ArmState &armState, bool result);
	void goToDestination();

	void executeArmSubAction(AbstractAction* action, ArmState &armState);
	void executeSubAction(AbstractAction* action);
	void endSubAction(AbstractAction* action);

	void checkEnding();

private slots:
	void testColor(Sensor::SensorFamily family);
	void leftArmMovementFinished(bool result, const QString& group, const QString& movement);
	void rightArmMovementFinished(bool result, const QString& group, const QString& movement);

	void startActionFinished(bool result);
	void leftArmActionFinished(bool result);
	void rightArmActionFisnihed(bool result);
	void endActionFinished(bool result);
	void destinationReached();

};

#endif // PREHISTOBOTACTIONS_H
