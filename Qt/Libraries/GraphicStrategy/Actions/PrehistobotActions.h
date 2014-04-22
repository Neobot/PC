#ifndef PREHISTOBOTACTIONS_H
#define PREHISTOBOTACTIONS_H

#include "AbstractAction.h"
#include "Sensor.h"
#include "NGrid.h"

#include <QTimer>

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
 * @brief Action which scan two color sensors and execute sub-action when predefined colors are found.
 */
class ColorScanAction : public AbstractAction
{
	Q_OBJECT
public:
	/**
	 * @brief Construct the action
	 * \p destination can be null, in that case, the robot won't move.
	 * \p timeout can be <= 0, on that case no timeout is applied.
	 */
	ColorScanAction(Tools::NGridNode* destination, int speed, int timeoutMs, int ourColor, int opponentColor, const ColorSensor* leftSensor, const ColorSensor* rightSensor,
										  AbstractAction* startAction,
										  AbstractAction* leftOpponentColorAction, AbstractAction* rightOpponentColorAction,
										  AbstractAction* leftOurColorAction, AbstractAction* rightOurColorAction,
										  AbstractAction* endAction,
										  StrategyManager* manager, TrajectoryFinder* finder, QObject* parent = 0);

	~ColorScanAction();

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
	QTimer* _timeout;

	int _ourColor;
	int _opponentColor;

	const ColorSensor* _leftSensor;
	const ColorSensor* _rightSensor;

	AbstractAction* _startAction;

	AbstractAction* _leftOpponentColorAction;
	AbstractAction* _rightOpponentColorAction;

	AbstractAction* _leftOurColorAction;
	AbstractAction* _rightOurColorAction;

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

	void executeArmSubAction(AbstractAction* action, ArmState &armState);
	void executeSubAction(AbstractAction* action);
	void endSubAction(AbstractAction* action);

	void checkEnding();

private slots:
	void testColor(Sensor::SensorFamily family);

	void startActionFinished(bool result);
	void leftArmActionFinished(bool result);
	void rightArmActionFisnihed(bool result);
	void endActionFinished(bool result);
	void destinationReached();
	void timeout();

};

#endif // PREHISTOBOTACTIONS_H
