#include "PrehistobotCommands.h"
#include "StrategyManager.h"
#include "PrehistobotStrategy.h"
#include "vmath.h"

#include <QVariant>
#include <QtDebug>

PBFrescoCommand::PBFrescoCommand(const QString &frescoAlias, double estimatedTimeInSeconds, StrategyManager *manager)
	: AbstractAICommand(manager), _frescoAlias(frescoAlias), _estimatedTime(estimatedTimeInSeconds)
{
	setDescription("Attach painting " + _frescoAlias);
}

double PBFrescoCommand::evaluate(GameState &state)
{
	if (state._content.value(_frescoAlias).toBool())
		return -1.0;

	double d = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getNode(_frescoAlias));
	if (d <= 0)
		return -1.0;

	double duration = calculateActionTime(d, AVERAGE_SPEED, _estimatedTime);
	if (state._remainingTime <= duration)
		return -1.0;

	state._remainingTime -= duration;

	double cost = 6.0 / duration;
	return cost;
}

void PBFrescoCommand::updateToFinalState(GameState &state) const
{
	//Update the state
	state._content[_frescoAlias] = true;
	state._robotposition = _manager->getGrid()->getNode(_frescoAlias);
}

AbstractAction *PBFrescoCommand::getAction(const GameState &state) const
{
	Q_UNUSED(state);

	QList<AbstractAction*> actionList;

	actionList  << _manager->getActionFactory()->moveAction(_manager->getGrid()->getNode(_frescoAlias), 100)
				   //...
				;
				
	return _manager->getActionFactory()->actionList(actionList);
}

////------------------------------------------------------------------------------------------

PBFruitPickupCommand::PBFruitPickupCommand(const QString& fruitAliasA, double angleA, RobotSide sideA, const QString& fruitAliasB, double angleB, RobotSide sideB, double distance, double estimatedTimeInSeconds, StrategyManager *manager)
	: AbstractAICommand(manager), _fruitAliasA(fruitAliasA), _angleA(angleA), _sideA(sideA), _fruitAliasB(fruitAliasB), _angleB(angleB), _sideB(sideB), _distance(distance), _estimatedTime(estimatedTimeInSeconds)
{
	setDescription("Fruit pickup at " + _fruitAliasA + " or " + _fruitAliasB);
}

double PBFruitPickupCommand::evaluate(GameState &state)
{
	if (state._content.value(_fruitAliasA).toBool() || state._content.value(_fruitAliasB).toBool() )
		return -1.0;

	double da = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getNode(_fruitAliasA));
	double db = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getNode(_fruitAliasB));
	double d = da <= db ? da : db;

	if (d <= 0)
		return -1.0;

	double duration = calculateActionTime(d, AVERAGE_SPEED, _estimatedTime);
	if (state._remainingTime <= duration)
		return -1.0;

	state._remainingTime -= duration;

	double cost = 3.0 / duration;
	return cost;
}

void PBFruitPickupCommand::updateToFinalState(GameState &state) const
{
	//Update the state
	double da = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getNode(_fruitAliasA));
	double db = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getNode(_fruitAliasB));

	QString selectedAlias;
	double selectedAngle;
	RobotSide selectedSide;

	getOptions(da, db, selectedAlias, selectedAngle, selectedSide);

	state._content[_fruitAliasA] = true;
	state._content[_fruitAliasB] = true;
	int nbPickupDone = state._content.value(NB_FRUIT_PICKUP).toInt();
	state._content[NB_FRUIT_PICKUP] = nbPickupDone + 1;
	state._robotposition = _manager->getGrid()->getNode(selectedAlias);
}

AbstractAction *PBFruitPickupCommand::getAction(const GameState &state) const
{
	Q_UNUSED(state);

	QList<AbstractAction*> actionList;

	double da = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getNode(_fruitAliasA));
	double db = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getNode(_fruitAliasB));

	QString selectedAlias;
	double selectedAngle;
	RobotSide selectedSide;

	getOptions(da, db, selectedAlias, selectedAngle, selectedSide);
	actionList  << _manager->getActionFactory()->moveAction(_manager->getGrid()->getNode(selectedAlias), 100)
				<< _manager->getActionFactory()->manualAbsoluteTurnMoveAction(selectedAngle, 100)
				<< _manager->getActionFactory()->manualForwardMoveAction(_distance, 100)
				   //...
					;

	return _manager->getActionFactory()->actionList(actionList);
}

void PBFruitPickupCommand::getOptions(double distanceToA, double distanceToB, QString &alias, double &angle, RobotSide &side) const
{
	bool useOptionA = distanceToA <= distanceToB;

	if (useOptionA)
	{
		alias = _fruitAliasA;
		angle = _angleA;
		side = _sideA;
	}
	else
	{
		alias = _fruitAliasB;
		angle = _angleB;
		side = _sideB;
	}
}

////------------------------------------------------------------------------------------------

