#include "RMovement.h"

using namespace Tools;

RMovement::RMovement()
	: _type(Tools::NONE), _deplacement(Tools::TURN_THEN_MOVE), _isStopPoint(true)
{
	_speed = 30;
}

RMovement::RMovement(const RPoint& destination, int type, int deplacement, bool isStopPoint, int speed)
	: _destination(destination), _type(type), _deplacement(deplacement), _isStopPoint(isStopPoint), _speed(speed)
{
setSpeed(speed);
}

RMovement::RMovement(const RMovement& M)
	: _destination(M._destination), _type(M._type), _deplacement(M._deplacement), _isStopPoint(M._isStopPoint), _speed(M._speed)

{}

const RPoint& RMovement::getDestination() const
{
    return _destination;
}

int RMovement::getType() const
{
	return _type;
}

int RMovement::getDeplacement() const
{
	return _deplacement;
}

bool RMovement::isStopPoint() const
{
    return _isStopPoint;
}

int RMovement::getSpeed() const
{
	return _speed;
}

void RMovement::setDestination(RPoint& newDestination)
{
    _destination = newDestination;
}

void RMovement::setType(int newType)
{
	_type = newType;
}

void RMovement::setDeplacement(int dep)
{
	_deplacement = dep;
}

void RMovement::setSpeed(int speed)
{
	_speed = qBound(0, speed, 100);
}

bool RMovement::isRelative(Tools::Movement m)
{
    return m == AVANCE_MM || m == TOURNE_RADIAN;
}

bool RMovement::isPureRotation(Tools::Movement m)
{
    switch(m)
    {
	case TOURNE_VERS_XY:
	case TOURNE_VERS_XY_AR:
	case TOURNE_RADIAN:
	case ROTATE_TO_ABSOLUTE_ANGLE:
	    return true;
	default:
	    return false;
    }
}

Direction RMovement::getMouvementDirection(Movement m)
{
    switch(m)
    {
        case AVANT_XY:
            return Forward;
        case ARRIERE_XY:
            return Backward;
        default:
            return Unknown;
    }
}

Movement RMovement::reverseMovement(Movement m)
{
    switch(m)
    {
        case AVANT_XY:
            return ARRIERE_XY;
        case ARRIERE_XY:
            return AVANT_XY;
        default:
            return m;
    }
}
