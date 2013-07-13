#include "NMicropather.h"
#include <QVector3D>
#include "StrategyMap.h"
#include <QRectF>
#include <QtDebug>
#include <ToolsLib.h>

using namespace Tools;

//#define USE_CLIPING_RECTS
//#define DISPLAY_MAP_CONTENT
#define TURN_PENALTY

NMicropather::NMicropather(Tools::AbstractLogger* logger, Heuristic heuristic, double turningPenalty) :
    Pather(logger), _micropather(0), _destinationState(0), _h(heuristic), _turningPenalty(turningPenalty)
{

}

NMicropather::~NMicropather()
{
}

Tools::NGridNode* NMicropather::getNode(void* state) const
{
    return static_cast<Tools::NGridNode*>(state);
}

float NMicropather::LeastCostEstimate(void* stateStart, void* stateEnd)
{
    if (_h == None)
	return 0;

    NGridNode* startNode = getNode(stateStart);
    NGridNode* endNode = getNode(stateEnd);

    QPointF delta = startNode->getPosition() - endNode->getPosition();

    switch(_h)
    {
    case Euclidean:
        return QVector2D(delta).length();
    case Manhattan:
        return qAbs(delta.x()) + qAbs(delta.y());
    case Diagonal:
        return qMax(qAbs(delta.x()), qAbs(delta.y()));
    case None:
	return 0;
    }

    return 0;
}

void NMicropather::AdjacentCost(void* previousState, void* state, std::vector< micropather::StateCost > *adjacent)
{
    NGridNode* previousNode = getNode(previousState);
    NGridNode* node = getNode(state);

    for(QHash<NGridNode*, int>::const_iterator it = node->getReachableNodes().constBegin(); it != node->getReachableNodes().constEnd(); ++it)
    {
        NGridNode* adjNode = it.key();

        micropather::StateCost sc;
        sc.cost = it.value();
        sc.state = adjNode;

        if (sc.cost > 0)
        {
            sc.cost *= qAbs(QVector2D(node->getPosition() - it.key()->getPosition()).length());
            sc.cost *= _additionalCostFactor.value(adjNode, 1.0);
        }

        if (sc.cost <= 0.0)
        {
            sc.cost = FLT_MAX;
        }
#ifdef TURN_PENALTY
        else
        {
			if ((_turningPenalty > 0 && !previousNode) || (!_grid->nodesAreAligned(previousNode, node, adjNode)))
               sc.cost += _turningPenalty;
        }
#endif
        adjacent->push_back(sc);
    }
}

void NMicropather::PrintStateInfo(void* state)
{
    logger() << getNode(state)->getPosition().x() << "-" << getNode(state)->getPosition().y();
}

void NMicropather::initPathFinder()
{
    delete _micropather;
	_destinationState = 0;
	_startState = 0;
    int nbNodes = _grid->getNodes().count();
    _micropather = new micropather::MicroPather(this, nbNodes > 0 ? nbNodes : 1000, 8);
}

void NMicropather::preparePathFinder(Tools::NGridNode* start, Tools::NGridNode* destination, const QList<QRectF>& forcedZones, bool forceUneacheable)
{
    _additionalCostFactor.clear();
    foreach(Tools::NGridNode* node, _grid->getNodes())
    {
        if (!_map->checkRobotPosition(node->getPosition()))
            _additionalCostFactor[node] = (forceUneacheable ? 10.0 : -1.0);

        for(QList<QRectF>::const_iterator it = forcedZones.constBegin(); it != forcedZones.constEnd(); ++it)
        {
            if (it->contains(node->getPosition()) && _additionalCostFactor.contains(node))
                _additionalCostFactor[node] = 10.0;
        }
    }

    //set start & end states
    _startState = start;
    _destinationState = destination;

    logger() << "Micropather: prepare plan from " << *start << " to " << *destination << Tools::endl;
}

Tools::NGridNode* NMicropather::doReplan(Tools::Trajectory& plannedTrajectory) const
{
    if (!_startState || !_destinationState)
    {
	logger() << "Micropather: Unknow start or/and destination." << Tools::endl;
        return _startState;
    }

    if (_startState == _destinationState)
    {
        plannedTrajectory << Tools::RPoint(_destinationState->getPosition().x(), _destinationState->getPosition().y());
        return _startState;
    }

    Q_ASSERT(_micropather);
    Q_ASSERT(_destinationState);

    _micropather->Reset();
    std::vector< void* > path;
    float totalCost;
    int result = _micropather->Solve(_startState, _destinationState, &path, &totalCost );
    switch(result)
    {
    case micropather::MicroPather::SOLVED:
	logger() << "Micropather: solved. Total cost is " << totalCost << Tools::endl;
	break;
    case micropather::MicroPather::START_END_SAME:
	logger() << "Micropather: start and end are the same." << Tools::endl;
	break;
    default: //case micropather::MicroPather::NO_SOLUTION:
	logger() << "Micropather: no solution" << Tools::endl;
    }

    Tools::NGridNode* reachedNode = _startState;
    if (path.size() > 1)
    {
	Tools::Curve curve;

	//The first point is removed as it is the start point
	for(std::vector< void* >::const_iterator it = path.begin(); it != path.end(); ++it)
	{
            NGridNode* node = getNode(*it);
            curve.append(Tools::Point(node->getPosition().x(), node->getPosition().y()));
	}

	plannedTrajectory = curve.toTrajectory();
        reachedNode = getNode(*(path.end() - 1));
    }

    return reachedNode;
}
