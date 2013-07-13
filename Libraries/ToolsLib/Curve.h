#ifndef CURVE_H
#define CURVE_H

#include "RPoint.h"
#include <QList>

namespace Tools
{
    /**
     * \brief A Trajectory is simply a QList of Robot Point.
     * \see Point
     */
    class Trajectory : public QList<RPoint>
    {
    public:
        Trajectory();
        Trajectory(const Trajectory& c);

        double distance() const;
        double squaredDistance() const;
        double manhattanDistance() const;
    };

    /**
     * \brief A curve is simply a list of Point.
     * \see Point
     */
    class Curve : public QList<Point>
    {
    public:
        Curve();
	Curve(const Curve& c);

        /**
          * \brief Get the minimum y of the curve.
          */
        double minYValue() const;

        /**
          * \brief Get the maximum y of the curve.
          */
        double maxYValue() const;

        /**
          * \brief Get the first value of y from an x.
          * \note Faster than #yValues.
          */
        double yValue(double x) const;

	/**
	  * \brief Get the first value of x from an y.
	  * \note Faster than #xValues.
	  */
	double xValue(double y) const;

        /**
          * \brief Get the list of values of y from an x.
          * \see yValue
          */
        QList<double> yValues(double x) const;

        /**
          * \brief Get the list of values of x from an y.
          * \see yValue
          */
        QList<double> xValues(double y) const;

        /**
          * \brief Sort the points by x.
          */
        void sortPointsOnAbscissa();

        /**
          * \brief Convert this curve in a trajectory.
          *
          * At each point, the direction is set toward the next point.
          */
        Trajectory toTrajectory() const;

        static Curve bezier(const Point& P1, const Point& P2, const Point& P3, const Point& P4, int nbPoints=20);
        static Curve bezier(const Point& P1, double startSpeed, double startAngle, const Point& P4, double endSpeed, double endAngle, int nbPoints);

        /**
	  * \brief Interpolate between two points on x axis.
          */
	static double interpolateX(const Point& P1, const Point& P2, double x);

	/**
	  * \brief Interpolate between two points on y axis.
	  */
	static double interpolateY(const Point& P1, const Point& P2, double y);

    private:
        static bool pointsLessThan(const Point& p1, const Point& p2);
    };
}

#endif //CURVE_H
