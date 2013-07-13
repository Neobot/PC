#include "TreasureIslandCommands.h"
#include "StrategyManager.h"
#include "DefaultStrategy.h"
#include <QVariant>
#include <QtDebug>

TSBottleCommand::TSBottleCommand(const QString& bottleAlias, StrategyManager *manager)
    : AbstractAICommand(manager), _bottleAlias(bottleAlias)
{
    setDescription("Retrieve " + _bottleAlias);
}

double TSBottleCommand::evaluate(GameState &state)
{
    if (state._content.value(_bottleAlias).toBool())
        return -1.0;

    _manager->getGrid();
    double d = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getNode(_bottleAlias));
    if (d <= 0)
        return -1.0;

    double duration = d / AVERAGE_SPEED + 10.0;
    if (state._remainingTime <= duration)
        return -1.0;

    state._remainingTime -= duration;

    double cost = 5.0/duration;
    return cost;
}

void TSBottleCommand::updateToFinalState(GameState &state) const
{
    //Update the state
    state._content[_bottleAlias] = true;
    state._robotposition = _manager->getGrid()->getNode(_bottleAlias);
}

AbstractAction *TSBottleCommand::getAction() const
{
    QPointF p(_manager->getGrid()->getNode(_bottleAlias)->getPosition());
    p.rx() += 200;

    QList<AbstractAction*> actionList;
    actionList  << _manager->getActionFactory()->moveAction(_manager->getGrid()->getNode(_bottleAlias))
                << _manager->getActionFactory()->moveAction(p, false, true) //Backward
                << _manager->getActionFactory()->waitAction(3000);
    return _manager->getActionFactory()->actionList(actionList);
}

//------------------------------------------------------------------------------------------

TSMapCommand::TSMapCommand(const QString& maplias, StrategyManager *manager)
    : AbstractAICommand(manager), _maplias(maplias)
{
    setDescription("Discover the map");
}

double TSMapCommand::evaluate(GameState &state)
{
    if (state._content.value(_maplias).toBool())
        return -1.0;

    double d = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getNode(_maplias));
    if (d <= 0)
        return -1.0;

    double duration = d / AVERAGE_SPEED + 15.0;
    if (state._remainingTime <= duration)
        return -1.0;

    state._remainingTime -= duration;

    double cost = 5.0/duration;
    return cost;
}

void TSMapCommand::updateToFinalState(GameState &state) const
{
    //Update the state
    state._content[_maplias] = true;
    state._robotposition = _manager->getGrid()->getNode(_maplias);
}

AbstractAction *TSMapCommand::getAction() const
{
    QList<AbstractAction*> actionList;
    actionList  << _manager->getActionFactory()->moveAction(_manager->getGrid()->getNode(_maplias))
                << _manager->getActionFactory()->waitAction(3000);
    return _manager->getActionFactory()->actionList(actionList);
}

//------------------------------------------------------------------------------------------

TSGatherSouthSeaCommand::TSGatherSouthSeaCommand(const QString& areaAlias, const Tools::Curve &curve, StrategyManager *manager)
    : AbstractAICommand(manager), _areaAlias(areaAlias), _probabilityCurve(curve)
{
    setDescription("Gather coins in the south sea");
    _manager->getGrid()->getArea(_areaAlias)->setCostToGoInside(3);
}

double TSGatherSouthSeaCommand::evaluate(GameState &state)
{
    if (state._content.value(ROBOT_CONTENT).toInt() >= MAX_ROBOT_CONTENT || state._content.value(_areaAlias).toBool())
        return -1.0;

    double d = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getArea(_areaAlias)->getCentralNode());
    if (d <= 0)
        return -1.0;

    double duration = d / AVERAGE_SPEED + 3.0;
    if (state._remainingTime <= duration)
        return -1.0;

    state._remainingTime -= duration;

    double cost = (3.0/2)/duration;
    cost *= _probabilityCurve.yValue(duration);

    return cost;
}

void TSGatherSouthSeaCommand::updateToFinalState(GameState &state) const
{
    //Update the state
    state._content[_areaAlias] = true;
    state._content[ROBOT_CONTENT] = 3; //3 coins gathered
    state._robotposition = _manager->getGrid()->getArea(_areaAlias)->getCentralNode();
}

AbstractAction *TSGatherSouthSeaCommand::getAction() const
{
    QList<AbstractAction*> actionList;
    actionList  << _manager->getActionFactory()->moveAction(_manager->getGrid()->getArea(_areaAlias))
                << _manager->getActionFactory()->waitAction(1000)
                << _manager->getActionFactory()->moveAction(_manager->getGrid()->getArea(_areaAlias)->getCentralNode())
                << _manager->getActionFactory()->waitAction(3000);
    return _manager->getActionFactory()->actionList(actionList);
}

void TSGatherSouthSeaCommand::end()
{
    _manager->getGrid()->getArea(_areaAlias)->setCostToGoInside(1);
}

//------------------------------------------------------------------------------------------

TSGatherTotemCommand::TSGatherTotemCommand(const QString& nodeInAlias, const QString& nodeOutAlias, const QString& mainNodeAlias, const Tools::Curve &curve, StrategyManager *manager)
    : AbstractAICommand(manager), _nodeInAlias(nodeInAlias), _nodeOutAlias(nodeOutAlias), _mainNodeAlias(mainNodeAlias), _probabilityCurve(curve)
{
    setDescription("Gather coins on a totem");
}

