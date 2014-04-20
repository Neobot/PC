#ifndef ABSTRACTAICOMMAND_H
#define ABSTRACTAICOMMAND_H

#include "GameState.h"

class StrategyManager;
class AbstractAction;
class AIEngine;

class AbstractAICommand
{
    friend class AIEngine;

public:
    AbstractAICommand(StrategyManager* manager);
    virtual ~AbstractAICommand();

    virtual double evaluate(GameState& state) = 0;
    virtual void updateToFinalState(GameState& state) const = 0;

	virtual AbstractAction* getAction(const GameState& state) const = 0;

    virtual void start() {} //Called when the action is executed
    virtual void end() {}   //Called when the action is done

    int getNumberOfCall() const;

    const QString& getDescription();
    void setDescription(const QString& description);
protected:
    StrategyManager* _manager;
    QString _description;

	double calculateActionTime(double estimatedDistance, double estimatedAverageSpeed, double fixedTime);

private:
    int _count;
};

//-----Standard commands-----

class WaitCommand : public AbstractAICommand
{
public:
    WaitCommand(int durationInSeconds, StrategyManager* manager);

    double evaluate(GameState &state);
    void updateToFinalState(GameState &state) const;

	AbstractAction* getAction(const GameState& state) const;
private:
    int _duration;
};


#endif // ABSTRACTAICOMMAND_H
