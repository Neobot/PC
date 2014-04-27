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

	double cost = 3.0 / 2.0 / duration;
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
				<< _manager->getActionFactory()->manualTurnMoveAction(selectedAngle, 100)
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
	double nbPickupDone = state._content.value(NB_FRUIT_PICKUP).toInt();
	if (nbPickupDone > 0)
		return -1.0;

	double d = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getNode(_dropAreaAlias));
	if (d <= 0)
		return -1.0;

	double duration = calculateActionTime(d, AVERAGE_SPEED, _estimatedTime);
	if (state._remainingTime <= duration)
		return -1.0;

	state._remainingTime -= duration;

	double cost = 3.0 * nbPickupDone / duration;
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
				<< _manager->getActionFactory()->manualTurnMoveAction(-Tools::pi, 100)
				   //...
				;

	return _manager->getActionFactory()->actionList(actionList);
}
