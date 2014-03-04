#include "ColorSensorObject.h"

ColorSensorObject::ColorSensorObject(const QPointF &position) : CircleObject(1.0, position)
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

