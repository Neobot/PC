#include "DataMap.h"
#include "vmath.h"
#include "math.h"
#include "ToolsLib.h"

#include <QtDebug>
#include <QTimer>

using namespace Tools;

const QPointF DEFAULT_POINT = QPointF(-1000, -1000);

DataMap::DataMap(const QSizeF& tableSize, double robotRadius, int robotGroup, double opponentRadius, int opponentGroup, QObject* parent) :
    QObject(parent), _manageObjectOwnership(true)
{
    _robot = new CircleObject(robotRadius, DEFAULT_POINT);
    _opponent = new CircleObject(opponentRadius, DEFAULT_POINT, pi/2);

    init(tableSize, robotRadius, robotGroup, opponentRadius, opponentGroup);
}

DataMap::~DataMap()
{
    removeContent();
    delete _robot;
    delete _opponent;
}

void DataMap::init(const QSizeF& tableSize, double robotRadius, int robotGroup, double opponentRadius, int opponentGroup)
{
    _tableRect = QRectF(QPointF(0, 0), tableSize);

    _robot->setRadius(robotRadius);
    _objects[_robotGroup].removeAll(_robot);
    _robotGroup = robotGroup;
    _objects[_robotGroup].append(_robot); //signal objectAdded() must not be emitted

    _opponent->setRadius(opponentRadius);
    _objects[_oppGroup].removeAll(_opponent);
    _oppGroup = opponentGroup;
    _objects[_oppGroup].append(_opponent); //signal objectAdded() must not be emitted
}

bool DataMap::isInTable(const QPointF& pos) const
{
    return _tableRect.contains(pos);
}

void DataMap::addObject(DataObject* object, int group)
{
    _objects[group].append(object);
    emit objectAdded(object, group);
}

bool DataMap::removeObject(DataObject* object, int group)
{
    bool ok = _objects[group].removeAll(object);
    emit objectRemoved(object, group);

    delete object;

    return ok;
}

bool DataMap::containsObject(DataObject* object, int group) const
{
    return _objects[group].contains(object);
}

void DataMap::moveRobot(const QPointF& pos)
{
    _robot->setPosition(pos);
}

void DataMap::moveRobot(const QPointF& pos, double angle)
{
    _robot->setPosition(pos);
    _robot->setRotation(angle);
}


void DataMap::moveRobot(const RPoint& pos)
{
    _robot->setPosition(pos.toQPointF());
    _robot->setRotation(pos.getTheta());
}

void DataMap::moveOpponent(const QPointF& pos)
{
    _opponent->setPosition(pos);
}

void DataMap::moveOpponent(const RPoint& pos)
{
    _opponent->setPosition(pos.toQPointF());
    _opponent->setRotation(pos.getTheta());
}

RPoint DataMap::getRobotPosition() const
{
    return RPoint(_robot->getPosition().x(), _robot->getPosition().y(), _robot->getRotation());
}

RPoint DataMap::getOpponentPosition() const
{
    return RPoint(_opponent->getPosition().x(), _opponent->getPosition().y(), _opponent->getRotation());
}

const CircleObject* DataMap::getRobot() const
{
    return _robot;
}

const CircleObject* DataMap::getOpponent() const
{
    return _opponent;
}

void DataMap::clear()
{
    removeContent();
    emit cleared();
}

void DataMap::clear(int group)
{
    QList<DataObject*> objectList = getObjects(group);
    foreach(DataObject* o, objectList)
	removeObject(o, group);
}

void DataMap::removeContent()
{
    moveRobot(DEFAULT_POINT);
    moveOpponent(DEFAULT_POINT);

    if (_manageObjectOwnership)
    {
	//Avoid deleting the robot and the opponent
	_objects[_robotGroup].removeAll(_robot);
	_objects[_oppGroup].removeAll(_opponent);
    }

    _temporayObjects.clear();

    for(QHash<int, QList<DataObject*> >::iterator it = _objects.begin(); it != _objects.end(); ++it)
    {
	if (_manageObjectOwnership)
	{
	    qDeleteAll(*it);
	}
	(*it).clear();
    }
    _objects.clear();   

    _objects[_robotGroup].append(_robot); //signal objectAdded() must not be emitted
    _objects[_oppGroup].append(_opponent); //signal objectAdded() must not be emitted
}

bool DataMap::collideWith(DataObject* objects, int group) const
{
    QList<DataObject*> objectsToChecks = _objects[group];
    for(QList<DataObject*>::const_iterator it = objectsToChecks.constBegin(); it != objectsToChecks.constEnd(); ++it)
    {
	if (collide(objects, *it))
	    return true;
    }

    return false;
}

bool DataMap::collideWith(DataObject* objects, int group, QList<DataObject*>& list) const
{
    bool result = false;
    QList<DataObject*> objectsToChecks = _objects[group];
    for(QList<DataObject*>::const_iterator it = objectsToChecks.constBegin(); it != objectsToChecks.constEnd(); ++it)
    {
	if (collide(objects, *it))
	{
	    list << *it;
	    result = true;
	}
    }

    return result;
}

