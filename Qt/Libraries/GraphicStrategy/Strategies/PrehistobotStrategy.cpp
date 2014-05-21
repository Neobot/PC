#include "PrehistobotStrategy.h"
#include "PrehistobotCommands.h"
#include "GameState.h"
#include <StrategyManager.h>
#include <StrategyMap.h>
#include <SharpFactory.h>
#include"DataObject.h"
#include <vmath.h>
#include <cmath>
#include "ToolsLib.h"
#include <QPointF>

using namespace Tools;

PrehistobotStrategy::PrehistobotStrategy(const QDir& strategyDirectory, Tools::AbstractLogger *logger)
	: DefaultAIStrategy(AIEngine::CostMaximizing, strategyDirectory, 2, logger), _pbActionFactory(nullptr)
{
	LoggerInterface::logger() << "---PREHISTOBOT STRATEGY---" << Tools::endl;

	_searchFiresPoints << QPointF(650,450) << QPointF(1600,450) << QPointF(1600,2550) << QPointF(650,2550);
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
	_searchFiresPoints = convertVariantListToList<QPointF>(defineSettingValueList(settings, "search_points", Tools::convertListToVariantList<QPointF>(_searchFiresPoints),
							QVariant::PointF, "A trajectory to search fires on the table at the end of the match.").toList());
	settings.endGroup();

	settings.beginGroup("Prehistobot AX-12 Groups");

	_ax12MvtNames.leftArmGroup = defineSettingValue(settings, "left_arm_group", _ax12MvtNames.leftArmGroup, "Left arm AX-12 group.").toString();
	_ax12MvtNames.rightArmGroup = defineSettingValue(settings, "right_arm_group", _ax12MvtNames.rightArmGroup, "Left arm AX-12 group.").toString();

	settings.endGroup();

	settings.beginGroup("Prehistobot AX-12 Movements");

	_ax12MvtNames.goToRest = defineSettingValue(settings, "go_to_rest", _ax12MvtNames.goToRest, "Move the arm to the default position.").toString();
	_ax12MvtNames.goToScan = defineSettingValue(settings, "go_to_scan", _ax12MvtNames.goToScan, "Move the arm to the scan position.").toString();
	_ax12MvtNames.pump = defineSettingValue(settings, "pump", _ax12MvtNames.pump, "Moves a bit the arm after a scan to catch a fire.").toString();
	_ax12MvtNames.moveFire = defineSettingValue(settings, "move_fire", _ax12MvtNames.moveFire, "Move the fire without turning it.").toString();
	_ax12MvtNames.turnFire = defineSettingValue(settings, "turn_fire", _ax12MvtNames.turnFire, "Turn the fire.").toString();
	_ax12MvtNames.holdFire = defineSettingValue(settings, "hold_fire", _ax12MvtNames.holdFire, "Hold the fire.").toString();
	_ax12MvtNames.scanInMobileTorche = defineSettingValue(settings, "go_to_scan_in_mobile_torche", _ax12MvtNames.scanInMobileTorche, "Go to the scan position in a mobile torche.").toString();
	_ax12MvtNames.moveOutOfMobileTorche = defineSettingValue(settings, "move_out_of_mobile_torche", _ax12MvtNames.moveOutOfMobileTorche, "Move the arm out of the mobile torche.").toString();
	_ax12MvtNames.dropFire = defineSettingValue(settings, "drop_fire", _ax12MvtNames.dropFire, "Turn the fire without turning it.").toString();
	_ax12MvtNames.dropAndTurnFire = defineSettingValue(settings, "drop_and_turn_fire", _ax12MvtNames.dropAndTurnFire, "Drop the fire while turning it.").toString();
	_ax12MvtNames.scanInFixedTorche = defineSettingValue(settings, "go_to_scan_in_fixed_torche", _ax12MvtNames.scanInFixedTorche, "Go to the scan position in a fixed torche.").toString();
	_ax12MvtNames.moveOutOfFixedTorche = defineSettingValue(settings, "move_out_of_fixed_torche", _ax12MvtNames.moveOutOfFixedTorche, "Move the arm out of the fixed torche.").toString();

	settings.endGroup();
}

