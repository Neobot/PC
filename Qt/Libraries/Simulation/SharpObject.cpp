#include "SharpObject.h"
#include <math.h>
#include <QtDebug>

using namespace Tools;

const double voltage_factor = 255.0 / 5.0; //255 = 5V

SharpObject::SharpObject(double range, const QPointF& relativePosition, double relativeRotation, const ZRange& zrange, const Curve& calibrationCurve)
    : LineObject(QLineF(relativePosition, relativePosition + QPointF(range * cos(relativeRotation), range * sin(relativeRotation))), zrange),
      _calibrationCurve(calibrationCurve),
      _relativePosition(relativePosition),
      _relativeRotation(relativeRotation),
      _distance(range),
      _range(range)
{}

double SharpObject::getValue()
{
    if (_distance >= _range)
	return 0.0;

    return qMin(_calibrationCurve.yValue(_distance), 255.0);
}

void SharpObject::setDistance(const double distance)
{
    _distance = qBound(0.1, distance, _range);
    setLength(_distance);
}

void SharpObject::setPositionAndRotation(const RPoint& robotPosition)
{
    double x, y, d, theta;
    d = _relativePosition.length();

    theta = robotPosition.getTheta() + atan2(_relativePosition.y(), _relativePosition.x());
    x = robotPosition.getX() + d*cos(theta);
    y = robotPosition.getY() + d*sin(theta);

    LineObject::setPosition(QPointF(x, y));
    LineObject::setRotation(_relativeRotation + robotPosition.getTheta());
}


double SharpObject::getRange() const
{
    return _range;
}

double SharpObject::getDistance() const
{
    return _distance;
}

//------------------------------------------------------------------------

Sharp4_30::Sharp4_30(const QPointF& relativePosition, double relativeRotation, double height)
    : SharpObject(301.0, relativePosition, relativeRotation, ZRange(height-13, height+13))  // 13 = 300 * tan(2,5°)
{
    _calibrationCurve << Point(50, 110)
                      << Point(100, 65)
                      << Point(150, 50)
                      << Point(200, 36)
                      << Point(350, 18)
                      << Point(500, 10)
                      << Point(10000, 2);
}

//------------------------------------------------------------------------

Sharp10_80::Sharp10_80(const QPointF& relativePosition, double relativeRotation, double height)
    : SharpObject(800.0, relativePosition, relativeRotation, ZRange(height-35, height+35))  // 35 = 800 * tan(2,5°)
{
    _calibrationCurve << Point(150, 145)
                      << Point(200, 140)
                      << Point(300, 110)
                      << Point(350, 100)
                      << Point(450, 75)
                      << Point(500, 70)
                      << Point(550, 60)
                      << Point(700, 48)
                      << Point(10000, 2);
}

//------------------------------------------------------------------------

Sharp20_150::Sharp20_150(const QPointF& relativePosition, double relativeRotation, double height)
    : SharpObject(1500.0, relativePosition, relativeRotation, ZRange(height-45, height+45))
{
    _calibrationCurve << Point(150, 145)
                      << Point(200, 140)
                      << Point(300, 110)
                      << Point(350, 100)
                      << Point(450, 75)
                      << Point(500, 70)
                      << Point(550, 60)
                      << Point(700, 48)
                      << Point(10000, 2);
}

//------------------------------------------------------------------------

MaxSonar_MB1240::MaxSonar_MB1240(const QPointF& relativePosition, double relativeRotation, double height)
		: SharpObject(2000.0, relativePosition, relativeRotation, ZRange(height-150, height+150))
{
	_calibrationCurve << Point(0, 0)
					  << Point(1000, 255);
}
