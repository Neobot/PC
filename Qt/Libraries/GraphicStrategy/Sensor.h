#ifndef SENSOR_H
#define SENSOR_H

#include "Instructions.h"

class Sensor
{
public:
	Sensor();
    virtual ~Sensor();

    int getState() const;
	void setState(int state);
	
	private:
		int _state = 0;
};

#endif // SENSOR_H
