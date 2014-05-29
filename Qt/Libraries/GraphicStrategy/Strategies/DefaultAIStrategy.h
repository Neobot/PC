#ifndef DEFAULTAISTRATEGY_H
#define DEFAULTAISTRATEGY_H

#include "DefaultStrategy.h"
#include "AIEngine.h"

/**
 * @brief The DefaultStrategy defined the default paramaters and behaviours which are directly linked to the robot shape.
 * It is common to all strategies.
 */
class DefaultAIStrategy : public DefaultStrategy, public AIEngine
{
public:
	DefaultAIStrategy(AIEngine::CostGoal costGoal, const QDir& strategyDirectory, int defaultNbRun = 2, Tools::AbstractLogger* logger = 0);
	virtual ~DefaultAIStrategy() {}

	virtual void initGameState(GameState& state) const = 0;
	virtual void createActions() = 0;

	virtual void readAndDefineParameters(Tools::NSettings &settings);

	virtual void mainStrategy(QList<AbstractAction*>& actions);
	virtual void actionDone(const AbstractAction *action, bool result, bool isLast);
	virtual void obstacleDetected();
	virtual void blockingDeteced();

protected:
	void addNextAction(bool lastActionResult = true);

private:
	int _nbRun;
};

#endif // DEFAULTAISTRATEGY_H
