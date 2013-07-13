#ifndef STRATEGYMAP_H
#define STRATEGYMAP_H

#include <DataMap.h>
#include "Sharp.h"

class StrategyMap : public Tools::DataMap
{
    Q_OBJECT

    friend class DataMapTests;

public:
    explicit StrategyMap(const QSizeF& tableSize, double robotRadius, double opponentRadius, QObject *parent = 0);
    ~StrategyMap();

    enum CollisionGroup
    {
	ObstaclesGroup,
	SharpGroup,
	OpponentGroup,
        RobotGroup,
        NoDetectionZone
    };

    void init(const QSizeF& tableSize, double robotRadius, double opponentRadius);

    void setSharpsDetectionTime(int ms);
    void setSharpObjectsRadius(double objectRadius, double overlapDetectionRadius);

    bool checkRobotPosition(const QPointF& pos, bool checkBorders = false);

    void addObstacle(Tools::DataObject* object);
    void removeObstacle(Tools::DataObject* object);

    bool robotIsInTable() const;

    bool addTemporarySharpObject(const QPointF& position);
    bool addTemporaryBlockingSharpObject(const QPointF& position);
    bool updateSharp(const Sharp& sharp, QPointF& obstaclePosition);
    QPointF getSharpDetectionPoint(const Sharp& sharp);
    QPointF getSharpDetectionPoint(const QPointF& relativePosition, const QVector2D& direction, double distance);
    void setNoDetectionZones(const QList<QRectF> &zones);

    bool checkNextTrajectoryForCollision(const Tools::RPoint& nextPoint);

private:
    Tools::CircleObject* _robotBounds; //Object used for collision tests (same size as DataMap::_robot)
    Tools::CircleObject* _testSharpObject; //Object used to test if a location is already detected by a sharp

    Tools::TrajectoryObject* _robotTraj;
    Tools::TrajectoryObject* _opponentTraj;

    QRectF _shrinkedTableRect;

    double  _sharpRadius;	    //mm - default value is 50
    int	    _sharpDetectionTime;   //ms - default value is 2000

    bool isInNoDetecionZone(const QPointF& point) const;

private slots:
    void objectRemoved(const Tools::DataObject* object, int group);
    void mapCleared();
};

#endif // STRATEGYMAP_H
