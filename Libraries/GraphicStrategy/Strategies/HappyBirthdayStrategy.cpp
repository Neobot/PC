#include "HappyBirthdayStrategy.h"
#include "HappyBirthdayCommands.h"
#include "GameState.h"
#include <StrategyManager.h>
#include <StrategyMap.h>
#include <SharpFactory.h>
#include"DataObject.h"
#include <vmath.h>
#include <cmath>

using namespace Tools;

const int GLASS_LEFT_SWITCH = 0;
const int GLASS_RIGHT_SWITCH = 1;

HappyBirthdayStrategy::HappyBirthdayStrategy(const QDir& strategyDirectory, Tools::AbstractLogger *logger)
	: DefaultAIStrategy(AIEngine::CostMaximizing, strategyDirectory, logger),
	  _plateRadius(80), _ownGlassesAreaPenalty(50), _opponentGlassesAreaPenalty(50), _estimatedPointsPerGlass(8), _opponentEstimatedPointsPerGlass(8),
	  _containerLeftMax(4), _containerRightMax(4), _maxNumberOfGlassesPerContainerToMove(0),
	  _timeToTakeAGlass(2), _timeToOpenAPresent(1), _timeToBlowACandle(3)
{
	LoggerInterface::logger() << "---TREASURE ISLAND STRATEGY---" << Tools::endl;
	_takeLeftAction = 0;
	_takeRightAction = 0;
}

void HappyBirthdayStrategy::defaultStrategyParameters(StrategyInterface::StrategyParameters &parameters) const
{
	DefaultStrategy::defaultStrategyParameters(parameters);

	parameters. enableAutoAvoiding = false;

	parameters. start = QPointF(1400, 250);
	parameters. startRotation = Tools::pi/2;
}

void HappyBirthdayStrategy::readParametersFromFile(NSettings &settings)
{
	DefaultStrategy::readParametersFromFile(settings);

	settings.beginGroup("Happy Birthday Strategy");
	double fileVersion = manageParameterVersion(settings, "HB_strategy");
	Q_UNUSED(fileVersion); //unused for now

	_plateRadius = settings.value("plates_radius").toInt();
	_ownGlassesAreaPenalty = settings.value("own_glasses_area_penalty").toInt();
	_opponentGlassesAreaPenalty = settings.value("opponent_glasses_area_penalty").toInt();
	_estimatedPointsPerGlass = settings.value("own_estimated_point_per_glasses").toInt();
	_opponentEstimatedPointsPerGlass = settings.value("opponent_estimated_point_per_glasses").toInt();

	_timeToTakeAGlass = settings.value("estimated_time_per_glass").toDouble();
	_timeToOpenAPresent = settings.value("estimated_time_per_present").toDouble();
	_timeToBlowACandle = settings.value("estimated_time_per_candle").toDouble();

	_containerLeftMax = settings.value("container_left_max_content").toInt();
	_containerRightMax = settings.value("container_right_max_content").toInt();
	_maxNumberOfGlassesPerContainerToMove = settings.value("max_glasses_per_container_for_presents").toDouble();

	settings.endGroup();
}

void HappyBirthdayStrategy::writeDefaultParametersToFile(NSettings &settings)
{
	DefaultAIStrategy::writeDefaultParametersToFile(settings);

	settings.beginGroup("Happy Birthday Strategy");
	settings.setValue("plates_radius", _plateRadius, "Radius of the plates.");
	settings.setValue("own_glasses_area_penalty", _ownGlassesAreaPenalty, "Penalty added to the pather in order to avoid the glasses in our camp.");
	settings.setValue("opponent_glasses_area_penalty", _opponentGlassesAreaPenalty, "Penalty added to the pather in order to avoid the glasses in the opponent camp.");
	settings.setValue("own_estimated_point_per_glasses", _estimatedPointsPerGlass, "Average amount of points earned with one of our glass.");
	settings.setValue("opponent_estimated_point_per_glasses", _opponentEstimatedPointsPerGlass, "Average amount of points earned with one of the opponent glass.");

	settings.setValue("estimated_time_per_glass", _timeToTakeAGlass, "Estimated time in seconds to take a glass.");
	settings.setValue("estimated_time_per_present", _timeToOpenAPresent, "Estimated time in seconds to open a present.");
	settings.setValue("estimated_time_per_candle", _timeToBlowACandle, "Estimated time in seconds to blow a candle.");

	settings.setValue("container_left_max_content", _containerLeftMax, "Max number of glasses in the left container.");
	settings.setValue("container_right_max_content", _containerRightMax, "Max number of glasses in the right container.");
	settings.setValue("max_glasses_per_container_to_move", _maxNumberOfGlassesPerContainerToMove, "Maximum number of glasses in a container to be able to move around.");

	settings.endGroup();
}

