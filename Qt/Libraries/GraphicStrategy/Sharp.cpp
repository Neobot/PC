#include "Sharp.h"
#include "ToolsLib.h"

using namespace Tools;

Sharp::Sharp() : _height(0.0), _distance(0.0), _threshold(0.0)
{
}

Sharp::Sharp(const QVector2D& position, const QVector2D& direction, double height, const Curve& calibration, double threshold)
    : _position(position), _direction(direction), _height(height), _calibrationCurve(calibration), _distance(0.0), _threshold(threshold)
{
    _direction.normalize();
}

Sharp::Sharp(const Sharp& other)
    : _position(other._position), _direction(other._direction), _height(other._height),
      _calibrationCurve(other._calibrationCurve), _distance(other._distance), _threshold(other._threshold)
{
}

double Sharp::getDistance(double volt) const
{
    return _calibrationCurve.xValue(volt);
}

double Sharp::getVolt(double distance) const
{
    return _calibrationCurve.yValue(distance);
}

const QVector2D& Sharp::getPosition() const
{
    return _position;
}

const QVector2D & Sharp::getDirection() const
{
    return _direction;
}

double Sharp::getHeight() const
{
    return _height;
}

double Sharp::getDistance() const
{
    return getValue();
}

double Sharp::getValue() const
{
    return _distance;
}

void Sharp::update(double value)
{
    _distance = getDistance(value);
}

Direction Sharp::getMovementDirection() const
{
    return _direction.x() >= 0 ? Tools::Forward : Tools::Backward;
}

bool Sharp::isActive() const
{
    return _distance > 0 && (_threshold <= 0.0 || _distance < _threshold);
}

double Sharp::getActivationThreshold() const
{
    return _threshold;
}

