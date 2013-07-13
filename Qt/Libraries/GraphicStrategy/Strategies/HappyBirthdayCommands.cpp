#include "HappyBirthdayCommands.h"
#include "StrategyManager.h"
#include "DefaultStrategy.h"
#include "vmath.h"

#include <QVariant>
#include <QtDebug>

HBOpenPresentCommand::HBOpenPresentCommand(const QString& presentAlias, double estimatedTimePerPresent, int maxNumberOfGlassesPerContainer, StrategyManager *manager)
	: AbstractAICommand(manager), _presentAlias(presentAlias), _estimatedTimePerPresent(estimatedTimePerPresent), _maxNumberOfGlassesPerContainer(maxNumberOfGlassesPerContainer)
{
	setDescription("Opens " + _presentAlias);
}

double HBOpenPresentCommand::evaluate(GameState &state)
{
	if (state._content.value(_presentAlias).toBool())
        return -1.0;

    _manager->getGrid();
	double d = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getNode(_presentAlias));
    if (d <= 0)
        return -1.0;

	if (state._content.value(ROBOT_CONTAINER_LEFT).toInt() > _maxNumberOfGlassesPerContainer || state._content.value(ROBOT_CONTAINER_RIGHT).toInt() > _maxNumberOfGlassesPerContainer)
		return -1.0;

	double duration = d / AVERAGE_SPEED + _estimatedTimePerPresent;
    if (state._remainingTime <= duration)
        return -1.0;

    state._remainingTime -= duration;

	double cost = 4.0/duration;
    return cost;
}

void HBOpenPresentCommand::updateToFinalState(GameState &state) const
{
    //Update the state
	state._content[_presentAlias] = true;
	state._robotposition = _manager->getGrid()->getNode(_presentAlias);
}

AbstractAction *HBOpenPresentCommand::getAction(const GameState &state) const
{
    QList<AbstractAction*> actionList;

	OrientationSwitchCaseAction* armSwitchAction = _manager->getActionFactory()->orientationSwitchCaseAction();
	armSwitchAction->addCase(0, Tools::pi, _manager->getActionFactory()->ax12Movement(RIGHT_ARM_GROUP, PRESENT_MOVEMENT));
	armSwitchAction->addCase(-Tools::pi, 0, _manager->getActionFactory()->ax12Movement(LEFT_ARM_GROUP, PRESENT_MOVEMENT));

    actionList  << _manager->getActionFactory()->moveAction(_manager->getGrid()->getNode(_presentAlias), 100)
				<< armSwitchAction;
				
    return _manager->getActionFactory()->actionList(actionList);
}

////------------------------------------------------------------------------------------------

HBTakeGlassCommand::HBTakeGlassCommand(int glassNum, const QString& glassNodeAlias, const QString& glassAreaAlias, Arm arm, int estimatedPointsPerGlass,
									   double estimatedTimePerGlass, int leftContainerMaxContent, int rightContainerMaxContent, HBAreaLocker *areaLocker, StrategyManager *manager)
	: AbstractAICommand(manager), _nodeAlias(glassNodeAlias), _areaAlias(glassAreaAlias), _areaLocker(areaLocker),
	  _estimatedPointsPerGlass(estimatedPointsPerGlass), _estimatedTimePerGlass(estimatedTimePerGlass),
	  _leftContainerMaxContent(leftContainerMaxContent), _rightContainerMaxContent(rightContainerMaxContent)
{
	_glassId = QString::number(glassNum);
	
	_doLeft = arm == Left || arm == Both;
	_doRight = arm == Right || arm == Both;
	
	QString armString;
	switch(arm)
	{
		case Left: armString = "the left arm"; break;
		case Right: armString = "the right arm"; break;
		case Both: armString = "both arms"; break;
	}
	setDescription(QString("Takes the glass ").append(_glassId).append(" at position ")
				   .append(_nodeAlias).append(" with ").append(armString));
}

