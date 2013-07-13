#include "TreasureIslandStrategy.h"

#include "TreasureIslandCommands.h"
#include "GameState.h"
#include <StrategyManager.h>
#include <StrategyMap.h>
#include <SharpFactory.h>
#include <vmath.h>
#include <cmath>

#include <QtDebug>
#include <QTime>

using namespace Tools;

const QString& TREASURE_ISLAND_DEAULT_GRID = "TreasureIslandDefaultGrid.ngrid";

TreasureIslandStrategy::TreasureIslandStrategy(Tools::AbstractLogger *logger)
    : DefaultStrategy(logger), AIEngine(AIEngine::CostMaximizing)
{
    LoggerInterface::logger() << "---TREASURE ISLAND STRATEGY---" << Tools::endl;
}

void TreasureIslandStrategy::parameters(StrategyParameters& parameters) const
{
    DefaultStrategy::parameters(parameters);

    parameters. robotRadius = 270.0;			//mm
    parameters. opponentRadius = 250.0;                 //mm

    parameters. enableAutoAvoiding = false;
    parameters. sharpObjectRadius = 250.0;		//mm
    parameters. sharpDetectionOverlapRadius = 200.0;	//mm
    parameters. sharpDetectionTime = 5000;		//ms
    parameters. noDetectionZones << QRectF(800, 900, 400, 1200);

    parameters. start = _manager->getGrid()->getNode(START_NODE);
    parameters. startRotation = _manager->isMirrored() ? -Tools::pi/2 : Tools::pi/2;
}

bool TreasureIslandStrategy::initGrid(NGrid *grid) const
{
    if (QFile::exists(TREASURE_ISLAND_DEAULT_GRID))
        grid->readFromFile(TREASURE_ISLAND_DEAULT_GRID, _manager->isMirrored());
    else
        DefaultStrategy::initGrid(grid);

    return checkGrid();
}

bool TreasureIslandStrategy::checkGrid() const
{
    bool result = true;
    result = _manager->getGrid()->getNode(START_NODE) && result;
    result = _manager->getGrid()->getNode(BOTTLE1_NODE) && result;
    result = _manager->getGrid()->getNode(BOTTLE2_NODE) && result;
    result = _manager->getGrid()->getNode(DECK_NODE) && result;

    result = _manager->getGrid()->getNode(TOTEM1_NORTH_NODE) && result;
    result = _manager->getGrid()->getNode(TOTEM1_WEST_NODE) && result;
    result = _manager->getGrid()->getNode(TOTEM1_SOUTH_NODE) && result;
    result = _manager->getGrid()->getNode(TOTEM2_NORTH_NODE) && result;
    result = _manager->getGrid()->getNode(TOTEM2_EAST_NODE) && result;
    result = _manager->getGrid()->getNode(TOTEM2_SOUTH_NODE) && result;

    result = _manager->getGrid()->getNode(OPP_START_NODE) && result;
    result = _manager->getGrid()->getNode(OPP_DECK_NODE) && result;

    result = _manager->getGrid()->getArea(CAPTAIN_BEDROOM_AREA) && result;
    result = _manager->getGrid()->getArea(LOADING_DECK_AREA) && result;
    result = _manager->getGrid()->getArea(OPPONENT_BEDROOM_AREA) && result;
    result = _manager->getGrid()->getArea(OPPONENT_DECK_AREA) && result;
    result = _manager->getGrid()->getArea(SOUTH_SEA_AREA) && result;

    return result;
}

void TreasureIslandStrategy::testSpeed()
{
    for (int i = 1; i < 8; ++i)
    {
	logger() << i << " action(s) : ";
	QTime t;
	t.start();
	nextAtion(i, AIEngine::BestCost, _manager->getCurrentState());
	logger() << t.elapsed() / 100.0 << " seconds" << Tools::endl;
    }
}

