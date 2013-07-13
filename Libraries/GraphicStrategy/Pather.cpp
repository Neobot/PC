#include "Pather.h"
#include "StrategyMap.h"
#include <vmath.h>
#include "math.h"

Pather::Pather(Tools::AbstractLogger* loggerObject)
    : LoggerInterface(loggerObject)
{
    setCurrentNode(0);
    _destinationArea = 0;
}

void Pather::init(StrategyMap* map, Tools::NGrid* grid, Tools::NGridNode *startNode)
{
    _map = map;
    _grid = grid;
	_destinationArea = 0;
    setCurrentNode(startNode);
    initPathFinder();
}

void Pather::prepareReplan(Tools::NGridNode* destination, const QList<QRectF>& forcedZones, bool forceUneacheable)
{
    if (!_currentNode)
    {
        logger() << "The current node is not set." << Tools::endl;
        return;
    }

    prepareReplan(_currentNode, destination, forcedZones, forceUneacheable);
}

void Pather::prepareReplan(const QString& nodeAlias, const QList<QRectF>& forcedZones, bool forceUneacheable)
{
    Tools::NGridNode* destination = _grid->getNode(nodeAlias);
    if (destination)
        prepareReplan(destination, forcedZones, forceUneacheable);
    else
        logger() << "The node \"" << nodeAlias << "\" doesn't exist" << Tools::endl;
}

void Pather::prepareReplan(Tools::NGridArea *destinationArea, const QList<QRectF> &forcedZones, bool forceUneacheable)
{
   Tools::NGridNode* destination = destinationArea->getCentralNode();
   prepareReplan(destinationArea, destination, forcedZones, forceUneacheable);
}

void Pather::prepareReplan(Tools::NGridArea *destinationArea, Tools::NGridNode *targetNode, const QList<QRectF> &forcedZones, bool forceUneacheable)
{
    prepareReplan(targetNode, forcedZones, forceUneacheable);
    _destinationArea = destinationArea;
}

void Pather::prepareReplan(Tools::NGridNode *start, Tools::NGridNode *destination, const QList<QRectF> &forcedZones, bool forceUneacheable)
{
    _destinationArea = 0;
    preparePathFinder(start, destination, forcedZones, forceUneacheable);
}

const Tools::NGrid * Pather::getGrid() const
{
    return _grid;
}

void Pather::setCurrentNode(Tools::NGridNode *startNode)
{
    _currentNode = startNode;
}

void Pather::findCurrentNode(StrategyMap *map)
{
    _currentNode = _grid->getNearestNode(map->getRobot()->getPosition());
}

void Pather::replanTrajectory(Tools::Trajectory &plannedTrajectory)
{
    _currentNode = doReplan(plannedTrajectory);

    //Remove point inside the destination area
    if (_destinationArea && plannedTrajectory.count() > 1)
    {
        for(Tools::Trajectory::iterator it = plannedTrajectory.begin(); it != plannedTrajectory.end() - 1; ++it)
        {
            if (_destinationArea->contains(it->toQPointF()))
            {
                plannedTrajectory.erase(it + 1, plannedTrajectory.end());
                break;
            }
        }
    }

    _destinationArea = 0;
}
