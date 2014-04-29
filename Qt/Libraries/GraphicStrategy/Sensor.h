#ifndef SENSOR_H
#define SENSOR_H

class Sensor
{
public:
    Sensor(SensorType type);
    virtual ~Sensor();

    int getState() const;
	void setState(int state);
	
	enum SharpState
	{
		SharpNothingDetected,
		SharpObjectDetected,
		SharpObjectVeryClose,
	};
	
	enum MicroswicthState
	{
		MicroswicthOn,
		MicroswicthOff
	};
	
	enum ColorState
	{
		ColorUnknown,
		ColorRed,
		ColorGreen
		ColorBlue,
		ColorYellow,
		ColorWhite,
		ColorBlack
	};
	
	private:
		int _state = 0;
		SensorType _type;
};

#endif // SENSOR_H