PBFruitDropCommand::PBFruitDropCommand(const QString &dropAreaAlias, double estimatedTimeInSeconds, StrategyManager *manager)
	: AbstractAICommand(manager), _dropAreaAlias(dropAreaAlias), _estimatedTime(estimatedTimeInSeconds)
{
	setDescription("Drop fruits at" + _dropAreaAlias);
}

double PBFruitDropCommand::evaluate(GameState &state)
{
	int nbPickupDone = state._content.value(NB_FRUIT_PICKUP).toInt();
	if (nbPickupDone <= 0)
		return -1.0;

	double d = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getArea(_dropAreaAlias)->getCentralNode());
	if (d <= 0)
		return -1.0;

	double duration = calculateActionTime(d, AVERAGE_SPEED, _estimatedTime);
	if (state._remainingTime <= duration)
		return -1.0;

	state._remainingTime -= duration;

	double cost = 3.0 * (double)nbPickupDone / duration;
	return cost;
}

void PBFruitDropCommand::updateToFinalState(GameState &state) const
{
	state._content[NB_FRUIT_PICKUP] = 0;
	state._robotposition = _manager->getGrid()->getArea(_dropAreaAlias)->getCentralNode();
}

AbstractAction *PBFruitDropCommand::getAction(const GameState &state) const
{
	Q_UNUSED(state);

	QList<AbstractAction*> actionList;

	actionList  << _manager->getActionFactory()->moveAction(_manager->getGrid()->getArea(_dropAreaAlias), 100)
				<< _manager->getActionFactory()->manualAbsoluteTurnMoveAction(-Tools::pi, 100)
				   //...
				;

	return _manager->getActionFactory()->actionList(actionList);
}

////------------------------------------------------------------------------------------------

PBSearchFiresCommand::PBSearchFiresCommand(const QList<QPointF> points, PBActionFactory *pbFactory, StrategyManager *manager)
	: AbstractAICommand(manager), _points(points), _pbFactory(pbFactory)
{
	setDescription("Search for fires");
}

double PBSearchFiresCommand::evaluate(GameState &state)
{
	Q_UNUSED(state);
	return 1/90.0; //smallest value possible
}

void PBSearchFiresCommand::updateToFinalState(GameState &state) const
{
	Q_UNUSED(state);
}

AbstractAction *PBSearchFiresCommand::getAction(const GameState &state) const
{
	QList<AbstractAction*> actions;
	QList<QPointF> list = getSortedPointList(state);
	if (!list.isEmpty())
	{

		foreach(const QPointF& p, list)
		{
			actions << _pbFactory->scanAndTurnFires(_manager->getGrid()->getNearestNode(p));
		}
	}

	return _manager->getActionFactory()->actionList(actions);
}

QList<QPointF> PBSearchFiresCommand::getSortedPointList(const GameState &state) const
{
	QPointF robotPos = state._robotposition->getPosition();

	QList<QPointF> result;
	if (!_points.isEmpty())
	{
		//get nearest point index
		int minDist = qAbs((_points.first()-robotPos).manhattanLength());
		int minIndex = 0;
		int i = 1;

		for(QList<QPointF>::const_iterator it = _points.begin() + 1; it != _points.end(); ++it)
		{
			int dist = qAbs((*it-robotPos).manhattanLength());
			if (dist < minDist)
			{
				minDist = dist;
				minIndex = i;
			}
			++i;
		}


		for(int i = minIndex; i < _points.count(); ++i)
			result << _points[i];

		for(int i = 0; i < minIndex; ++i)
			result << _points[i];
	}

	return result;
}

////------------------------------------------------------------------------------------------

PBTakeFixedTorcheCommand::PBTakeFixedTorcheCommand(const QString& torcheAlias, bool vertical, double estimatedTimeInSeconds, bool turnFire, PBActionFactory* pbFactory, StrategyManager* manager)
	: AbstractAICommand(manager), _pbFactory(pbFactory), _torcheAlias(torcheAlias), _vertical(vertical), _estimatedTime(estimatedTimeInSeconds), _turnFire(turnFire)
{
	setDescription("Take fire in fixed torche " + _torcheAlias);
}

double PBTakeFixedTorcheCommand::evaluate(GameState &state)
{
	if (state._content.value(_torcheAlias).toBool())
		return -1.0;

	double d = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getNode(_torcheAlias));
	if (d <= 0)
		return -1.0;

	double duration = calculateActionTime(d, AVERAGE_SPEED, _estimatedTime);
	if (state._remainingTime <= duration)
		return -1.0;

	state._remainingTime -= duration;

	double cost = 1.0 / duration;
	return cost;
}

void PBTakeFixedTorcheCommand::updateToFinalState(GameState &state) const
{
	//Update the state
	state._content[_torcheAlias] = true;
	state._robotposition = _manager->getGrid()->getNode(_torcheAlias);
}

