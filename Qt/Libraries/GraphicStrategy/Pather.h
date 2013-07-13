#ifndef PATHER_H
#define PATHER_H

#include "Curve.h"
#include "NGrid.h"
#include <QSize>
#include <QPointF>
#include <QRectF>
#include <QHash>
typedef QPoint Cell;

class StrategyMap;
class PatherTests;

class Pather : public Tools::LoggerInterface
{
    friend class PatherTests;

public:
    Pather(Tools::AbstractLogger* logger = 0);
    virtual ~Pather() {}

    const Tools::NGrid* getGrid() const;

    void init(StrategyMap* map, Tools::NGrid* grid, Tools::NGridNode* startNode);
    void prepareReplan(Tools::NGridNode* destination, const QList<QRectF>& forcedZones = QList<QRectF>(), bool forceUneacheable = false);
    void prepareReplan(const QString& nodeAlias, const QList<QRectF>& forcedZones = QList<QRectF>(), bool forceUneacheable = false);
    void prepareReplan(Tools::NGridArea* destinationArea, const QList<QRectF>& forcedZones = QList<QRectF>(), bool forceUneacheable = false);
    void prepareReplan(Tools::NGridArea* destinationArea, Tools::NGridNode* targetNode, const QList<QRectF>& forcedZones = QList<QRectF>(), bool forceUneacheable = false);

    void replanTrajectory(Tools::Trajectory& plannedTrajectory);

    void prepareReplan(Tools::NGridNode* start, Tools::NGridNode* destination, const QList<QRectF>& forcedZones = QList<QRectF>(), bool forceUneacheable = false);

    void setCurrentNode(Tools::NGridNode* startNode);
    void findCurrentNode(StrategyMap* map);

protected:
    Tools::NGrid* _grid;
    Tools::NGridNode* _currentNode;
    Tools::NGridArea* _destinationArea;
    StrategyMap* _map;

    virtual void preparePathFinder(Tools::NGridNode* start, Tools::NGridNode* destination, const QList<QRectF>& forcedZones = QList<QRectF>(), bool forceUneacheable = false) = 0;

    virtual void initPathFinder() = 0;

    /**
      * Makes a replan and returns the last node in the trajectory.
      */
    virtual Tools::NGridNode* doReplan(Tools::Trajectory& plannedTrajectory) const = 0;

};

#endif // PATHER_H
