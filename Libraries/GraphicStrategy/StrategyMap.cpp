#include "StrategyMap.h"
#include "ToolsLib.h"
#include "vmath.h"
#include <QtDebug>

using namespace Tools;

const QPointF DEFAULT_POINT = QPointF(-1000, -1000);

StrategyMap::StrategyMap(const QSizeF& tableSize, double robotRadius, double opponentRadius, QObject *parent) :
    Tools::DataMap(tableSize, robotRadius, StrategyMap::RobotGroup, opponentRadius, StrategyMap::OpponentGroup, parent),
    _shrinkedTableRect(robotRadius, robotRadius, tableSize.width() - 2 * robotRadius, tableSize.height() - 2 * robotRadius),
    _sharpRadius(50), _sharpDetectionTime(2000)
{
    _robotBounds = new CircleObject(*getRobot());
    _testSharpObject = new CircleObject(_sharpRadius, DEFAULT_POINT);
    _robotTraj = new TrajectoryObject(_robotBounds, Trajectory());
    _opponentTraj = 0;

    connect(this, SIGNAL(cleared()), this, SLOT(mapCleared()));
    connect(this, SIGNAL(objectRemoved(const Tools::DataObject*,int)), this, SLOT(objectRemoved(const Tools::DataObject*,int)));
}

StrategyMap::~StrategyMap()
{
    delete _testSharpObject;
    delete _robotBounds;
    delete _robotTraj;
    delete _opponentTraj;
}

void StrategyMap::init(const QSizeF& tableSize, double robotRadius, double opponentRadius)
{
    DataMap::init(tableSize, robotRadius, StrategyMap::RobotGroup, opponentRadius, StrategyMap::OpponentGroup);
	_robotBounds->setRadius(robotRadius);
}

bool StrategyMap::robotIsInTable() const
{
    return _shrinkedTableRect.contains(_robot->getPosition());
}

void StrategyMap::addObstacle(DataObject* object)
{
    addObject(object, ObstaclesGroup);
}

void StrategyMap::removeObstacle(DataObject* object)
{
    removeObject(object, ObstaclesGroup);
}

bool StrategyMap::checkRobotPosition(const QPointF& pos, bool checkBorder)
{
    _robotBounds->setPosition(pos);

    bool result = ( !checkBorder || robotIsInTable()) &&
            !(collideWith(_robotBounds, OpponentGroup)
              || collideWith(_robotBounds, ObstaclesGroup)
              || collideWith(_robotBounds, SharpGroup) );

    return result;
}

bool StrategyMap::checkNextTrajectoryForCollision(const Tools::RPoint& nextPoint)
{
    Tools::Trajectory t;
    t << nextPoint;
    _robotTraj->setTrajectory(t);

    return !( collideWith(_robotTraj, OpponentGroup)
             || collideWith(_robotTraj, ObstaclesGroup)
              || collideWith(_robotTraj, SharpGroup) );
}

bool StrategyMap::updateSharp(const Sharp &sharp, QPointF& obstaclePosition)
{
    if (sharp.getDistance() <= 0)
        return false;

    obstaclePosition = getSharpDetectionPoint(sharp);

    return addTemporarySharpObject(obstaclePosition);
}

QPointF StrategyMap::getSharpDetectionPoint(const Sharp &sharp)
{
    if (sharp.getDistance() <= 0)
        return QPointF();

    return getSharpDetectionPoint(sharp.getPosition().toPointF(),
                                   sharp.getDirection(),
                                   sharp.getDistance());
}

QPointF StrategyMap::getSharpDetectionPoint(const QPointF &relativePosition, const QVector2D &direction, double distance)
{
    return Tools::relativePosition(_robot->getPosition(),
                                   relativePosition,
                                   direction,
                                   distance + _sharpRadius,
                                   _robot->getRotation());
}

void StrategyMap::setNoDetectionZones(const QList<QRectF>& zones)
{
    clear(NoDetectionZone);
    foreach(const QRectF& zone, zones)
        addObject(new RectangleObject(zone), NoDetectionZone);
}

bool StrategyMap::isInNoDetecionZone(const QPointF &point) const
{
    QList<DataObject*> zones = getObjects(NoDetectionZone);
    foreach(const DataObject* zone, zones)
    {
        if (zone->contains(point))
            return true;
    }

    return false;
}

bool StrategyMap::addTemporarySharpObject(const QPointF &position)
{
    _testSharpObject->setPosition(position);
    if (_tableRect.contains(position) && !isInNoDetecionZone(position))
    {
        if (!collideWith(_testSharpObject, SharpGroup))
        {
            addTemporaryObject(new CircleObject(_sharpRadius, position), SharpGroup, _sharpDetectionTime);
            return true;
        }
    }
    else
    {
        qDebug() << "Object outside the table " << position;
    }

    return false;
}

bool StrategyMap::addTemporaryBlockingSharpObject(const QPointF &position)
{
    _testSharpObject->setPosition(position);
    if (_tableRect.contains(position) && !isInNoDetecionZone(position))
    {
        addTemporaryObject(new CircleObject(_sharpRadius, position), SharpGroup, _sharpDetectionTime);
        return true;
    }
    else
    {
        qDebug() << "Object outside the table " << position;
    }

    return false;
}


void StrategyMap::objectRemoved(const Tools::DataObject* object, int group)
{
    Q_UNUSED(object);
    Q_UNUSED(group);
    //if (group == SharpGroup)
        //_sharpObjects.removeAll(object);
}

void StrategyMap::mapCleared()
{

}

void StrategyMap::setSharpsDetectionTime(int ms)
{
    _sharpDetectionTime = ms;
}

void StrategyMap::setSharpObjectsRadius(double objectRadius, double overlapDetectionRadius)
{
    _sharpRadius = objectRadius;
    _testSharpObject->setRadius(overlapDetectionRadius);
}



