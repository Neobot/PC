#ifndef SHARP_H
#define SHARP_H

#include "Curve.h"
#include "RMovement.h"
#include <QVector2D>


class Sharp
{
public:
    Sharp();
    Sharp(const QVector2D& position, const QVector2D& direction, double height, const Tools::Curve& calibration, double threshold = 0.0);
    Sharp(const Sharp& other);

    double getDistance(double volt) const;
    double getVolt(double distance) const;

    const QVector2D& getPosition() const;
    const QVector2D& getDirection() const;
    double getHeight() const;

    void update(double value);
    double getDistance() const;
    double getValue() const;

    Tools::Direction getMovementDirection() const;

    bool isActive() const;
    double getActivationThreshold() const;

	void setThresholdValue();
	void setDistance(double distance);

protected:
    QVector2D _position; //position relative to the robot
    QVector2D _direction;
    double _height;
    Tools::Curve _calibrationCurve; //Curve: Volt on Y axis and Distance on X axis

    double _distance;  //last calculated distance
    double _threshold;
};

#endif // SHARP_H