double HBTakeGlassCommand::evaluate(GameState &state)
{
	if (state._content.value(_glassId).toBool())
		return -1.0;
		
	if (_doLeft && state._content.value(ROBOT_CONTAINER_LEFT).toInt() >= _leftContainerMaxContent)
		return -1.0;
		
	if (_doRight && state._content.value(ROBOT_CONTAINER_RIGHT).toInt() >= _rightContainerMaxContent)
		return -1.0;

	_manager->getGrid();
	double d = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getNode(_nodeAlias));
	if (d <= 0)
		return -1.0;

	double duration = d / AVERAGE_SPEED + _estimatedTimePerGlass;
	if (state._remainingTime <= duration)
		return -1.0;

	state._remainingTime -= duration;
	
	double points = _estimatedPointsPerGlass;
	double cost = points / duration;
	
	return cost;
}

void HBTakeGlassCommand::updateToFinalState(GameState &state) const
{
	//Update the state
	state._content[_glassId] = true;
	if (_doLeft)
	{
		int contentLeft = state._content[ROBOT_CONTAINER_LEFT].toInt();
		state._content[ROBOT_CONTAINER_LEFT] = contentLeft + 1;
	}
	
	if (_doRight)
	{
		int contentRight = state._content[ROBOT_CONTAINER_RIGHT].toInt();
		state._content[ROBOT_CONTAINER_RIGHT] = contentRight + 1;
	}
	
	state._robotposition = _manager->getGrid()->getNode(_nodeAlias);
}

AbstractAction *HBTakeGlassCommand::getAction(const GameState& state) const
{
	QList<AbstractAction*> actionList;
    actionList  << _manager->getActionFactory()->moveAction(_manager->getGrid()->getNode(_nodeAlias), 100)
				<< _manager->getActionFactory()->manualForwardMoveAction(100, 30)
				<< _manager->getActionFactory()->waitAction(500);
				
	if (_doLeft)
	{
		//actionList  << _manager->getActionFactory()->ax12Movement(LEFT_ARM_GROUP, ...) //
	}
	
	if (_doRight)
	{
		//actionList  << _manager->getActionFactory()->ax12Movement(RIGHT_ARM_GROUP, ...) //
	}
	
	return _manager->getActionFactory()->actionList(actionList);
}

void HBTakeGlassCommand::end()
{
	_areaLocker->unlockArea(_areaAlias);
}

//------------------------------------------------------------------------------------------

HBReleaseCommand::HBReleaseCommand(const QString& areaAlias, StrategyManager *manager)
   : AbstractAICommand(manager), _areaAlias(areaAlias)
{
   setDescription("Release towers");
   _manager->getGrid()->getArea(_areaAlias)->setCostToGoInside(3);
   _manager->getGrid()->getArea(_areaAlias)->setInternalCost(10);
}

double HBReleaseCommand::evaluate(GameState &state)
{
   int robotContentLeft = state._content.value(ROBOT_CONTAINER_LEFT).toInt();
   int robotContentRight = state._content.value(ROBOT_CONTAINER_RIGHT).toInt();
   if ((robotContentLeft <= 0 && robotContentRight <= 0 )|| state._content.value(_areaAlias).toInt())
       return -1.0;

   double d = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getArea(_areaAlias)->getCentralNode());
   if (d <= 0)
       return -1.0;

   double duration = d / AVERAGE_SPEED + 2.0;
   if (state._remainingTime <= duration)
       return -1.0;

   state._remainingTime -= duration;
   
   int points = calculatePoint(robotContentLeft) + calculatePoint(robotContentRight);

   double cost = points / duration;

   return cost;
}

int HBReleaseCommand::calculatePoint(int nbGlassTower) const
{
	int result = 0;
	for(int i = 0; i < nbGlassTower; ++i) 
		result += (i + 1) * 4;
		
	return result;
}

void HBReleaseCommand::updateToFinalState(GameState &state) const
{
   //Update the state
   state._content[ROBOT_CONTAINER_LEFT] = 0;
   state._content[ROBOT_CONTAINER_RIGHT] = 0;
   state._robotposition = _manager->getGrid()->getArea(_areaAlias)->getCentralNode();
}

AbstractAction *HBReleaseCommand::getAction(const GameState &state) const
{
   QList<AbstractAction*> actionList;
   actionList  << _manager->getActionFactory()->moveAction(_manager->getGrid()->getArea(_areaAlias), 100)
		<< _manager->getActionFactory()->waitAction(4000)  //drop
        << _manager->getActionFactory()->manualBackwardMoveAction(100, 100);
		   ;

   return _manager->getActionFactory()->actionList(actionList);
}