void TreasureIslandStrategy::mainStrategy(QList<AbstractAction *> &actions)
{
    _manager->getCurrentState()._content[BOTTLE1_NODE] = false;
    _manager->getCurrentState()._content[BOTTLE2_NODE] = false;
    _manager->getCurrentState()._content[MAP_NODE] = false;
    _manager->getCurrentState()._content[SOUTH_SEA_AREA] = false;
    _manager->getCurrentState()._content[ROBOT_CONTENT] = 0;
    _manager->getCurrentState()._content[DECK_NODE] = 0;

    clearAndDeleteCommands();

    addCommand(new TSPushBarCommand(DECK_NODE, _manager));
    //addCommand(new TSBottleCommand(BOTTLE1_NODE, _manager));
    //addCommand(new TSBottleCommand(BOTTLE2_NODE, _manager));
    //addCommand(new TSMapCommand(MAP_NODE, _manager));
    addCommand(new WaitCommand(1, _manager));

//    Tools::Curve southSeaCurve;
//    southSeaCurve << Point(0, 1) << Point(30, 1) << Point(70, 0.1) << Point(90, 0.1);
//    addCommand(new TSGatherSouthSeaCommand(SOUTH_SEA_AREA, southSeaCurve, _manager));

//    Tools::Curve totem1Curve;
//    totem1Curve << Point(0, 1) << Point(70, 1) << Point(90, 0.1);
//    addCommand(new TSGatherTotemCommand(TOTEM1_NORTH_NODE, TOTEM1_WEST_NODE, TOTEM1_NORTH_NODE, totem1Curve, _manager));
//    addCommand(new TSGatherTotemCommand(TOTEM1_SOUTH_NODE, TOTEM1_WEST_NODE, TOTEM1_SOUTH_NODE, totem1Curve, _manager));

//    Tools::Curve totem2Curve;
//    totem2Curve << Point(0, 1) << Point(40, 1) << Point(60, 0.1) << Point(90, 0.1);
//    addCommand(new TSGatherTotemCommand(TOTEM2_EAST_NODE, TOTEM2_SOUTH_NODE, TOTEM2_SOUTH_NODE, totem2Curve, _manager));
//    addCommand(new TSGatherTotemCommand(TOTEM2_EAST_NODE, TOTEM2_NORTH_NODE, TOTEM2_NORTH_NODE, totem2Curve, _manager));

    addCommand(new TSReleaseCommand(CAPTAIN_BEDROOM_AREA, 4, _manager));
    addCommand(new TSReleaseCommand(LOADING_DECK_AREA, 99999, _manager));

    //testSpeed();


    AbstractAICommand* c = nextAtion(2, AIEngine::BestCost, _manager->getCurrentState());
    if (c)
    {
        actions << c->getAction();
        logger() << "Command: \"" << c->getDescription() << "\"" << Tools::endl;
    }
}

void TreasureIslandStrategy::actionDone(const AbstractAction *action, bool result)
{
    Q_UNUSED(action);
    AbstractAICommand* c = nextAtion(2, AIEngine::BestCost, _manager->getCurrentState(), result);
    if (c)
    {
        _manager->addAtion(c->getAction());
        logger() << "Command: \"" << c->getDescription() << "\" - " << _manager->getCurrentState()._remainingTime << " seconds remaining." << Tools::endl;
    }
    else
    {
        logger() << "No commands to do... "   << _manager->getCurrentState()._remainingTime << " seconds remaining." << Tools::endl;
    }
}

void TreasureIslandStrategy::obstacleDetected()
{
    if (_manager->movementInProgress())
    {
        _manager->cancelCurrentAction();
        logger() << "OBSTACLE detected: Cancelling current action." << Tools::endl;
    }
}

void TreasureIslandStrategy::blockingDeteced()
{
    if (_manager->movementInProgress())
    {
        _manager->cancelCurrentAction();
        logger() << "BLOCKING detected: Cancelling current action." << Tools::endl;
    }
}