void HappyBirthdayStrategy::initGameState(GameState &state) const
{
	state._content[PRESENT1_NODE] = false;
	state._content[PRESENT2_NODE] = false;
	state._content[PRESENT3_NODE] = false;
	state._content[PRESENT4_NODE] = false;
	
	state._content["1"] = false;
	state._content["2"] = false;
	state._content["3"] = false;
	state._content["4"] = false;
	state._content["5"] = false;
	state._content["6"] = false;
	state._content["7"] = false;
	state._content["8"] = false;
	state._content["9"] = false;
	state._content["10"] = false;
	state._content["11"] = false;
	state._content["12"] = false;
	
	state._content[ROBOT_CONTAINER_LEFT] = 0;
	state._content[ROBOT_CONTAINER_RIGHT] = 0;

	state._content[CANDLE1_NODE] = false;
	state._content[CANDLE2_NODE] = false;
	state._content[CANDLE3_NODE] = false;
}

void HappyBirthdayStrategy::createActions()
{
	_manager->addAtion(_manager->getActionFactory()->ax12Movement(LEFT_ARM_GROUP, INIT_MOVEMENT));
	_manager->addAtion(_manager->getActionFactory()->ax12Movement(RIGHT_ARM_GROUP, INIT_MOVEMENT));

	addCommand(new WaitCommand(1, _manager));

	//Presents commands
	addCommand(new HBOpenPresentCommand(PRESENT1_NODE, _timeToOpenAPresent, _maxNumberOfGlassesPerContainerToMove, _manager));
	addCommand(new HBOpenPresentCommand(PRESENT2_NODE, _timeToOpenAPresent, _maxNumberOfGlassesPerContainerToMove, _manager));
	addCommand(new HBOpenPresentCommand(PRESENT3_NODE, _timeToOpenAPresent, _maxNumberOfGlassesPerContainerToMove, _manager));
	addCommand(new HBOpenPresentCommand(PRESENT4_NODE, _timeToOpenAPresent, _maxNumberOfGlassesPerContainerToMove, _manager));

	//glasses commands
	//HBAreaLocker* areaLocker = new HBAreaLocker(_manager->getGrid());

		//our camp
//	areaLocker->lockArea(GLASS1_AREA, _ownGlassesAreaPenalty, 1, _ownGlassesAreaPenalty);
//	areaLocker->lockArea(GLASS2_AREA, _ownGlassesAreaPenalty, 1, _ownGlassesAreaPenalty);
//	areaLocker->lockArea(GLASS3_AREA, _ownGlassesAreaPenalty, 1, _ownGlassesAreaPenalty);
//	areaLocker->lockArea(GLASS4_AREA, _ownGlassesAreaPenalty, 1, _ownGlassesAreaPenalty);
//	areaLocker->lockArea(GLASS5_AREA, _ownGlassesAreaPenalty, 1, _ownGlassesAreaPenalty);
//	areaLocker->lockArea(GLASS6_AREA, _ownGlassesAreaPenalty, 1, _ownGlassesAreaPenalty);

//	addCommand(new HBTakeGlassCommand(1, GLASS1A_NODE, GLASS1_AREA, HBTakeGlassCommand::Right, _estimatedPointsPerGlass, _timeToTakeAGlass, _containerLeftMax, _containerRightMax, areaLocker, _manager));
//	addCommand(new HBTakeGlassCommand(2, GLASS2A_NODE, GLASS2_AREA, HBTakeGlassCommand::Right, _estimatedPointsPerGlass, _timeToTakeAGlass, _containerLeftMax, _containerRightMax, areaLocker, _manager));
//	addCommand(new HBTakeGlassCommand(3, GLASS3A_NODE, GLASS3_AREA, HBTakeGlassCommand::Left, _estimatedPointsPerGlass, _timeToTakeAGlass, _containerLeftMax, _containerRightMax, areaLocker, _manager));
//	addCommand(new HBTakeGlassCommand(4, GLASS4A_NODE, GLASS4_AREA, HBTakeGlassCommand::Left, _estimatedPointsPerGlass, _timeToTakeAGlass, _containerLeftMax, _containerRightMax, areaLocker, _manager));
//	addCommand(new HBTakeGlassCommand(5, GLASS5A_NODE, GLASS5_AREA, HBTakeGlassCommand::Left, _estimatedPointsPerGlass, _timeToTakeAGlass, _containerLeftMax, _containerRightMax, areaLocker, _manager));
//	addCommand(new HBTakeGlassCommand(6, GLASS6A_NODE, GLASS6_AREA, HBTakeGlassCommand::Left, _estimatedPointsPerGlass, _timeToTakeAGlass, _containerLeftMax, _containerRightMax, areaLocker, _manager));

		//opponent camp
//	areaLocker->lockArea(GLASS7_AREA, _opponentGlassesAreaPenalty, 1, _opponentGlassesAreaPenalty);
//	areaLocker->lockArea(GLASS8_AREA, _opponentGlassesAreaPenalty, 1, _opponentGlassesAreaPenalty);
//	areaLocker->lockArea(GLASS9_AREA, _opponentGlassesAreaPenalty, 1, _opponentGlassesAreaPenalty);
//	areaLocker->lockArea(GLASS10_AREA, _opponentGlassesAreaPenalty, 1, _opponentGlassesAreaPenalty);
//	areaLocker->lockArea(GLASS11_AREA, _opponentGlassesAreaPenalty, 1, _opponentGlassesAreaPenalty);
//	areaLocker->lockArea(GLASS12_AREA, _opponentGlassesAreaPenalty, 1, _opponentGlassesAreaPenalty);

//	addCommand(new HBTakeGlassCommand(7, GLASS7A_NODE, GLASS7_AREA, HBTakeGlassCommand::Right, _opponentEstimatedPointsPerGlass, _timeToTakeAGlass, _containerLeftMax, _containerRightMax, areaLocker, _manager));
//	addCommand(new HBTakeGlassCommand(8, GLASS8A_NODE, GLASS8_AREA, HBTakeGlassCommand::Right, _opponentEstimatedPointsPerGlass, _timeToTakeAGlass, _containerLeftMax, _containerRightMax, areaLocker, _manager));
//	addCommand(new HBTakeGlassCommand(9, GLASS9A_NODE, GLASS9_AREA, HBTakeGlassCommand::Left, _opponentEstimatedPointsPerGlass, _timeToTakeAGlass, _containerLeftMax, _containerRightMax, areaLocker, _manager));
//	addCommand(new HBTakeGlassCommand(10, GLASS10A_NODE, GLASS10_AREA, HBTakeGlassCommand::Left, _opponentEstimatedPointsPerGlass, _timeToTakeAGlass, _containerLeftMax, _containerRightMax, areaLocker, _manager));
//	addCommand(new HBTakeGlassCommand(11, GLASS11A_NODE, GLASS11_AREA, HBTakeGlassCommand::Left, _opponentEstimatedPointsPerGlass, _timeToTakeAGlass, _containerLeftMax, _containerRightMax, areaLocker, _manager));
//	addCommand(new HBTakeGlassCommand(12, GLASS12A_NODE, GLASS12_AREA, HBTakeGlassCommand::Left, _opponentEstimatedPointsPerGlass, _timeToTakeAGlass, _containerLeftMax, _containerRightMax, areaLocker, _manager));

//	addCommand(new HBReleaseCommand(CONSTRUCTION_AREA, _manager));

//	//candle commands
//	addCommand(new HBBlowCandleCommand(CANDLE1_NODE, _timeToBlowACandle, _maxNumberOfGlassesPerContainerToMove, _manager));
//	addCommand(new HBBlowCandleCommand(CANDLE2_NODE, _timeToBlowACandle, _maxNumberOfGlassesPerContainerToMove, _manager));
//	addCommand(new HBBlowCandleCommand(CANDLE3_NODE, _timeToBlowACandle, _maxNumberOfGlassesPerContainerToMove, _manager));
}

