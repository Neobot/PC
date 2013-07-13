#include "AIEngine.h"
#include "AbstractAICommand.h"

#include <ctime>

AIEngine::AIEngine(CostGoal costGoal) : _costGoal(costGoal), _commandInProgress(0)
{
}

AIEngine::~AIEngine()
{
    clearAndDeleteCommands();
}

AbstractAICommand *AIEngine::nextAtion(int nbRun, AIEngine::Criteria criteria, GameState& currentState, bool lastActionResult)
{
    if (lastActionResult && _commandInProgress)
        _commandInProgress->updateToFinalState(currentState);

    if (_commandInProgress)
        _commandInProgress->end();

    _commandInProgress = findBestAction(_availableCommands, currentState, nbRun, criteria);
    if (_commandInProgress)
    {
        _commandInProgress->start();
        ++_commandInProgress->_count;
    }

    return _commandInProgress;
}

AbstractAICommand *AIEngine::findBestAction(const QList<AbstractAICommand *> commands, const GameState &state, int nbRun, AIEngine::Criteria criteria) const
{
    QList<CommandEvaluation> evaluations = evaluate(commands, state, nbRun);
    return findBestAction(evaluations, criteria);
}

void AIEngine::addCommand(AbstractAICommand *command)
{
    _availableCommands << command;
}

void AIEngine::clearAndDeleteCommands()
{
    _commandInProgress = 0;
    qDeleteAll(_availableCommands);
    _availableCommands.clear();
}

QList<AIEngine::CommandEvaluation> AIEngine::evaluate(const QList<AbstractAICommand *> commands, const GameState &state, int nbRun) const
{
    QList<CommandEvaluation> result;
    evaluate(commands, state, nbRun, result, 0);

    return result;
}

int AIEngine::evaluate(const QList<AbstractAICommand*> commands, const GameState& state, int nbRun,
                                              QList<CommandEvaluation>& result, CommandEvaluation* parentEvaluation) const
{
    int nbPossibleChildCommands = 0;
    if (!commands.isEmpty())
    {  
        foreach(AbstractAICommand* c, commands)
        {
            GameState futureState(state);
            double cost = c->evaluate(futureState);
            c->updateToFinalState(futureState);

            if (cost >= 0.0)
            {
                CommandEvaluation eval;
                eval.cost = cost;
                eval.firstCost = cost;
                eval.finalState = futureState;
                if (parentEvaluation)
                {
                    eval.cost += parentEvaluation->cost;
                    eval.firstCost = parentEvaluation->firstCost;
                    eval.commands << parentEvaluation->commands;
                }          

                eval.commands << c;
                if (nbRun <= 1 || evaluate(commands, futureState, nbRun - 1, result, &eval) == 0)
                    result << eval;

                ++nbPossibleChildCommands;
            }
        }
    }

    return nbPossibleChildCommands;
}

AbstractAICommand *AIEngine::getWeightedRandomCommand(const QList<CommandEvaluation> &evaluations) const
{
    double total = 0.0;

    foreach(const CommandEvaluation& e, evaluations)
        total += e.cost;

    total*= 100.0;

    qsrand(time(NULL));
    int r = qrand()%((int)total);

    double c = 0.0;
    foreach(const CommandEvaluation& e, evaluations)
    {
        c += e.cost * 100.0;
        if (c >= r)
            return e.commands.first();
    }

    return evaluations.first().commands.first();
}

AbstractAICommand *AIEngine::findBestAction(const QList<CommandEvaluation> &evaluations, Criteria criteria) const
{
    if (evaluations.isEmpty())
        return 0;

    QList<CommandEvaluation> evals(evaluations);
    qSort(evals.begin(), evals.end(), _costGoal == CostMaximizing ? maximizingLessThan : minimizingLessThan);

    switch(criteria)
    {
        case BestCost:
           return evals.first().commands.first();
        case WeightedRandom:
            return getWeightedRandomCommand(evaluations);
        case WeightedRandomInBestOfThree:
            QList<CommandEvaluation>::iterator bofEnd = evals.count() >= 3 ? evals.begin() + 3 : evals.end();
            QList<CommandEvaluation> bof3;
            qCopy(evals.begin(), bofEnd, bof3.begin());
            return getWeightedRandomCommand(bof3);
    }

    return 0;
}

bool AIEngine::maximizingLessThan(const CommandEvaluation &c1, const CommandEvaluation &c2)
{
    return c1.cost == c2.cost ? c1.firstCost > c2.firstCost : c1.cost > c2.cost;
}

bool AIEngine::minimizingLessThan(const CommandEvaluation &c1, const CommandEvaluation &c2)
{
    return c1.cost == c2.cost ? c1.firstCost < c2.firstCost : c1.cost < c2.cost;
}
