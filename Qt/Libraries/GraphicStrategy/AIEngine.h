#ifndef AISTRATEGY_H
#define AISTRATEGY_H

#include "GameState.h"

class AbstractAction;
class AbstractAICommand;

class AIEngine
{
public:
    enum Criteria
    {
        BestCost,
        WeightedRandom,
        WeightedRandomInBestOfThree
    };

    enum CostGoal
    {
        CostMinimizing,
        CostMaximizing
	};

	AIEngine(CostGoal costGoal);
    virtual ~AIEngine();

    AbstractAICommand *nextAtion(int nbRun, AIEngine::Criteria criteria, GameState& currentState, bool lastActionResult = true);
    AbstractAICommand* findBestAction(const QList<AbstractAICommand*> commands, const GameState& state, int nbRun, Criteria criteria) const;

    void addCommand(AbstractAICommand* command);
    void clearAndDeleteCommands();

	void notifyCancel();

private:
    CostGoal _costGoal;
    QList<AbstractAICommand*> _availableCommands;
    AbstractAICommand* _commandInProgress;
	bool _hasBeenCanceled;

    struct CommandEvaluation
    {
        QList<AbstractAICommand*> commands;
        double cost;
        double firstCost;
        GameState finalState;
    };

    QList<CommandEvaluation> evaluate(const QList<AbstractAICommand*> commands, const GameState& state, int nbRun) const;
    int evaluate(const QList<AbstractAICommand*> commands, const GameState& state, int nbRun,
                  QList<CommandEvaluation>& result, CommandEvaluation* parentEvaluation = 0) const;

    AbstractAICommand* findBestAction(const QList<CommandEvaluation>& evaluations, Criteria criteria) const;

    AbstractAICommand* getWeightedRandomCommand(const QList<CommandEvaluation>& evaluations) const;

    static bool maximizingLessThan(const CommandEvaluation& c1, const CommandEvaluation& c2);
    static bool minimizingLessThan(const CommandEvaluation& c1, const CommandEvaluation& c2);
};

#endif // AISTRATEGY_H
