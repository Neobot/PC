#ifndef RPOINT_H
#define RPOINT_H

#include "Point.h"

namespace Tools
{
	/**
	 * \brief This class represents a position of the robot : its coordinates X et Y and its angle theta.
	 */
	class RPoint : public Point
	{
	public:
		RPoint(double x=0, double y=0, double theta=0);
		RPoint(const QPointF& p, double theta=0);
		RPoint(const RPoint& P);

		Point toPoint() const;
		bool isNull() const;

		double getTheta() const;
		int getThetaDegree() const;
		void setTheta(double theta);

		double diffAngle(const RPoint& p) const;
		void rotateBy(double theta);

		RPoint mirrored(double width) const;
		RPoint& mirror(double width);

		RPoint& operator=(const RPoint& p);
		RPoint& operator+=(const RPoint& p);
		RPoint& operator-=(const RPoint& p);
		RPoint& operator*=(const double& a);
		RPoint& operator/=(const double& a);
		bool operator!=(const RPoint& p) const;
		bool operator==(const RPoint& p) const;

	private:
		double _theta;
	};

	const RPoint operator+(const RPoint& p1, const RPoint& p2);
	const RPoint operator-(const RPoint& p1, const RPoint& p2);
	const RPoint operator*(const RPoint& p1, const double& a);
	const RPoint operator*(const double& a, const RPoint& p1);
	const RPoint operator/(const RPoint& p1, const double& a);

	AbstractLogger& operator<<(AbstractLogger& logger, const RPoint& point);
}

#endif // RPOINT_H