void PrehistobotStrategy::mainStrategy(QList<AbstractAction *> &actions)
{
	_pbActionFactory = new PBActionFactory(_manager->getActionFactory(), _ax12MvtNames, _manager->isMirrored());

	actions << _manager->getActionFactory()->asynchroneActionList(
	{
		_manager->getActionFactory()->ax12Movement(_ax12MvtNames.leftArmGroup, _ax12MvtNames.goToRest),
		_manager->getActionFactory()->ax12Movement(_ax12MvtNames.rightArmGroup, _ax12MvtNames.goToRest)
	}, AsynchroneActionGroup::AllActionFinished);

	DefaultAIStrategy::mainStrategy(actions);
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

	state._content[TORCHE_1_NODE] = false;
	state._content[TORCHE_2_NODE] = false;
	state._content[TORCHE_3_NODE] = false;
	state._content[TORCHE_4_NODE] = false;
	state._content[TORCHE_5_NODE] = false;
	state._content[TORCHE_6_NODE] = false;

	state._content[EASYFIRE_1A_NODE] = false;
	state._content[EASYFIRE_2A_NODE] = false;
	state._content[EASYFIRE_3A_NODE] = false;
	
	state._content[LEFT_HAND_HAS_FIRE] = false;
	state._content[RIGHT_HAND_HAS_FIRE] = false;
	
	state._content[HEARTH_1_NODE] = 0;
}

void PrehistobotStrategy::createActions()
{
	addCommand(new WaitCommand(1, _manager));

	addCommand(new PBFrescoCommand(FRESCO_NODE, 6, _manager));

	addCommand(new PBFruitPickupCommand(FRUIT_1A_NODE, 0, RightSide, FRUIT_1B_NODE, autoMirror(Tools::pi), LeftSide, 400, 2, _manager));
	addCommand(new PBFruitPickupCommand(FRUIT_2A_NODE, autoMirror(Tools::pi/2.0), RightSide, FRUIT_2B_NODE, autoMirror(-Tools::pi/2.0), LeftSide, 400, 2, _manager));
	addCommand(new PBFruitPickupCommand(FRUIT_3A_NODE, autoMirror(Tools::pi/2.0), RightSide, FRUIT_3B_NODE, autoMirror(-Tools::pi/2.0), LeftSide, 400, 2, _manager));
	addCommand(new PBFruitPickupCommand(FRUIT_4A_NODE, autoMirror(Tools::pi), RightSide, FRUIT_4B_NODE, 0, LeftSide, 400, 2, _manager));
	addCommand(new PBFruitDropCommand(FRUIT_DROP_AREA, 2.0, _manager));

	addCommand(new PBTakeFixedTorcheCommand(TORCHE_3_NODE, true, 3, true, _pbActionFactory, _manager));
	addCommand(new PBTakeFixedTorcheCommand(TORCHE_4_NODE, false, 3, false, _pbActionFactory, _manager));
	addCommand(new PBTakeFixedTorcheCommand(TORCHE_5_NODE, false, 3, true, _pbActionFactory, _manager));
	addCommand(new PBTakeFixedTorcheCommand(TORCHE_6_NODE, true, 3, false, _pbActionFactory, _manager));
	
	//addCommand(new PBTakeMobileTorcheCommand(TORCHE_1_NODE, 10, _pbActionFactory, _manager));
	
	//addCommand(new PBDropHeldFiresCommand(HEARTH_1_NODE, true, 4, _pbActionFactory, _manager));

	addCommand(new PBEasyFireCommand(EASYFIRE_1A_NODE, EASYFIRE_1B_NODE, 20, 1, _pbActionFactory, _manager));
	addCommand(new PBEasyFireCommand(EASYFIRE_2A_NODE, EASYFIRE_3B_NODE, 20, 1, _pbActionFactory, _manager));
	addCommand(new PBEasyFireCommand(EASYFIRE_2A_NODE, EASYFIRE_3B_NODE, 20, 1, _pbActionFactory, _manager));

	addCommand(new PBSearchFiresCommand(autoMirrorList(_searchFiresPoints), _pbActionFactory, _manager));
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

	result = grid->getNode(TORCHE_1_NODE) && result;
	result = grid->getNode(TORCHE_2_NODE) && result;
	result = grid->getNode(TORCHE_3_NODE) && result;
	result = grid->getNode(TORCHE_4_NODE) && result;
	result = grid->getNode(TORCHE_5_NODE) && result;
	result = grid->getNode(TORCHE_6_NODE) && result;

	result = grid->getNode(EASYFIRE_1A_NODE) && result;
	result = grid->getNode(EASYFIRE_1B_NODE) && result;
	result = grid->getNode(EASYFIRE_2A_NODE) && result;
	result = grid->getNode(EASYFIRE_2B_NODE) && result;
	result = grid->getNode(EASYFIRE_3A_NODE) && result;
	result = grid->getNode(EASYFIRE_3B_NODE) && result;
	
	result = grid->getNode(HEARTH_1_NODE) && result;

	return result;
}