void HappyBirthdayStrategy::defaultObstaclePositions(QList<DataObject *> &objects) const
{
	Tools::RPoint p = getStartPosition();
	double x = p.getX();
	if (x > 400)
		objects << new CircleObject(_plateRadius, autoMirror(RPoint(250, 200)).toQPointF());

	if (x < 400 || x > 800)
		objects << new CircleObject(_plateRadius, autoMirror(RPoint(600, 200)).toQPointF());

	if (x < 800 || x > 1200)
		objects << new CircleObject(_plateRadius, autoMirror(RPoint(1000, 200)).toQPointF());

	if (x < 1200 || x > 1600)
		objects << new CircleObject(_plateRadius, autoMirror(RPoint(1400, 200)).toQPointF());

	if (x < 1600)
		objects << new CircleObject(_plateRadius, autoMirror(RPoint(1750, 200)).toQPointF());
}

void HappyBirthdayStrategy::microswitchChanged(const QList<int> &raisingEdge, const QList<int> &faillingEdge)
{
	return;
	Q_UNUSED(faillingEdge);

	if (!_takeLeftAction && raisingEdge.contains(GLASS_LEFT_SWITCH))
	{
		logger() << "A glass on the left position has been detected: Cancelling current action." << Tools::endl;
		_manager->cancelCurrentAction();

		QList<AbstractAction*> actionList;
		actionList  //<< _manager->getActionFactory()->ax12Movement(RIGHT_ARM_GROUP, "")
					<< _manager->getActionFactory()->waitAction(1500);

		_takeLeftAction = _manager->getActionFactory()->actionList(actionList);
		connect(_takeLeftAction, SIGNAL(finished(bool)), this, SLOT(takeLeftFinished()));

		_manager->addAtion(_takeLeftAction);

		addNextAction();
	}

	if (!_takeRightAction && raisingEdge.contains(GLASS_RIGHT_SWITCH))
	{
		logger() << "A glass on the right position has been detected: Cancelling current action." << Tools::endl;
		_manager->cancelCurrentAction();

		QList<AbstractAction*> actionList;
		actionList  //<< _manager->getActionFactory()->ax12Movement(RIGHT_ARM_GROUP, "")
					<< _manager->getActionFactory()->waitAction(1500);

		_takeRightAction = _manager->getActionFactory()->actionList(actionList);
		connect(_takeRightAction, SIGNAL(finished(bool)), this, SLOT(takeRightFinished()));

		_manager->addAtion(_takeRightAction);

		addNextAction();
	}
}

