#include "DataObject.h"
#include "ToolsLib.h"
#include <cmath>
#include "vmath.h"
#include <QtDebug>

using namespace Tools;

DataObject::DataObject(const ZRange& zrange) : _type(NoType), _zRange(zrange), _collideable(true)
{
}

DataObject::DataObject(const DataObject& other) : _type(other._type), _zRange(other._zRange), _collideable(true)
{
}

DataObject::ObjectType DataObject::getType() const
{
    return _type;
}

const ZRange& DataObject::getZRange() const
{
    return _zRange;
}

bool DataObject::isCollideable() const
{
    return _collideable;
}

void DataObject::collideable()
{
    _collideable = true;
}

void DataObject::notCollideable()
{
    _collideable = false;
}


//--------------------

CircleObject::CircleObject(double radius, const QPointF& position, double rotation, const ZRange& zrange)
    : DataObject(zrange), _r(radius), _angle(rotation), _position(position)
{
    _type = Circle;
}

CircleObject::CircleObject(const CircleObject& other) : DataObject(other), _r(other._r), _angle(other._angle), _position(other._position)
{
}

void CircleObject::setRadius(double radius)
{
    _r = radius;
}

double CircleObject::getRadius() const
{
    return _r;
}

void CircleObject::setRotation(double angle)
{
    _angle = angle;
}

double CircleObject::getRotation() const
{
    return _angle;
}

QPointF CircleObject::getPosition() const
{
    return _position;
}

void CircleObject::setPosition(const QPointF& position)
{
    _position = position;
}

QRectF CircleObject::getBoundingRect() const
{
    return QRectF(_position.x() - _r, _position.y() - _r, _r*2, _r*2);
}

bool CircleObject::contains(const QPointF& point) const
{
    return QVector2D(_position - point).lengthSquared() <= square(_r);
}

//--------------------

RectangleObject::RectangleObject(const QRectF rect, const ZRange& zrange)
    : DataObject(zrange), _rect(rect), _angle(0.0)
{
    _type = Rectangle;
}

RectangleObject::RectangleObject(const RectangleObject& other) : DataObject(other), _rect(other._rect), _angle(other._angle)
{
}

const QRectF& RectangleObject::getRectangle() const
{
    return _rect;
}

void RectangleObject::setRotation(double angle)
{
    _angle = angle;
}

double RectangleObject::getRotation() const
{
    return _angle;
}

QPointF RectangleObject::getPosition() const
{
    return _rect.center();
}

void RectangleObject::setPosition(const QPointF& position)
{
    _rect.moveCenter(position);
}

QRectF RectangleObject::getBoundingRect() const
{
    return _rect;
}

bool RectangleObject::contains(const QPointF& point) const
{
    return _rect.contains(point);
}

//--------------------

LineObject::LineObject(const QLineF& line, const ZRange& zrange)
    : DataObject(zrange), _line(line)
{
    _type = Line;
}

LineObject::LineObject(const LineObject& other) : DataObject(other), _line(other._line)
{
}

const QLineF& LineObject::getLine() const
{
    return _line;
}

void LineObject::setRotation(double angle)
{
    //_line.setAngle(Tools::radianToDegree(angle));
    _line.setAngle(Tools::radianToDegree(-angle));
}

double LineObject::getRotation() const
{
    //return Tools::degreeToRadian(_line.angle());
    return Tools::degreeToRadian(-_line.angle());
}

QPointF LineObject::getPosition() const
{
    return _line.p1();
}

void LineObject::setPosition(const QPointF& position)
{
    _line.translate(position - _line.p1());
}

void LineObject::setLength(double length)
{
    _line.setLength(length);
}

void LineObject::setP2(const QPointF &position)
{
    _line.setP2(position);
}

QRectF LineObject::getBoundingRect() const
{
    return QRectF(_line.p1(), _line.p2());
}

bool LineObject::contains(const QPointF& point) const
{
    //NOT IMPLEMENTED YET
    Q_UNUSED(point);
    Q_ASSERT(false);
    return false;
}

//--------------------

TrajectoryObject::TrajectoryObject(const CircleObject* patternCircle, const Tools::Trajectory& tr, const ZRange& zrange)
    : DataObject(zrange), _pattern(patternCircle), _trajectory(tr)
{
    _type = Trajectory;
}

TrajectoryObject::TrajectoryObject(const CircleObject* patternCircle, const ZRange& zrange)
    : DataObject(zrange), _pattern(patternCircle)
{
    _type = Trajectory;
}

TrajectoryObject::TrajectoryObject(const TrajectoryObject& other)
    : DataObject(other), _pattern(other._pattern), _trajectory(other._trajectory)
{
}

TrajectoryObject::~TrajectoryObject()
{
    clear();
}

void TrajectoryObject::append(const Tools::RPoint& point)
{
    _trajectory << point;
}

void TrajectoryObject::prepend(const Tools::RPoint& point)
{
   _trajectory.prepend(point);
}

void TrajectoryObject::removeFirst()
{
    _trajectory.removeFirst();
}

void TrajectoryObject::removeLast()
{
    _trajectory.removeLast();
}

void TrajectoryObject::clear()
{
    _trajectory.clear();
}

void TrajectoryObject::setTrajectory(const Tools::Trajectory& tr)
{
    _trajectory = tr;
}

const CircleObject* TrajectoryObject::getPatternCircle() const
{
    return _pattern;
}

const Tools::Trajectory& TrajectoryObject::getTrajectory() const
{
    return _trajectory;
}

void TrajectoryObject::setRotation(double angle)
{
    Q_UNUSED(angle);
}

double TrajectoryObject::getRotation() const
{
    return 0.0;
}

QPointF TrajectoryObject::getPosition() const
{
    return _trajectory.first().toQPointF();
}

void TrajectoryObject::setPosition(const QPointF& position)
{
    Q_UNUSED(position);
    //NOT YET IMPLEMENTED

    Q_ASSERT(false);
}

QRectF TrajectoryObject::getBoundingRect() const
{
    if (_trajectory.isEmpty())
	return QRectF();

    return QRectF(_trajectory.first().toQPointF(), _trajectory.last().toQPointF());
}

bool TrajectoryObject::contains(const QPointF& point) const
{
    //NOT IMPLEMENTED YET
    Q_UNUSED(point);
    Q_ASSERT(false);
    return false;
}


