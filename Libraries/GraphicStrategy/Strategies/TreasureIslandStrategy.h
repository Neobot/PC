#ifndef TREASUREISLANDSTRATEGY_H
#define TREASUREISLANDSTRATEGY_H

#include "DefaultStrategy.h"
#include "AIEngine.h"

class TreasureIslandStrategy : public DefaultStrategy, public AIEngine
{
public:
    TreasureIslandStrategy(Tools::AbstractLogger* logger = 0);

    void parameters(StrategyParameters& parameters) const;
    bool initGrid(Tools::NGrid* grid) const;

    void mainStrategy(QList<AbstractAction*>& actions);
    void actionDone(const AbstractAction *action, bool result);
    void obstacleDetected();
    void blockingDeteced();

private:
    bool checkGrid() const;
    void testSpeed();
};

#endif // TREASUREISLANDSTRATEGY_H