bool DataMap::collide(DataObject* o1, DataObject* o2)
{
    if (!o1->getZRange().intersected(o2->getZRange()) || !o1->isCollideable() || !o2->isCollideable())
	return false;

    if (o1->getType() == DataObject::Circle && o2->getType() == DataObject::Circle)
	return collide(static_cast<CircleObject*>(o1), static_cast<CircleObject*>(o2));

    else if (o1->getType() == DataObject::Rectangle && o2->getType() == DataObject::Circle)
	return collide(static_cast<RectangleObject*>(o1), static_cast<CircleObject*>(o2));
    else if (o1->getType() == DataObject::Circle && o2->getType() == DataObject::Rectangle)
        return collide(static_cast<RectangleObject*>(o1), static_cast<CircleObject*>(o2));

    else if (o1->getType() == DataObject::Line && o2->getType() == DataObject::Circle)
	return collide(static_cast<LineObject*>(o1), static_cast<CircleObject*>(o2));
    else if (o1->getType() == DataObject::Circle && o2->getType() == DataObject::Line)
        return collide(static_cast<LineObject*>(o1), static_cast<CircleObject*>(o2));

    else if (o1->getType() == DataObject::Trajectory && o2->getType() == DataObject::Circle)
	return collide(static_cast<TrajectoryObject*>(o1), static_cast<CircleObject*>(o2));
    else if (o1->getType() == DataObject::Circle && o2->getType() == DataObject::Trajectory)
        return collide(static_cast<TrajectoryObject*>(o1), static_cast<CircleObject*>(o2));

    return false;
}

bool DataMap::collide(CircleObject* c1, CircleObject* c2)
{
    QPointF d = c1->getPosition() - c2->getPosition();
    double rr = c1->getRadius() + c2->getRadius();
    return  d.x() * d.x() + d.y() * d.y() <= rr * rr;
}

bool DataMap::collide(RectangleObject* r, CircleObject* c)
{
    //Does not support rotation
    const QRectF& rect = r->getRectangle();
    const QPointF& circle = c->getPosition();

    double circleDistanceX = qAbs(circle.x() - rect.center().x());
    double circleDistanceY = qAbs(circle.y() - rect.center().y());

    if (circleDistanceX > (rect.width()/2 + c->getRadius())) { return false; }
    if (circleDistanceY > (rect.height()/2 + c->getRadius())) { return false; }

    if (circleDistanceX <= (rect.width()/2)) { return true; }
    if (circleDistanceY <= (rect.height()/2)) { return true; }

    double cornerDistance_sq = (square(circleDistanceX - rect.width()/2)) +
			 (square(circleDistanceY - rect.height()/2));

    return (cornerDistance_sq <= square(c->getRadius()));
}

bool DataMap::collide(LineObject* l, CircleObject* circle)
{
    return compareLinePointDistance(QVector2D(l->getLine().p1()),
				    QVector2D(l->getLine().p2()),
				    QVector2D(circle->getPosition()),
				    circle->getRadius());
}

bool DataMap::collide(TrajectoryObject* t, CircleObject* c)
{
    if (t->getTrajectory().count() <= 1)
	return false;

    double maxDistance = c->getRadius() + t->getPatternCircle()->getRadius();
    for(Tools::Trajectory::const_iterator it = t->getTrajectory().constBegin(); it != t->getTrajectory().constEnd() - 1; ++it)
    {
	if (compareLinePointDistance(QVector2D(it->toQPointF()),
				     QVector2D((it + 1)->toQPointF()),
				     QVector2D(c->getPosition()),
				     maxDistance))
	{
	    return true;
	}
    }

    return false;
}

bool DataMap::compareLinePointDistance(const QVector2D& A, const QVector2D& B, const QVector2D& C, double distance)
{
    QVector2D AB(B - A);
    QVector2D AC(C - A);

    qreal dp = QVector2D::dotProduct(AB, AC);

    if (dp <= 0)
    {
	return AC.x() * AC.x() + AC.y() * AC.y() <= square(distance);
    }

    if (dp >= AB.lengthSquared())
    {
	QVector2D BC(C - B);
	return BC.x() * BC.x() + BC.y() * BC.y() <= square(distance);
    }

    double dup = AB.y()*C.x() - AB.x()*C.y() + A.y()*B.x() - A.x()*B.y();

    double distLinePointSquared = square(dup) / AB.lengthSquared();
    return distLinePointSquared <= square(distance);;
}

void DataMap::addTemporaryOpponent(const QPointF &pos, int ms)
{
    CircleObject* tmpOpp = new CircleObject(*_opponent);
    tmpOpp->setPosition(pos);
    addTemporaryObject(tmpOpp, _oppGroup, ms);
}

void DataMap::addTemporaryObject(DataObject *object, int group, int ms)
{
    _objects[group].append(object);
    _temporayObjects.enqueue(qMakePair(group, object));
    emit objectAdded(object, group);
    QTimer::singleShot(ms, this, SLOT(removeTempObject()));
}

void DataMap::removeTempObject()
{
    if (!_temporayObjects.isEmpty())
    {
	QPair<int, DataObject*> o = _temporayObjects.dequeue();
	_objects[o.first].removeAll(o.second);

	emit objectRemoved(o.second, o.first);
	delete o.second;
    }
}

QList<Tools::DataObject *> DataMap::getObjects(int group) const
{
    return _objects[group];
}

const QHash<int, QList<Tools::DataObject*> >& DataMap::getObjects() const
{
    return _objects;
}

const QRectF & Tools::DataMap::getTableRect() const
{
    return _tableRect;
}

void Tools::DataMap::manageObjectOwnership(bool value)
{
	_manageObjectOwnership = value;
}

int DataMap::getRobotGroup() const
{
	return _robotGroup;
}

int DataMap::getOpponentGroup() const
{
	return _oppGroup;
}
