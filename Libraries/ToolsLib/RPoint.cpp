#include "RPoint.h"
#include "vmath.h"
#include <cmath>
#include "ToolsLib.h"

using namespace Tools;

RPoint::RPoint(double x, double y, double theta) : Point(x, y), _theta(theta)  {}

RPoint::RPoint(const RPoint& p) : Point (p), _theta(p._theta) {}

RPoint::RPoint(const QPointF& p, double theta) : Point(p), _theta(theta) {}

bool RPoint::isNull() const
{
    return (getX()==0 && getY()==0 && getTheta()==0);
}

Point RPoint::toPoint() const
{
    return Point(getX(), getY());
}

double RPoint::getTheta() const
{
    return _theta;
}

int RPoint::getThetaDegree() const
{
    return round(radianToDegree(_theta));
}

void RPoint::setTheta(double theta)
{
    _theta = theta;
}


double RPoint::diffAngle(const RPoint& p) const
{
    return angleInMinusPiPlusPi(_theta - p._theta);
}

void RPoint::rotateBy(double theta)
{
    _theta = angleInZeroTwoPi(_theta + theta);
}

RPoint& RPoint::operator=(const RPoint& p)
{
    if (&p != this)
    {
        Point::operator=(p);
        _theta = p._theta;
    }

    return *this;
}
RPoint& RPoint::operator+=(const RPoint& p)
{
    Point::operator+=(p);
	_theta += p._theta;
    return *this;
}
RPoint& RPoint::operator-=(const RPoint& p)
{
    Point::operator-=(p);
	_theta -= p._theta;
	return *this;
}
RPoint& RPoint::operator*=(const double& a)
{
    Point::operator*=(a);
	_theta *= a;
	return *this;
}
RPoint& RPoint::operator/=(const double& a)
{
    Point::operator/=(a);
	_theta /= a;
	return *this;
}
bool RPoint::operator!=(const RPoint& p) const
{
    return !(*this==p);
}
bool RPoint::operator==(const RPoint& p) const
{
    return Point::operator==(p) && _theta == p._theta;
}

const RPoint Tools::operator+(const RPoint& p1, const RPoint& p2) { RPoint p(p1); p += p2; return p; }
const RPoint Tools::operator-(const RPoint& p1, const RPoint& p2) { RPoint p(p1); p -= p2; return p; }
const RPoint Tools::operator*(const RPoint& p1, const double& a) { RPoint p(p1); p *= a; return p; }
const RPoint Tools::operator*(const double& a, const RPoint& p1) { RPoint p(p1); p *= a; return p; }
const RPoint Tools::operator/(const RPoint& p1, const double& a) { RPoint p(p1); p /= a; return p; }

AbstractLogger& Tools::operator<<(AbstractLogger& logger, const RPoint& point)
{
    logger << "RPoint(" << point.getX() << ";" << point.getY() << ";" << point.getThetaDegree() << ") ";
    return logger;
}

RPoint Tools::RPoint::mirrored(double width) const
{
    return RPoint(*this).mirror(width);
}

RPoint& Tools::RPoint::mirror(double width)
{
    setY(width - getY());
    _theta = -_theta;
    return *this;
}
