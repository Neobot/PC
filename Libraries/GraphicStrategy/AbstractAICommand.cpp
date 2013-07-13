#include "AbstractAICommand.h"
#include "StrategyManager.h"

AbstractAICommand::AbstractAICommand(StrategyManager *manager) : _manager(manager), _count(0)
{
    setDescription("...");
}

AbstractAICommand::~AbstractAICommand()
{
}

int AbstractAICommand::getNumberOfCall() const
{
    return _count;
}

const QString &AbstractAICommand::getDescription()
{
    return _description;
}

void AbstractAICommand::setDescription(const QString &description)
{
    _description = description;
}

//---------------------------------------------------------------------------------------------------

WaitCommand::WaitCommand(int durationInSeconds, StrategyManager* manager)
    : AbstractAICommand(manager), _duration(durationInSeconds)
{
    setDescription("Wait " + QString::number(_duration) + " seconds");
}

double WaitCommand::evaluate(GameState &state)
{
    if (state._remainingTime <= _duration)
        return -1.0;

    state._remainingTime -= _duration;
    return 0.0;
}

void WaitCommand::updateToFinalState(GameState &state) const
{
    Q_UNUSED(state);
}

AbstractAction *WaitCommand::getAction(const GameState &state) const
{
    QList<AbstractAction*> actionList;
    actionList  << _manager->getActionFactory()->waitAction(_duration * 1000);

    return _manager->getActionFactory()->actionList(actionList);
}