QList<QPointF> PrehistobotStrategy::autoMirrorList(const QList<QPointF> &points)
{
	QList<QPointF> results;
	for(QList<QPointF>::const_iterator it = points.constBegin(); it != points.constEnd(); ++it)
	{
		const QPointF& p = *it;
		results << autoMirror(Tools::RPoint(p)).toQPointF();
	}

	return results;
}

//----------------------------------------------------PBActionFactory----------------------------------------------------

AbstractAction *PBActionFactory::scanAndTurnFires(NGridNode *destination, int speed, int timeout) const
{
	AbstractAction* startAction = _factory->asynchroneActionList({
														_factory->enableColorSensorAction(DefaultStrategy::BothColorSensor),
														_factory->ax12Movement(_mvt.leftArmGroup, _mvt.goToScan),
														_factory->ax12Movement(_mvt.rightArmGroup, _mvt.goToScan)}, AsynchroneActionGroup::AllActionFinished);

	AbstractAction* endAction = _factory->asynchroneActionList({
														_factory->disableColorSensorAction(DefaultStrategy::BothColorSensor),
														_factory->ax12Movement(_mvt.leftArmGroup, _mvt.goToRest),
														_factory->ax12Movement(_mvt.rightArmGroup, _mvt.goToRest)}, AsynchroneActionGroup::AllActionFinished);

	return defaultColorScanAction(_reversed, destination, speed, timeout,
								startAction,
								hanfleFireAction(true, false, _mvt.goToScan), hanfleFireAction(false, false, _mvt.goToScan),
								hanfleFireAction(true, true, _mvt.goToScan), hanfleFireAction(false, true, _mvt.goToScan),
								endAction);

}

AbstractAction *PBActionFactory::scanAndHoldFires(NGridNode *destination, int speed, int timeout) const
{
	AbstractAction* startAction = _factory->asynchroneActionList({
														_factory->enableColorSensorAction(DefaultStrategy::BothColorSensor),
														_factory->ax12Movement(_mvt.leftArmGroup, _mvt.goToScan),
														_factory->ax12Movement(_mvt.rightArmGroup, _mvt.goToScan)}, AsynchroneActionGroup::AllActionFinished);

	AbstractAction* endAction = _factory->asynchroneActionList({
														_factory->disableColorSensorAction(DefaultStrategy::BothColorSensor),
														_factory->ax12Movement(_mvt.leftArmGroup, _mvt.goToRest),
														_factory->ax12Movement(_mvt.rightArmGroup, _mvt.goToRest)}, AsynchroneActionGroup::AllActionFinished);

	return defaultColorScanAction(false, destination, speed, timeout,
								startAction,
								holdFireAction(true), holdFireAction(false),
								holdFireAction(true), holdFireAction(false),
								  endAction);
}

AbstractAction *PBActionFactory::dropHeldFires(DefaultStrategy::PumpId pumpId)
{
	int ourColor = getOurColor(_reversed);
	int oppColor = getOpponentColor(_reversed);

	QList<AbstractAction*> actions = {_factory->enableColorSensorAction(DefaultStrategy::BothColorSensor),
									  _factory->waitAction(1000)}; //wait to gather updated color info

	if (pumpId == DefaultStrategy::LeftPump || pumpId == DefaultStrategy::BothPump)
	{
		SensorSwitchCaseAction* leftAction = _factory->colorSensorSwitchCaseAction(DefaultStrategy::LeftColorSensor);
		leftAction->addCase(ourColor, dropFireAction(true, true));
		leftAction->addCase(oppColor, dropFireAction(true, false));

		actions << leftAction;
	}

	if (pumpId == DefaultStrategy::RightPump || pumpId == DefaultStrategy::BothPump)
	{
		SensorSwitchCaseAction* rightAction = _factory->colorSensorSwitchCaseAction(DefaultStrategy::LeftColorSensor);
		rightAction->addCase(ourColor, dropFireAction(false, true));
		rightAction->addCase(oppColor, dropFireAction(false, false));

		actions << rightAction;
	}

	return _factory->asynchroneActionList(actions, AsynchroneActionGroup::AllActionFinished);
}

