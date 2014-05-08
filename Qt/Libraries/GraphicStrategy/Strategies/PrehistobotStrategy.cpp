#include "PrehistobotStrategy.h"
#include "PrehistobotCommands.h"
#include "GameState.h"
#include <StrategyManager.h>
#include <StrategyMap.h>
#include <SharpFactory.h>
#include"DataObject.h"
#include <vmath.h>
#include <cmath>

using namespace Tools;

PrehistobotStrategy::PrehistobotStrategy(const QDir& strategyDirectory, Tools::AbstractLogger *logger)
	: DefaultAIStrategy(AIEngine::CostMaximizing, strategyDirectory, logger)
{
	LoggerInterface::logger() << "---PREHISTOBOT STRATEGY---" << Tools::endl;
}

void PrehistobotStrategy::defaultStrategyParameters(StrategyInterface::StrategyParameters &parameters) const
{
	DefaultStrategy::defaultStrategyParameters(parameters);

	parameters. enableAutoAvoiding = false;

	parameters. start = QPointF(250, 250);
	parameters. startRotation = Tools::pi/2;
}

void PrehistobotStrategy::readAndDefineParameters(NSettings &settings)
{
	DefaultStrategy::readAndDefineParameters(settings);

	settings.beginGroup("Prehistobot Strategy");
	settings.endGroup();

	settings.beginGroup("Prehistobot AX-12 Movements");

	_ax12MvtNames.leftArmGroup = defineSettingValue(settings, "left_arm_group", _ax12MvtNames.leftArmGroup, "Left arm AX-12 group.").toString();
	_ax12MvtNames.rightArmGroup = defineSettingValue(settings, "right_arm_group", _ax12MvtNames.rightArmGroup, "Left arm AX-12 group.").toString();

	_ax12MvtNames.goToRest = defineSettingValue(settings, "go_to_rest", _ax12MvtNames.goToRest, "Move the arm to the default position.").toString();
	_ax12MvtNames.goToScan = defineSettingValue(settings, "go_to_scan", _ax12MvtNames.goToScan, "Move the arm to the scan position.").toString();
	_ax12MvtNames.pump = defineSettingValue(settings, "pump", _ax12MvtNames.pump, "Moves a bit the arm after a scan to catch a fire.").toString();
	_ax12MvtNames.moveFire = defineSettingValue(settings, "move_fire", _ax12MvtNames.moveFire, "Move the fire without turning it.").toString();
	_ax12MvtNames.turnFire = defineSettingValue(settings, "turn_fire", _ax12MvtNames.turnFire, "Turn the fire.").toString();
	_ax12MvtNames.holdFire = defineSettingValue(settings, "hold_fire", _ax12MvtNames.holdFire, "Hold the fire.").toString();
	_ax12MvtNames.goToScanInTorche = defineSettingValue(settings, "go_to_scan_in_torche", _ax12MvtNames.goToScanInTorche, "Go to the scan position in a mobile torche.").toString();
	_ax12MvtNames.moveOutOfTorche = defineSettingValue(settings, "move_out_of_torche", _ax12MvtNames.moveOutOfTorche, "Move the arm out of the mobile torche.").toString();
	_ax12MvtNames.dropFire = defineSettingValue(settings, "drop_fire", _ax12MvtNames.dropFire, "Turn the fire without turning it.").toString();
	_ax12MvtNames.dropAndTurnFire = defineSettingValue(settings, "drop_and_turn_fire", _ax12MvtNames.dropAndTurnFire, "Drop the fire while turning it.").toString();

	settings.endGroup();
}

void PrehistobotStrategy::initGameState(GameState &state) const
{
	state._content[FRESCO_NODE] = false;

	state._content[NB_FRUIT_PICKUP] = 0;
	state._content[FRUIT_1A_NODE] = false;
	state._content[FRUIT_1B_NODE] = false;
	state._content[FRUIT_2A_NODE] = false;
	state._content[FRUIT_2B_NODE] = false;
	state._content[FRUIT_3A_NODE] = false;
	state._content[FRUIT_3B_NODE] = false;
	state._content[FRUIT_4A_NODE] = false;
	state._content[FRUIT_4B_NODE] = false;
}

void PrehistobotStrategy::createActions()
{
	_manager->getActionFactory()->asynchroneActionList(
		{
			_manager->getActionFactory()->ax12Movement(_ax12MvtNames.leftArmGroup, _ax12MvtNames.goToRest),
			_manager->getActionFactory()->ax12Movement(_ax12MvtNames.rightArmGroup, _ax12MvtNames.goToRest)
		}, AsynchroneActionGroup::AllActionFinished);

	addCommand(new WaitCommand(1, _manager));

	addCommand(new PBFrescoCommand(FRESCO_NODE, 6, _manager));

	addCommand(new PBFruitPickupCommand(FRUIT_1A_NODE, 0, RightSide, FRUIT_1B_NODE, autoMirror(Tools::pi), LeftSide, 400, 2, _manager));
	addCommand(new PBFruitPickupCommand(FRUIT_2A_NODE, autoMirror(Tools::pi/2.0), RightSide, FRUIT_2B_NODE, autoMirror(-Tools::pi/2.0), LeftSide, 400, 2, _manager));
	addCommand(new PBFruitPickupCommand(FRUIT_3A_NODE, autoMirror(Tools::pi/2.0), RightSide, FRUIT_3B_NODE, autoMirror(-Tools::pi/2.0), LeftSide, 400, 2, _manager));
	addCommand(new PBFruitPickupCommand(FRUIT_4A_NODE, autoMirror(Tools::pi), RightSide, FRUIT_4B_NODE, 0, LeftSide, 400, 2, _manager));
	addCommand(new PBFruitDropCommand(FRUIT_DROP_AREA, 2.0, _manager));
}

void PrehistobotStrategy::defaultObstaclePositions(QList<DataObject *> &objects) const
{	
	Q_UNUSED(objects);
}

void PrehistobotStrategy::microswitchChanged(const QList<int> &raisingEdge, const QList<int> &faillingEdge)
{
	Q_UNUSED(raisingEdge);
	Q_UNUSED(faillingEdge);
}

bool PrehistobotStrategy::checkGrid(const NGrid *grid) const
{
	bool result = true;

	result = grid->getNode(FRESCO_NODE) && result;

	result = grid->getNode(FRUIT_1A_NODE) && result;
	result = grid->getNode(FRUIT_1B_NODE) && result;
	result = grid->getNode(FRUIT_2A_NODE) && result;
	result = grid->getNode(FRUIT_2B_NODE) && result;
	result = grid->getNode(FRUIT_3A_NODE) && result;
	result = grid->getNode(FRUIT_3B_NODE) && result;
	result = grid->getNode(FRUIT_4A_NODE) && result;
	result = grid->getNode(FRUIT_4B_NODE) && result;

	result = grid->getArea(FRUIT_DROP_AREA) && result;

	return result;
}
