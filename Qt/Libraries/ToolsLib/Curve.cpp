#include "Curve.h"
#include "vmath.h"
#include <cmath>

using namespace Tools;

Curve::Curve() : QList<Point>() {}

Curve::Curve(const Curve& c) :  QList<Point>(c) {}

void Curve::sortPointsOnAbscissa()
{
    qSort(begin(), end(), pointsLessThan);
}

bool Curve::pointsLessThan(const Point& p1, const Point& p2)
{
    return p1.getX() < p2.getX();
}

double Curve::minYValue() const
{
    double min = 0.0;
    if (size() > 0)
    {
        min = value(0).getY();
        for(Curve::const_iterator it = constBegin(); it != constEnd(); ++it)
            min = qMin(it->getY(), min);
    }
    return min;
}

double Curve::maxYValue() const
{
    double max = 0.0;
    if (size() > 0)
    {
        max = value(0).getY();
        for(Curve::const_iterator it = constBegin(); it != constEnd(); ++it)
            max = qMax(it->getY(), max);
    }
    return max;
}

double Curve::yValue(double x) const
{
    Curve::const_iterator prevIt;
    for(Curve::const_iterator it = constBegin(); it != constEnd(); prevIt = it++)
    {
        if (it->getX() == x)
            return it->getY();

		if (it != constBegin() && ((it->getX() >= x && prevIt->getX() < x) || (it->getX() <= x && prevIt->getX() > x)))
	    return interpolateX(*prevIt, *it, x);
    }

    return 0.0;
}

double Curve::xValue(double y) const
{
    Curve::const_iterator prevIt;
    for(Curve::const_iterator it = constBegin(); it != constEnd(); prevIt = it++)
    {
	if (it->getY() == y)
	    return it->getX();

	if (it != constBegin() && ((it->getY() >= y && prevIt->getY() < y) || (it->getY() <= y && prevIt->getY() > y)))
	    return interpolateY(*prevIt, *it, y);
    }

    return 0.0;
}

QList<double> Curve::yValues(double x) const
{
    QList<double> values;
    bool found = false;
    Point p1, p2;

    Curve::const_iterator prevIt;
    for(Curve::const_iterator it = constBegin(); it != constEnd(); prevIt = it++)
    {
        if (it->getX() == x)
        {
            values << it->getY();
            found = true;
        }
		else if (it != constBegin() && ((it->getX() >= x && prevIt->getX() < x) || (it->getX() <= x && prevIt->getX() > x)))
        {       
                found = true;
                p1 = *prevIt;
                p2 = *it;
		values << interpolateX(p1, p2, x);
        }
    }

    if (!found)
        return QList<double>();

    return values;
}

QList<double> Curve::xValues(double y) const
{
    QList<double> values;
    bool found = false;
    Point p1, p2;

    Curve::const_iterator prevIt;
    for(Curve::const_iterator it = constBegin(); it != constEnd(); prevIt = it++)
    {
        if (it->getY() == y)
        {
            values << it->getX();
            found = true;
        }
		else if (it != constBegin() && ((it->getY() >= y && prevIt->getY() < y) || (it->getY() <= y && prevIt->getY() > y)))
        {
            if (it != constBegin())
            {
                found = true;
                p1 = *prevIt;
                p2 = *it;
                values << interpolateY(p1, p2, y);
            }
        }
    }

    if (!found)
        return QList<double>();

    return values;
}

double Curve::interpolateX(const Point& p1, const Point& p2, double x)
{
    if (p1.getX() == p2.getX())
        return p1.getX();

    double a = (p2.getY() - p1.getY()) / (p2.getX() - p1.getX());
    double b = p1.getY() - a * p1.getX();
    return a * x + b;
}

double Curve::interpolateY(const Point& p1, const Point& p2, double y)
{
    return interpolateX(p1.reverse(), p2.reverse(), y);
}

Trajectory Curve::toTrajectory() const
{
    Trajectory tr;
    for(Curve::const_iterator it = constBegin(); it != constEnd(); ++it)
    {
        Point p = *it;
        RPoint rp(p.getX(), p.getY());
        if (it != constEnd() - 1)
        {
            Point p2 = *(it + 1);
            rp.setTheta(atan2(p.getY() - p2.getY(), p.getX() - p2.getX()) + pi);
        }
        else if (it != constBegin())
            rp.setTheta(tr.last().getTheta());

        tr << rp;
    }

    return tr;
}

Curve Curve::bezier(const Point& P1, const Point& P2, const Point& P3, const Point& P4, int nbPoints)
{
    Curve result;
    double pas = 1/((double)nbPoints-1);
    double t;
    double T_t;

    for(int i = 0; i<nbPoints; ++i)
    {
        t = i*pas ;
        T_t = 1 - t;

        result << P1*cubic(T_t) + 3*P2*t*square(T_t) + 3*P3*(T_t)*square(t) + P4*cubic(t);
    }
    return result;
}

Curve Curve::bezier(const Point& P1, double startSpeed, double startAngle, const Point& P4, double endSpeed, double endAngle, int nbPoints)
{
    Point P2(startSpeed*cos(startAngle)/3,startSpeed*sin(startAngle)/3);
    Point P3(endSpeed*cos(endAngle)/3,endSpeed*cos(endAngle)/3);
    P2 = P1 + P2;
    P3 = P4 - P3;
    return bezier(P1, P2, P3, P4, nbPoints);
}

Trajectory::Trajectory() : QList<RPoint>() {}

Trajectory::Trajectory(const Trajectory& c) :  QList<RPoint>(c) {}

double Trajectory::distance() const
{
    double distance = 0;
    if (isEmpty())
    {
        for(Tools::Trajectory::const_iterator it = constBegin() + 1; it != constEnd(); ++it)
            distance += (it-1)->distance(*it);
    }

    return distance;
}

double Trajectory::squaredDistance() const
{
    double distance = 0;
    if (!isEmpty())
    {
        for(Tools::Trajectory::const_iterator it = constBegin() + 1; it != constEnd(); ++it)
            distance += (it-1)->squareDistance(*it);
    }

    return distance;
}

double Trajectory::manhattanDistance() const
{
    double distance = 0;
    if (!isEmpty())
    {
        for(Tools::Trajectory::const_iterator it = constBegin() + 1; it != constEnd(); ++it)
            distance += (it-1)->manhattanDistance(*it);
    }

    return distance;
}