void HappyBirthdayStrategy::takeLeftFinished()
{
	disconnect(_takeLeftAction, SIGNAL(finished(bool)), this, SLOT(takeLeftFinished()));
	_takeLeftAction = 0;
}

void HappyBirthdayStrategy::takeRightFinished()
{
	disconnect(_takeRightAction, SIGNAL(finished(bool)), this, SLOT(takeLeftFinished()));
	_takeRightAction = 0;
}

bool HappyBirthdayStrategy::checkGrid(const NGrid *grid) const
{
	bool result = true;

	result = grid->getNode(PRESENT1_NODE) && result;
	result = grid->getNode(PRESENT2_NODE) && result;
	result = grid->getNode(PRESENT3_NODE) && result;
	result = grid->getNode(PRESENT4_NODE) && result;
	
	result = grid->getNode(GLASS1A_NODE) && result;
	//result = grid->getNode(GLASS1B_NODE) && result;
	result = grid->getNode(GLASS2A_NODE) && result;
	result = grid->getNode(GLASS3A_NODE) && result;
	result = grid->getNode(GLASS4A_NODE) && result;
	result = grid->getNode(GLASS5A_NODE) && result;
	result = grid->getNode(GLASS6A_NODE) && result;
//	result = grid->getNode(GLASS7A_NODE) && result;
//	result = grid->getNode(GLASS8A_NODE) && result;
//	result = grid->getNode(GLASS9A_NODE) && result;
//	result = grid->getNode(GLASS10A_NODE) && result;
//	result = grid->getNode(GLASS11A_NODE) && result;
//	result = grid->getNode(GLASS12A_NODE) && result;
	
	result = grid->getArea(GLASS1_AREA) && result;
	result = grid->getArea(GLASS2_AREA) && result;
	result = grid->getArea(GLASS3_AREA) && result;
	result = grid->getArea(GLASS4_AREA) && result;
	result = grid->getArea(GLASS5_AREA) && result;
	result = grid->getArea(GLASS6_AREA) && result;
//	result = grid->getArea(GLASS7_AREA) && result;
//	result = grid->getArea(GLASS8_AREA) && result;
//	result = grid->getArea(GLASS9_AREA) && result;
//	result = grid->getArea(GLASS10_AREA) && result;
//	result = grid->getArea(GLASS11_AREA) && result;
//	result = grid->getArea(GLASS12_AREA) && result;

	result = grid->getArea(CONSTRUCTION_AREA) && result;

	return result;
}
