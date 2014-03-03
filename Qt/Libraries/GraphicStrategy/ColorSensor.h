#ifndef COLORSENSOR_H
#define COLORSENSOR_H

#include "Sensor.h"

class ColorSensor : public Sensor
{
public:
    ColorSensor();

    double getValue() const;
};

#endif // COLORSENSOR_H
