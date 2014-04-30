#include "Sensor.h"

Sensor::Sensor()
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