AbstractAction *PBActionFactory::takeAndHoldFireInMobileTorche(DefaultStrategy::PumpId pumpId)
{
	bool left = pumpId == DefaultStrategy::LeftPump;
	QString group = left ? _mvt.leftArmGroup : _mvt.rightArmGroup;

	return _factory->actionList({
									_factory->ax12Movement(group, _mvt.scanInMobileTorche),
									_factory->ax12Movement(group, _mvt.pump),
									_factory->startPumpAction(pumpId),
									_factory->waitAction(1000),
									_factory->ax12Movement(group, _mvt.moveOutOfMobileTorche)});
}

AbstractAction *PBActionFactory::takeAndHoldFireInFixedTorche(DefaultStrategy::PumpId pumpId)
{
	bool left = pumpId == DefaultStrategy::LeftPump;
	QString group = left ? _mvt.leftArmGroup : _mvt.rightArmGroup;

	return _factory->actionList({
									_factory->ax12Movement(group, _mvt.scanInFixedTorche),
									_factory->ax12Movement(group, _mvt.pump),
									_factory->startPumpAction(pumpId),
									_factory->waitAction(1000),
									_factory->ax12Movement(group, _mvt.moveOutOfFixedTorche)});
}

ActionGroup *PBActionFactory::hanfleFireAction(bool left, bool ourColor, const QString& finalMvt) const
{
	QString group = left ? _mvt.leftArmGroup : _mvt.rightArmGroup;
	int pumpId = left ? DefaultStrategy::LeftPump : DefaultStrategy::RightPump;
	QString movement = ourColor ? _mvt.moveFire : _mvt.turnFire;

	return _factory->actionList({
					_factory->ax12Movement(group, _mvt.pump),
					_factory->startPumpAction(pumpId),
					_factory->waitAction(1000),
					_factory->ax12Movement(group, movement),
					_factory->stopPumpAction(pumpId),
					_factory->waitAction(1000),
					_factory->ax12Movement(group, finalMvt)});
}

ActionGroup *PBActionFactory::holdFireAction(bool left) const
{
	QString group = left ? _mvt.leftArmGroup : _mvt.rightArmGroup;
	int pumpId = left ? DefaultStrategy::LeftPump : DefaultStrategy::RightPump;
	int sensorId = left ? DefaultStrategy::LeftColorSensor : DefaultStrategy::RightColorSensor;

	return _factory->actionList({
					_factory->disableColorSensorAction(sensorId),
					_factory->ax12Movement(group, _mvt.pump),
					_factory->startPumpAction(pumpId),
					_factory->waitAction(1000),
					_factory->ax12Movement(group, _mvt.holdFire)});
}

ActionGroup *PBActionFactory::dropFireAction(bool left, bool ourColor) const
{
	QString group = left ? _mvt.leftArmGroup : _mvt.rightArmGroup;
	QString movement = ourColor ? _mvt.dropFire : _mvt.dropAndTurnFire;

	return _factory->actionList({_factory->ax12Movement(group, movement),
								 _factory->stopPumpAction(DefaultStrategy::LeftPump),
								 _factory->waitAction(1000)});
}

AbstractAction *PBActionFactory::defaultColorScanAction(bool reversed, NGridNode *destination, int speed, int timeoutMs, AbstractAction *startAction, AbstractAction *leftOpponentColorAction,
														AbstractAction *rightOpponentColorAction, AbstractAction *leftOurColorAction, AbstractAction *rightOurColorAction,
														AbstractAction *endAction) const
{
	return _factory->colorScanAction(destination, speed, timeoutMs,
								getOurColor(reversed),
								getOpponentColor(reversed),
								DefaultStrategy::LeftColorSensor, DefaultStrategy::RightColorSensor,
								startAction,
								leftOpponentColorAction, rightOpponentColorAction,
								leftOurColorAction, rightOurColorAction,
									 endAction);
}

int PBActionFactory::getOurColor(bool reversed) const
{
	return reversed ? Comm::ColorYellow : Comm::ColorRed;
}

int PBActionFactory::getOpponentColor(bool reversed) const
{
	return reversed ? Comm::ColorRed : Comm::ColorYellow;
}
