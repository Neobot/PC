#include "Sensor.h"

Sensor::Sensor(SensorType type) : _type(type)
{
}

Sensor::~Sensor()
{
}

int Sensor::getState() const
{
	return _state;
}
void Sensor::setState(int state)
{
	_state = state;
}