#ifndef SENSOR_H
#define SENSOR_H

class Sensor
{
public:
    enum SensorFamily
    {
        AvoidingSharpSensorFamily,
        OtherSharpSensorFamily,
        ColorSensorFamily
    };

    enum ThresholdTestOperationFlag
    {
        GreaterThan = 1,
        LesserThan = 2,
        EqualTo = 4
    };

    Sensor();
    virtual ~Sensor();

    virtual double getValue() const = 0;

    bool testThreshold(double threshold, int operations) const;
};

#endif // SENSOR_H
