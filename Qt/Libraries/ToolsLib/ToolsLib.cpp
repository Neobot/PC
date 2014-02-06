#include "ToolsLib.h"
#include "vmath.h"
#include <QtDebug>
#include <QStandardPaths>
#include <QTransform>

using namespace Tools;

const int MAX_WIDTH = 10000;
uint qHash (const QPoint& p)
{
	return qHash(p.x()) * MAX_WIDTH + qHash(p.y());
}

void Tools::relativeTrajectory(Trajectory& trajectory, const RPoint& point)
{
	for(Trajectory::iterator it = trajectory.begin(); it != trajectory.end(); ++it)
		*it += point;
}

void Tools::trajectoryStopAtDistance(Trajectory& trajectory, const RPoint& objectif, double distance)
{
	if (trajectory.isEmpty())
		return;

	double squaredDistance = square(distance);
	if (objectif.squareDistance(trajectory.first()) < squaredDistance)
	{
		trajectory.clear();
	}
	else
	{
		int i = trajectory.count() - 1;
		while(i >= 0)
		{
			if (objectif.squareDistance(trajectory.at(i)) < squaredDistance)
			{
				trajectory.removeAt(i);
			}
			--i;
		}
	}
}

void Tools::trajectoryStraightLineSmoothing(Trajectory& trajectory)
{
	if (trajectory.count() <= 2)
		return;

	for(int i = 1; i < trajectory.count() - 1;)
	{
		if (trajectory.at(i - 1).getTheta() == trajectory.at(i).getTheta())
			trajectory.removeAt(i);
		else
			++i;
	}
}

void Tools::trajectoryDiagonalSmoothing(Trajectory& trajectory, double maximumDistance)
{
	double maxDistSquared = square(maximumDistance);
	double currentStep = 0;
	double currentAngleDiff = 0;

	QList<int> pointIndexToRemove;
	int nbPoints = trajectory.count();
	for(int i = 1; i < nbPoints - 1; ++i)
	{
		const RPoint& pm1 = trajectory.at(i - 1);
		const RPoint& p = trajectory.at(i);
		const RPoint& pp1 = trajectory.at(i + 1);

		double prevDiff = pm1.diffAngle(p);
		int prevAngle = abs(radianToDegree(angleInMinusPiPlusPi(prevDiff)));

		//int nextAngle = abs(radianToDegree(angleInMinus180And180(p.diffAngle(pp1))));

		//qDebug() << p.toQPointF() << " " << prevAngle << " " << prevDiff << "currentDiff=" << currentAngleDiff;
		double d = pm1.squareDistance(pp1);

		//if (prevAngle == 45 && prevAngle == nextAngle)
		if (prevAngle == 45 && (currentAngleDiff == 0.0 || currentAngleDiff == -prevDiff))
		{
			currentAngleDiff = prevDiff;
			if (d > maxDistSquared)
			{
				//qDebug() << "Too long distance " << i << d;
				continue;
			}

			if (currentStep == 0.0)
			{
				currentStep = d;
				//qDebug() << "firstStep" << i;
			}
			else if (d != currentStep)
			{
				//qDebug() << "Change step from " << currentStep << " to " << d;
				currentStep = d;
				continue;
			}

			//qDebug() << "Remove " << i << d << p.toQPointF();
			pointIndexToRemove << i;
		}
		else
		{
			//qDebug() << "Change direction";
			currentStep = 0.0;
			currentAngleDiff = 0.0;
		}
	}

	for(int j = pointIndexToRemove.count() - 1; j >= 0; --j)
	{
		trajectory.removeAt(pointIndexToRemove.value(j));
	}
}

double Tools::degreeToRadian(double degree)
{
	return (degree * pi) / 180.0;
}

double Tools::radianToDegree(double radian)
{   
	return (radian * 180.0) / pi;
}

double Tools::angleInZeroTwoPi(double radian)
{
	while (radian < 0)
		radian += 2*pi;

	while (radian >= 2*pi)
		radian -= 2*pi;

	return radian;
}

double Tools::angleInMinusPiPlusPi(double radian)
{
	while (radian > pi)
		radian -= 2*pi;

	while (radian < -pi)
		radian += 2*pi;

	return radian;
}

void Tools::displayTrajectory(const Tools::Trajectory& t, Tools::AbstractLogger* logger)
{
	if (!t.isEmpty())
	{
		int nbOfTurn = 0;
		double currentAngle = 0;
		foreach(const RPoint& p, t)
		{
			if (p.getTheta() != currentAngle)
				nbOfTurn++;
			currentAngle = p.getTheta();
			*logger << "RPoint(" << p.getX() << " " << p.getY() << " " << p.getThetaDegree() << ")" << Tools::endl;
		}

		*logger << "Result: " << t.count() << " moves with " << nbOfTurn << " turn" << Tools::endl;
	}
	else
		*logger << "Empty trajectory" << Tools::endl;
}

QPointF Tools::relativePosition(const QPointF& centerOfRotation, const QPointF& relativePosition, const QVector2D& direction, double distance, double angle)
{
	QVector2D dir(direction);
	dir.normalize();

	QPointF r = (dir * distance).toPointF();

	QTransform m;
	m.rotateRadians(angle);

	return centerOfRotation + m.map(relativePosition + r);
}

Tools::RPoint Tools::autoMirror(const RPoint& point, bool mirror, double width)
{
	if (mirror)
		return point.mirrored(width);
	else
		return point;
}

QPointF Tools::autoMirror(const QPointF& point, bool mirror, double width)
{
	if (mirror)
		return QPointF(point.x(), width - point.y());
	else
		return point;
}


QDir Tools::getDataDirectory()
{
	QDir d;
#ifdef 	Q_OS_LINUX
	d = QDir("/etc");
	d.mkpath("NeobotData");
	d.cd("NeobotData");
#else
    QStringList locations = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
    if (!locations.isEmpty())
        d = QDir(locations.first());
    else
    {
        d.mkpath("NeobotData");
        d.cd("NeobotData");
    }
#endif

	return d;
}

