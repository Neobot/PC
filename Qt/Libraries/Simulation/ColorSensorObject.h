#ifndef COLORSENSOROBJECT_H
#define COLORSENSOROBJECT_H

#include "DataObject.h"

#include <QColor>
#include <QVector2D>

class ColorSensorObject : public Tools::CircleObject
{
public:
    ColorSensorObject(const QVector2D& relativePosition);

    QColor getColor() const;
    void setColor(const QColor& color);

    QPointF getRelativePosition() const;

    void setPositionAndRotation(const Tools::RPoint &robotPosition);

private:
    QVector2D _relativePosition;
    QColor _color;
};

#endif // COLORSENSOROBJECT_H
