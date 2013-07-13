#ifndef POINT_H
#define POINT_H

#include "AbstractLogger.h"
#include <QPointF>

namespace Tools
{
    /**
     * \brief This class represents a catesian Point with X et Y coordinates.
     */
    class Point
    {
    public:
        Point(double x=0, double y=0);
        Point(const Point& P);
        Point(const QPointF& P);

        double distance     (const Point& p) const;
	double squareDistance(const Point& p) const;
        double manhattanDistance(const Point& p) const;
        double angle        (const Point& p) const;
        double manhattanLength() const;

        bool   isInCircle   (const Point& p, const double& radius) const;

        double getX() const;
        void setX(double x);

        double getY() const;
        void setY(double y);

        Point reverse() const;

        Point& operator=(const Point& p);
        Point& operator+=(const Point& p);
        Point& operator-=(const Point& p);
        Point& operator*=(const double& a);
        Point& operator/=(const double& a);
        bool operator!=(const Point& p) const;
        bool operator==(const Point& p) const;

	QPointF toQPointF() const;

    private:
        double _x;
        double _y;

    };

    const Point operator+(const Point& p1, const Point& p2);
    const Point operator-(const Point& p1, const Point& p2);
    const Point operator*(const Point& p1, const double& a);
    const Point operator*(const double& a, const Point& p1);
    const Point operator/(const Point& p1, const double& a);

    AbstractLogger& operator<<(AbstractLogger& logger, const Point& point);

}
																						  
#endif // POINT_H
