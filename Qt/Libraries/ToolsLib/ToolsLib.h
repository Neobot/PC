#ifndef TOOLSLIB_H
#define TOOLSLIB_H

#include "Curve.h"
#include <QVector2D>
#include <QVariantList>
#include <QDir>
#include <algorithm>

uint qHash (const QPoint& p);

namespace Tools
{
	QDir getDataDirectory();

	void relativeTrajectory(Trajectory& trajectory, const RPoint& point);
	void trajectoryStraightLineSmoothing(Trajectory& trajectory);
	void trajectoryDiagonalSmoothing(Trajectory& trajectory, double maximumDistance);
	void trajectoryStopAtDistance(Trajectory& trajectory, const RPoint& objectif, double distance);


	double degreeToRadian(double degree);
	double radianToDegree(double radian);

	double angleInZeroTwoPi(double radian);
	double angleInMinusPiPlusPi(double radian);

	void displayTrajectory(const Tools::Trajectory& t, Tools::AbstractLogger* logger);

	QPointF relativePosition(const QPointF& centerOfRotation, const QPointF& relativePosition, const QVector2D& direction, double distance, double angle);

	double autoMirror(double angle, bool mirror);
	RPoint autoMirror(const RPoint& point, bool mirror, double width = 3000.0);
	QPointF autoMirror(const QPointF& point, bool mirror, double width = 3000.0);
	QRectF autoMirror(const QRectF& rect, bool mirror, double width = 3000.0);

	template<class T>
	struct QVariantCaster
	{
		QVariant operator()(const T& element) const
		{
			return QVariant::fromValue<T>(element);
		}
	};

	template<class T>
	QVariantList convertListToVariantList(const QList<T>& list)
	{
		QVariantList result;
#if QT_VERSION > 0x040800
		result.reserve(list.size());
#endif
		std::transform(list.begin(), list.end(), std::back_inserter(result), Tools::QVariantCaster<T>());

		return result;
	}

	template<class T>
	struct QVariantRevertCaster
	{
		T operator()(const QVariant& element) const
		{
			return element.value<T>();
		}
	};

	template<class T>
	QList<T> convertVariantListToList(const QVariantList& list)
	{
		QList<T> result;
#if QT_VERSION > 0x040800
		result.reserve(list.size());
#endif
		std::transform(list.begin(), list.end(), std::back_inserter(result), Tools::QVariantRevertCaster<T>());

		return result;
	}

}

#endif // TOOLSLIB_H