AbstractAction *PBTakeFixedTorcheCommand::getAction(const GameState &state) const
{
	Q_UNUSED(state);

	QList<AbstractAction*> actionList;

	Tools::NGridNode* node = _manager->getGrid()->getNode(_torcheAlias);
	bool isOnLeftSide = node->getPosition().x() < 1500;

	OrientationSwitchCaseAction* switchAction = _manager->getActionFactory()->orientationSwitchCaseAction();

	if (_vertical)
	{
		ActionGroup* upDirectionActions = _manager->getActionFactory()->actionList({
						_manager->getActionFactory()->manualAbsoluteTurnMoveAction(-Tools::pi, 100),
						_pbFactory->takeAndHoldFireInFixedTorche(isOnLeftSide ? DefaultStrategy::LeftPump : DefaultStrategy::RightPump)
					});

		ActionGroup* downDirectionActions = _manager->getActionFactory()->actionList({
						 _manager->getActionFactory()->manualAbsoluteTurnMoveAction(0, 100),
					 _pbFactory->takeAndHoldFireInFixedTorche(isOnLeftSide ? DefaultStrategy::RightPump : DefaultStrategy::LeftPump)
					});

		switchAction->addCase(-Tools::pi/2.0, Tools::pi/2.0, downDirectionActions);
		switchAction->setDefaultAction(upDirectionActions);
	}
	else
	{
		ActionGroup* rightDirectionsActions = _manager->getActionFactory()->actionList({
						_manager->getActionFactory()->manualAbsoluteTurnMoveAction(Tools::pi/2.0, 100),
						_pbFactory->takeAndHoldFireInFixedTorche(DefaultStrategy::RightPump)
					});

		ActionGroup* leftDirectionsActions = _manager->getActionFactory()->actionList({
						 _manager->getActionFactory()->manualAbsoluteTurnMoveAction(-Tools::pi/2.0, 100),
					 _pbFactory->takeAndHoldFireInFixedTorche(DefaultStrategy::LeftPump)
					});

		switchAction->addCase(0, Tools::pi, rightDirectionsActions);
		switchAction->setDefaultAction(leftDirectionsActions);
	}

	actionList  << _manager->getActionFactory()->moveAction(node, 100)
				<< switchAction
				;

	return _manager->getActionFactory()->actionList(actionList);
}

////------------------------------------------------------------------------------------------

PBEasyFireCommand::PBEasyFireCommand(const QString& aliasA, const QString& aliasB, double availableTimeToPerformAction, double estimatedTimeInSeconds, PBActionFactory* pbFactory, StrategyManager* manager)
	: AbstractAICommand(manager), _pbFactory(pbFactory), _aliasA(aliasA), _aliasB(aliasB), _availableTimeToPerformAction(availableTimeToPerformAction), _estimatedTime(estimatedTimeInSeconds)
{
	setDescription("Easy fire: " + _aliasA + " - " + _aliasB);
}

double PBEasyFireCommand::evaluate(GameState &state)
{
	if (state._content.value(_aliasA).toBool())
		return -1.0;

	if (state._remainingTime < (90 - _availableTimeToPerformAction)) //only available during the first x seconds of the match
		return -1.0;

	double da = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getNode(_aliasA));
	double db = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getNode(_aliasB));
	double d = da <= db ? da : db;

	if (d <= 0)
		return -1.0;

	double duration = calculateActionTime(d, AVERAGE_SPEED, _estimatedTime);
	if (state._remainingTime <= duration)
		return -1.0;

	state._remainingTime -= duration;

	double cost = 5.0 / duration; //high value because it's easy

	return cost;
}

void PBEasyFireCommand::updateToFinalState(GameState &state) const
{
	//Update the state
	double da = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getNode(_aliasA));
	double db = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getNode(_aliasB));

	QString firstAlias;
	QString secondAlias;
	getOptions(da, db, firstAlias, secondAlias);

	state._content[_aliasA] = true;
	state._robotposition = _manager->getGrid()->getNode(secondAlias);
}

AbstractAction *PBEasyFireCommand::getAction(const GameState &state) const
{
	Q_UNUSED(state);

	double da = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getNode(_aliasA));
	double db = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getNode(_aliasB));

	QString firstAlias;
	QString secondAlias;
	getOptions(da, db, firstAlias, secondAlias);

	Q_UNUSED(state);

	QList<AbstractAction*> actionList;

	actionList  << _manager->getActionFactory()->moveAction(_manager->getGrid()->getNode(firstAlias), 100)
				<< _pbFactory->scanAndTurnFires(_manager->getGrid()->getNode(secondAlias))
				;

	return _manager->getActionFactory()->actionList(actionList);
}

void PBEasyFireCommand::getOptions(double distanceToA, double distanceToB, QString &firstAlias, QString &secondAlias) const
{
	bool useOptionA = distanceToA <= distanceToB;

	if (useOptionA)
	{
		firstAlias = _aliasA;
		secondAlias = _aliasB;
	}
	else
	{
		firstAlias = _aliasB;
		secondAlias = _aliasA;
	}
}
