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

void DefaultAIStrategy::readAndDefineParameters(Tools::NSettings &settings)
{
	DefaultStrategy::readAndDefineParameters(settings);

	settings.beginGroup("AI parameters");

	_nbRun = defineSettingValue(settings, "number_of_AI_run", _nbRun, "Number of run performed by the AI to determie the best action. Higher means more optimal results but more time consumming.").toInt();
	settings.endGroup();
}