double TSGatherTotemCommand::evaluate(GameState &state)
{
    //Don't do it if robot is not empty or already done
    if (state._content.value(ROBOT_CONTENT).toInt() > 0 || state._content.value(_mainNodeAlias).toBool())
        return -1.0;

    double d = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getNode(_nodeInAlias));
    if (d <= 0)
        return -1.0;

    double duration = d / AVERAGE_SPEED + 5.0; //fixed duration: 5s
    if (state._remainingTime <= duration)
        return -1.0;

    state._remainingTime -= duration;

    double cost = (5.0/2)/duration; //5 points
    cost *= _probabilityCurve.yValue(duration);

    return cost;
}

void TSGatherTotemCommand::updateToFinalState(GameState &state) const
{
    //Update the state
    state._content[_mainNodeAlias] = true;
    state._content[ROBOT_CONTENT] = 5; //5 points
    state._robotposition = _manager->getGrid()->getNode(_nodeOutAlias);
}

AbstractAction *TSGatherTotemCommand::getAction() const
{
    QList<AbstractAction*> actionList;
    actionList  << _manager->getActionFactory()->moveAction(_manager->getGrid()->getNode(_nodeInAlias))
		<< _manager->getActionFactory()->ax12Action(DefaultStrategy::FRONT_LEFT, 180, 0) //open
		<< _manager->getActionFactory()->ax12Action(DefaultStrategy::FRONT_RIGHT, 180, 0) //open
                << _manager->getActionFactory()->waitAction(2000) //open
                << _manager->getActionFactory()->moveAction(_manager->getGrid()->getNode(_nodeOutAlias))
		<< _manager->getActionFactory()->ax12Action(DefaultStrategy::FRONT_LEFT, 0, 0) //open
		<< _manager->getActionFactory()->ax12Action(DefaultStrategy::FRONT_RIGHT, 0, 0) //open
                << _manager->getActionFactory()->waitAction(2000); //close
    return _manager->getActionFactory()->actionList(actionList);
}

//------------------------------------------------------------------------------------------

TSReleaseCommand::TSReleaseCommand(const QString& areaAlias, int maxPointsInside, StrategyManager *manager)
    : AbstractAICommand(manager), _areaAlias(areaAlias), _maxPoints(maxPointsInside)
{
    setDescription("Release coins");
    _manager->getGrid()->getArea(_areaAlias)->setCostToGoInside(3);
    _manager->getGrid()->getArea(_areaAlias)->setInternalCost(10);
}

double TSReleaseCommand::evaluate(GameState &state)
{
    int robotContent = state._content.value(ROBOT_CONTENT).toInt();
    if (robotContent <= 0 || state._content.value(_areaAlias).toInt() + robotContent > _maxPoints)
        return -1.0;

    double d = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getArea(_areaAlias)->getCentralNode());
    if (d <= 0)
        return -1.0;

    double duration = d / AVERAGE_SPEED + 4.0;
    if (state._remainingTime <= duration)
        return -1.0;

    state._remainingTime -= duration;

    double cost = state._content.value(ROBOT_CONTENT).toInt() /duration;

    return cost;
}

void TSReleaseCommand::updateToFinalState(GameState &state) const
{
    //Update the state
    state._content[_areaAlias] = state._content[ROBOT_CONTENT].toInt() + state._content[_areaAlias].toInt();
    state._content[ROBOT_CONTENT] = 0;
    state._robotposition = _manager->getGrid()->getArea(_areaAlias)->getCentralNode();
}

AbstractAction *TSReleaseCommand::getAction() const
{
    QList<AbstractAction*> actionList;
    actionList  << _manager->getActionFactory()->moveAction(_manager->getGrid()->getArea(_areaAlias))
		<< _manager->getActionFactory()->waitAction(4000)  //drop
		<< _manager->getActionFactory()->manualBackwardMoveAction(400);
		   ;

    return _manager->getActionFactory()->actionList(actionList);
}

//------------------------------------------------------------------------------------------

TSPushBarCommand::TSPushBarCommand(const QString& nodeAlias, StrategyManager *manager)
    : AbstractAICommand(manager), _nodeAlias(nodeAlias)
{
    setDescription("Push bar");
}

double TSPushBarCommand::evaluate(GameState &state)
{
    if (state._content.value(_nodeAlias).toBool() && state._content.value(LOADING_DECK_AREA).toInt() == 0)
	return -1.0;

    double d = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getNode(_nodeAlias));
    if (d <= 0)
	return -1.0;

    double duration = d / AVERAGE_SPEED;
    if (state._remainingTime <= duration)
	return -1.0;

    state._remainingTime -= duration;

    double cost = 5.0 / duration;

    return cost;
}

void TSPushBarCommand::updateToFinalState(GameState &state) const
{
    //Update the state
    state._content[_nodeAlias] = true;
    state._robotposition = _manager->getGrid()->getNode(_nodeAlias);
}

AbstractAction *TSPushBarCommand::getAction() const
{
    QList<AbstractAction*> actionList;
    actionList  << _manager->getActionFactory()->moveAction(_manager->getGrid()->getNode(_nodeAlias))
		<< _manager->getActionFactory()->manualBackwardMoveAction(400);
		   ;

    return _manager->getActionFactory()->actionList(actionList);
}
