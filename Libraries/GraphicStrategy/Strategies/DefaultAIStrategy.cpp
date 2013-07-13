#include "DefaultAIStrategy.h"

#include "GameState.h"
#include "AbstractAICommand.h"

#include <StrategyManager.h>
#include <StrategyMap.h>
#include <SharpFactory.h>
#include <vmath.h>
#include <cmath>

#include <QTime>

DefaultAIStrategy::DefaultAIStrategy(AIEngine::CostGoal costGoal, const QDir &strategyDirectory, Tools::AbstractLogger *logger)
	: DefaultStrategy(strategyDirectory, logger), AIEngine(costGoal), _nbRun(2)
{
}

void DefaultAIStrategy::mainStrategy(QList<AbstractAction *> &actions)
{
	initGameState(_manager->getCurrentState());
	clearAndDeleteCommands();
	createActions();

	AbstractAICommand* c = nextAtion(_nbRun, AIEngine::BestCost, _manager->getCurrentState());
	if (c)
	{
		actions << c->getAction(_manager->getCurrentState());
		logger() << "Command: \"" << c->getDescription() << "\"" << Tools::endl;
	}
}

void DefaultAIStrategy::actionDone(const AbstractAction *action, bool result, bool isLast)
{
	Q_UNUSED(action);
	if (isLast)
		addNextAction(result);
}

void DefaultAIStrategy::obstacleDetected()
{
	if (_manager->movementInProgress())
	{
		_manager->cancelCurrentAction();
		logger() << "OBSTACLE detected: Cancelling current action." << Tools::endl;
	}
}

void DefaultAIStrategy::blockingDeteced()
{
	if (_manager->movementInProgress())
	{
		_manager->cancelCurrentAction();
		logger() << "BLOCKING detected: Cancelling current action." << Tools::endl;
	}
}

void DefaultAIStrategy::addNextAction(bool lastActionResult)
{
	AbstractAICommand* c = nextAtion(_nbRun, AIEngine::BestCost, _manager->getCurrentState(), lastActionResult);
	if (c)
	{
		_manager->addAtion(c->getAction(_manager->getCurrentState()));
		logger() << "Command: \"" << c->getDescription() << "\" - " << _manager->getCurrentState()._remainingTime << " seconds remaining." << Tools::endl;
	}
	else
	{
		logger() << "No commands to do... "   << _manager->getCurrentState()._remainingTime << " seconds remaining." << Tools::endl;
	}
}

void DefaultAIStrategy::readParametersFromFile(Tools::NSettings &settings)
{
	DefaultStrategy::readParametersFromFile(settings);

	settings.beginGroup("AI parameters");
	double fileVersion = manageParameterVersion(settings, "AI_parameters");
	Q_UNUSED(fileVersion); //unused for now

	_nbRun = settings.value("number_of_AI_run").toInt();
	settings.endGroup();
}

void DefaultAIStrategy::writeDefaultParametersToFile(Tools::NSettings &settings)
{
	DefaultStrategy::writeDefaultParametersToFile(settings);

	settings.beginGroup("AI parameters");
	settings.setValue("number_of_AI_run", _nbRun, "Number of run performed by the AI to determie the best action. Higher means more optimal results but more time consumming.");
	settings.endGroup();
}
