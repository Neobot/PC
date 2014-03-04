#include "Sensor.h"

Sensor::Sensor()
{
}

Sensor::~Sensor()
{
}

bool Sensor::testThreshold(double threshold, int operations) const
{
    double value = getValue();

    if ((operations & GreaterThan) == GreaterThan)
    {
        if (value > threshold)
            return true;
    }

    if ((operations & LesserThan) == LesserThan)
    {
        if (value < threshold)
            return true;
    }

    if ((operations & EqualTo) == EqualTo)
    {
        if (value == threshold)
            return true;
    }

    return false;
}
