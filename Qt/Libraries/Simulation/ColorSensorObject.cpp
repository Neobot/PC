#include "ColorSensorObject.h"
#include <math.h>


ColorSensorObject::ColorSensorObject(const QVector2D &relativePosition) : CircleObject(20.0, QPointF(0,0)), _relativePosition(relativePosition)
{
}

QColor ColorSensorObject::getColor() const
{
    return _color;
}

void ColorSensorObject::setColor(const QColor &color)
{
    _color = color;
}

void ColorSensorObject::setPositionAndRotation(const Tools::RPoint& robotPosition)
{
    double x, y, d, theta;
    d = _relativePosition.length();

    theta = robotPosition.getTheta() + atan2(_relativePosition.y(), _relativePosition.x());
    x = robotPosition.getX() + d*cos(theta);
    y = robotPosition.getY() + d*sin(theta);

    CircleObject::setPosition(QPointF(x, y));
}

