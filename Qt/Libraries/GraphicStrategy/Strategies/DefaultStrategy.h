#ifndef DEFAULTSTRATEGY_H
#define DEFAULTSTRATEGY_H

#include "StrategyInterface.h"

/**
 * @brief The DefaultStrategy defined the default paramaters and behaviours which are directly linked to the robot shape.
 * It is common to all strategies.
 */
class DefaultStrategy : public StrategyInterface
{
public:
	enum Sensors
	{
		REAR_LEFT_SONAR = 0,
		FRONT_LEFT_SONAR = 1,
		REAR_RIGHT_SONAR = 2,
		FRONT_RIGHT_SONAR = 3
	};
	
	enum ColorSensorId
	{
		BothColorSensor = 0,
		RightColorSensor = 1,
		LeftColorSensor = 2,
	};

	enum PumpId
	{
		BothPump = 0,
		LeftPump = 1,
		RightPump = 2
	};

	enum ServoId
	{
		BothServo = 0,
		LeftServo = 1,
		RightServo = 2
	};
	
	enum MicroswitchId
	{
		AllMicroswitch = 0
	};

	DefaultStrategy(const QDir& strategyDirectory, Tools::AbstractLogger* logger = 0);
	virtual ~DefaultStrategy() {}

	virtual void defaultStrategyParameters(StrategyParameters& parameters) const;

	virtual QList<QPointF> doDetection(const QMap<int, const Sharp*> &activatedSharps) const;
	virtual void activatedSensors(QMap<int, Sharp*>& sharpList) const;

private:
	QPointF doDetection(const Sharp* s1, const Sharp* s2) const;
	QPointF doDetection(const Sharp* s1) const;

};

#endif // DEFAULTSTRATEGY_H
