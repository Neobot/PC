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

void PrehistobotStrategy::readParametersFromFile(NSettings &settings)
{
	DefaultStrategy::readParametersFromFile(settings);

	settings.beginGroup("Prehistobot Strategy");
	double fileVersion = manageParameterVersion(settings, "PB_strategy");
	Q_UNUSED(fileVersion); //unused for now

	_leftArmAX12Group = settings.value("left_arm_group").toString();
	_rightArmAX12Group = settings.value("right_arm_group").toString();

	settings.endGroup();
}

void PrehistobotStrategy::writeDefaultParametersToFile(NSettings &settings)
{
	DefaultAIStrategy::writeDefaultParametersToFile(settings);

	settings.beginGroup("Prehistobot Strategy");

	settings.setValue("left_arm_group", _leftArmAX12Group, "Left arm AX-12 group.");
	settings.setValue("right_arm_group", _rightArmAX12Group, "Left arm AX-12 group.");

	settings.endGroup();
}

void PrehistobotStrategy::initGameState(GameState &state) const
{
	state._content[FRESCO_NODE] = false;
}

void PrehistobotStrategy::createActions()
{
	//_manager->addAtion(_manager->getActionFactory()->ax12Movement(LEFT_ARM_GROUP, INIT_MOVEMENT));
	//_manager->addAtion(_manager->getActionFactory()->ax12Movement(RIGHT_ARM_GROUP, INIT_MOVEMENT));

	addCommand(new WaitCommand(1, _manager));

	addCommand(new PBFrescoCommand(FRESCO_NODE, 6, _manager));
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
	//result = grid->getArea(CONSTRUCTION_AREA) && result;

	return result;
}
