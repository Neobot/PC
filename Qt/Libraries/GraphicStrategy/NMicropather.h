#ifndef MICROPATHERSTRATEGY_H
#define MICROPATHERSTRATEGY_H

#include "Pather.h"
#include "micropather.h"
#include <QHash>

class NMicropather : public Pather, public micropather::Graph
{
    friend class PatherTests;

public:
    enum Heuristic {Euclidean, Manhattan, Diagonal, None};

    NMicropather(Tools::AbstractLogger* logger = 0, Heuristic heuristic = Manhattan, double turningPenalty = 1000.0);
    ~NMicropather();

private:
    mutable micropather::MicroPather* _micropather;
    QHash<Tools::NGridNode*, double> _additionalCostFactor;


    Tools::NGridNode* _destinationState;
    Tools::NGridNode* _startState;

    Heuristic _h;
    double _turningPenalty;

    //Pather methods
    void initPathFinder();
    void preparePathFinder(Tools::NGridNode* start, Tools::NGridNode* destination, const QList<QRectF>& forcedZones = QList<QRectF>(), bool forceUneacheable = false);
    Tools::NGridNode* doReplan(Tools::Trajectory& plannedTrajectory) const;

    //Graph methods
    float LeastCostEstimate(void* stateStart, void* stateEnd);
    void AdjacentCost(void* previousState, void* state, std::vector< micropather::StateCost > *adjacent);
    void PrintStateInfo(void* state);

    Tools::NGridNode* getNode(void* state) const;
};

#endif // MICROPATHERSTRATEGY_H
