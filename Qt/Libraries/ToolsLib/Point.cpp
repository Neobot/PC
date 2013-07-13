#include "Point.h"
#include "vmath.h"
#include <cmath>
#include <QtGlobal>

using namespace Tools;

Point::Point(double x, double y) : _x(x), _y(y)  {}

Point::Point(const QPointF& p) : _x(p.x()), _y(p.y())  {}

Point::Point(const Point& p)
{
    _x = p._x;
    _y = p._y;
}

double Point::getX() const
{
    return _x;
}

void Point::setX(double x)
{
    _x = x;
}

double Point::getY() const
{
    return _y;
}

void Point::setY(double y)
{
    _y = y;
}

double Point::angle(const Point &p) const
{
    return atan2(p._y - _y, p._x - _x);
}

double Point::distance(const Point &p) const
{
    double d1 = square(p._y - _y);
    double d2 = square(p._x - _x);
    double r = sqrt( d1 + d2 );
    return r;
}

double Point::squareDistance(const Point& p) const
{
    return square(p._y - _y) + square(p._x - _x);
}

double Point::manhattanDistance(const Point &p) const
{
    return qAbs(_x - p._x) + qAbs(_y - p._y);
}

double Point::manhattanLength() const
{
    return qAbs(_x) + qAbs(_y);
}

bool Point::isInCircle(const Point& p, const double& radius) const
{
    if (radius < 0)
	return false;

    return squareDistance(p) <= square(radius);
}

Point Point::reverse() const
{
    return Point(_y, _x);
}

QPointF Point::toQPointF() const
{
    return QPointF(_x, _y);
}

Point& Point::operator=(const Point& p)
{
    if (&p != this)
    {
        _x = p._x;
        _y = p._y;
    }

    return *this;
}
Point& Point::operator+=(const Point& p)
{
    _x += p._x;
    _y += p._y;
    return *this;
}
Point& Point::operator-=(const Point& p)
{
    _x -= p._x;
    _y -= p._y;
    return *this;
}
Point& Point::operator*=(const double& a)
{
    _x *= a;
    _y *= a;
    return *this;
}
Point& Point::operator/=(const double& a)
{
    _x /= a;
    _y /= a;
    return *this;
}
bool Point::operator!=(const Point& p) const
{
    return !(*this==p);
}
bool Point::operator==(const Point& p) const
{
    return _x == p._x && _y == p._y;
}

const Point Tools::operator+(const Point& p1, const Point& p2) { Point p(p1); p += p2; return p; }
const Point Tools::operator-(const Point& p1, const Point& p2) { Point p(p1); p -= p2; return p; }
const Point Tools::operator*(const Point& p1, const double& a) { Point p(p1); p *= a; return p; }
const Point Tools::operator*(const double& a, const Point& p1) { Point p(p1); p *= a; return p; }
const Point Tools::operator/(const Point& p1, const double& a) { Point p(p1); p /= a; return p; }

AbstractLogger& Tools::operator<<(AbstractLogger& logger, const Point& point)
{
    logger << "Point(" << point.getX() << ";" << point.getY() << ") ";
    return logger;
}