//------------------------------------------------------------------------------------------

void HBAreaLocker::lockArea(const QString &area, int costToGoInside, int costToGoOutside, int internalCost)
{
	AreaLockingValues values;
	values.costToGoInside = costToGoInside;
	values.costToGoOutside = costToGoOutside;
	values.internalCost = internalCost;

	_lockedAreas[area] = values;
	_areaList << area;

	setAreaValues(area);
}

void HBAreaLocker::setAreaValues(const QString &area)
{
	Tools::NGridArea* a = _grid->getArea(area);
	if (a && _lockedAreas.contains(area))
	{
		const AreaLockingValues& values = _lockedAreas[area];
		a->setCostToGoInside(values.costToGoInside);
		a->setCostToGoOutside(values.costToGoOutside);
		a->setInternalCost(values.internalCost);
	}
}

void HBAreaLocker::unlockArea(const QString &area)
{
	Tools::NGridArea* a = _grid->getArea(area);
	if (a && _lockedAreas.contains(area))
	{
		a->setCostToGoInside(1);
		a->setCostToGoOutside(1);
		a->setInternalCost(1);

		_lockedAreas.remove(area);
		_areaList.removeAll(area);

		foreach(const QString& area, _areaList)
			setAreaValues(area);
	}
}

//------------------------------------------------------------------------------------------

HBBlowCandleCommand::HBBlowCandleCommand(const QString& candleAlias, double estimatedTimePerCandle, int maxNumberOfGlassesPerContainer, StrategyManager *manager)
	: AbstractAICommand(manager), _candleAlias(candleAlias), _estimatedTimePerCandle(estimatedTimePerCandle), _maxNumberOfGlassesPerContainer(maxNumberOfGlassesPerContainer)
{
	setDescription("Opens " + _candleAlias);
}

double HBBlowCandleCommand::evaluate(GameState &state)
{
	if (state._content.value(_candleAlias).toBool())
		return -1.0;

	_manager->getGrid();
	double d = _manager->getFuturePathingDistance(state, state._robotposition, _manager->getGrid()->getNode(_candleAlias));
	if (d <= 0)
		return -1.0;

	if (state._content.value(ROBOT_CONTAINER_LEFT).toInt() > _maxNumberOfGlassesPerContainer || state._content.value(ROBOT_CONTAINER_RIGHT).toInt() > _maxNumberOfGlassesPerContainer)
		return -1.0;

	double duration = d / AVERAGE_SPEED + _estimatedTimePerCandle;
	if (state._remainingTime <= duration)
		return -1.0;

	state._remainingTime -= duration;

	double cost = 4.0/duration;
	return cost;
}

void HBBlowCandleCommand::updateToFinalState(GameState &state) const
{
	//Update the state
	state._content[_candleAlias] = true;
	state._robotposition = _manager->getGrid()->getNode(_candleAlias);
}

AbstractAction *HBBlowCandleCommand::getAction(const GameState& state) const
{
	QList<AbstractAction*> actionList;

	OrientationSwitchCaseAction* armSwitchAction = _manager->getActionFactory()->orientationSwitchCaseAction();
//	armSwitchAction->addCase(0, Tools::pi, _manager->getActionFactory()->ax12Movement(RIGHT_ARM_GROUP, ""));
//	armSwitchAction->addCase(-Tools::pi, 0, _manager->getActionFactory()->ax12Movement(RIGHT_ARM_GROUP, ""));
//	armSwitchAction->addCase(0, Tools::pi, _manager->getActionFactory()->waitAction(1000));
//	armSwitchAction->addCase(-Tools::pi, 0, _manager->getActionFactory()->waitAction(5000));
	//armSwitchAction->addCase(0, Tools::pi, _manager->getActionFactory()->ax12Movement("Test", "Mvt4"));
	//armSwitchAction->addCase(-Tools::pi, 0, _manager->getActionFactory()->asynchroneAx12Movement("Test", "Mvt4"));

	actionList  << _manager->getActionFactory()->moveAction(_manager->getGrid()->getNode(_candleAlias), 100)
				<< _manager->getActionFactory()->waitAction(1500);
				//<< armSwitchAction;

	return _manager->getActionFactory()->actionList(actionList);
}
