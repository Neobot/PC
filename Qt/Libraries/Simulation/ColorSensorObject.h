#ifndef COLORSENSOROBJECT_H
#define COLORSENSOROBJECT_H

#include "DataObject.h"

#include <QColor>

class ColorSensorObject : public Tools::CircleObject
{
public:
    ColorSensorObject(const QPointF& position);

    QColor getColor() const;
    void setColor(const QColor& color);

private:
    QColor _color;
};

#endif // COLORSENSOROBJECT_H
