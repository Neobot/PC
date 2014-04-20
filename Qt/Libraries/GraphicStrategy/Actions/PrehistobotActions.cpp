#include "PrehistobotActions.h"
#include "StrategyManager.h"
#include "TrajectoryFinder.h"
#include "Ax12MovementRunner.h"

PrehistobotScanAndTurnOverFiresAction::PrehistobotScanAndTurnOverFiresAction(Tools::NGridNode* destination, int speed, int timeoutMs, int ourColor, int opponentColor, const ColorSensor *leftSensor, const ColorSensor *rightSensor,
																		  AbstractAction* startAction,
																		  AbstractAction* leftOpponentColorAction, AbstractAction* rightOpponentColorAction,
																		  AbstractAction* leftOurColorAction, AbstractAction* rightOurColorAction, AbstractAction *endAction,
																		  StrategyManager* manager, TrajectoryFinder* finder, QObject* parent)
 : AbstractAction(parent), _manager(manager), _finder(finder),
   _destinationReached(false), _destination(destination), _speed(speed), _timeout(0), _ourColor(ourColor), _opponentColor(opponentColor), _leftSensor(leftSensor), _rightSensor(rightSensor),
   _startAction(startAction), _leftOpponentColorAction(leftOpponentColorAction), _rightOpponentColorAction(rightOpponentColorAction),
   _leftOurColorAction(leftOurColorAction), _rightOurColorAction(rightOurColorAction), _endAction(endAction),
   _leftArmState(Unknown), _rightArmState(Unknown), _stopped(false)
{
	if (timeoutMs > 0)
	{
		_timeout = new QTimer(this);
		_timeout->setInterval(timeoutMs);
		_timeout->setSingleShot(true);
		connect(_timeout, SIGNAL(timeout()), this, SLOT(timeout()));
	}
}


PrehistobotScanAndTurnOverFiresAction::~PrehistobotScanAndTurnOverFiresAction()
{
}


void PrehistobotScanAndTurnOverFiresAction::execute()
{
	connect(_manager, SIGNAL(sensorValuesReceived(Sensor::SensorFamily)), this, SLOT(testColor(Sensor::SensorFamily)));
	connect(_finder, SIGNAL(objectiveReached()), this, SLOT(destinationReached()));

	connect(_leftOpponentColorAction, SIGNAL(finished(bool)), this, SLOT(leftArmActionFinished(bool)));
	connect(_leftOurColorAction, SIGNAL(finished(bool)), this, SLOT(leftArmActionFinished(bool)));
	connect(_rightOpponentColorAction, SIGNAL(finished(bool)), this, SLOT(rightArmActionFinished(bool)));
	connect(_rightOurColorAction, SIGNAL(finished(bool)), this, SLOT(rightArmActionFinished(bool)));

	if (_startAction)
		executeSubAction(_startAction);
	else
		startActionFinished(true);
}


void PrehistobotScanAndTurnOverFiresAction::stop()
{
	_stopped = true;
	foreach(AbstractAction* subAction, _runningActions)
		subAction->stop();
}

void PrehistobotScanAndTurnOverFiresAction::end()
{
	disconnect(_manager, SIGNAL(sensorValuesReceived(Sensor::SensorFamily)), this, SLOT(testColor(Sensor::SensorFamily)));
	disconnect(_finder, SIGNAL(objectiveReached()), this, SLOT(destinationReached()));
}

QString PrehistobotScanAndTurnOverFiresAction::getActionName() const
{
	return "Scan and turn over fires";
}

void PrehistobotScanAndTurnOverFiresAction::testColor(Sensor::SensorFamily family)
{
	if (family == Sensor::ColorSensorFamily)
	{
		checkColor(_leftArmState, _leftSensor->getValue(), _leftOpponentColorAction, _leftOurColorAction);
		checkColor(_rightArmState, _rightSensor->getValue(), _rightOpponentColorAction, _rightOurColorAction);
	}
}

void PrehistobotScanAndTurnOverFiresAction::checkColor(ArmState& armState, int sensorColor, AbstractAction* turnOverFireAction, AbstractAction* moveFireAction)
{
	if (armState == Scanning)
	{
		if (sensorColor == _ourColor)
		{
			armState = Moving;
			executeArmSubAction(turnOverFireAction, armState);
		}
		else if (sensorColor == _opponentColor)
		{
			armState = Moving;
			executeArmSubAction(moveFireAction, armState);
		}
	}
}

void PrehistobotScanAndTurnOverFiresAction::leftArmActionFinished(bool result)
{
	AbstractAction *action = qobject_cast<AbstractAction*>(sender());
	endSubAction(action);

	armActionFinsihed(_leftArmState, result);
}

void PrehistobotScanAndTurnOverFiresAction::rightArmActionFisnihed(bool result)
{
	AbstractAction *action = qobject_cast<AbstractAction*>(sender());
	endSubAction(action);

	armActionFinsihed(_rightArmState, result);
}

void PrehistobotScanAndTurnOverFiresAction::startActionFinished(bool result)
{
	AbstractAction *action = qobject_cast<AbstractAction*>(sender());
	endSubAction(action);

	_destinationReached = false;
	if (_destination)
		_finder->findTrajectory(_destination, _speed, Tools::Forward);

	if (_timeout)
		_timeout->start();


	_leftArmState = Scanning;
	_rightArmState = Scanning;
}

void PrehistobotScanAndTurnOverFiresAction::endActionFinished(bool result)
{
	AbstractAction *action = qobject_cast<AbstractAction*>(sender());
	endSubAction(action);

	succeed();
}

void PrehistobotScanAndTurnOverFiresAction::destinationReached()
{
	_destinationReached = true;
	if (_timeout)
		_timeout->stop();

	checkEnding();
}

void PrehistobotScanAndTurnOverFiresAction::timeout()
{
	if (!_destinationReached && _destination)
		_finder->stop();

	_destinationReached = true;
	checkEnding();
}

void PrehistobotScanAndTurnOverFiresAction::armActionFinsihed(ArmState& armState, bool result)
{
	switch(armState)
	{
		case Moving:
			armState = Scanning;
			checkEnding();
			break;
		default:
			Q_ASSERT(false);
			break;
	}
}

void PrehistobotScanAndTurnOverFiresAction::executeArmSubAction(AbstractAction *action, ArmState& armState)
{
	if (action)
	{
		executeSubAction(action);
	}
	else
	{
		armActionFinsihed(armState, true);
	}
}

void PrehistobotScanAndTurnOverFiresAction::executeSubAction(AbstractAction *action)
{
	action->execute();
	_runningActions << action;
}

void PrehistobotScanAndTurnOverFiresAction::endSubAction(AbstractAction *action)
{
	if (action)
	{
		_runningActions.removeAll(action);
		action->end();

		if (_stopped && _runningActions.isEmpty())
			failed();
	}
}

void PrehistobotScanAndTurnOverFiresAction::checkEnding()
{
	if (_destinationReached && _leftArmState == Scanning && _rightArmState == Scanning)
	{
		if (_endAction)
			executeSubAction(_endAction);
		else
			succeed();
	}
}
