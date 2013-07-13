#ifndef THREEAXISARM_H
#define THREEAXISARM_H

#include "AbstractLogger.h"
#include "AX12CommManager.h"

class ThreeAxisArm
{
public:
	ThreeAxisArm(Tools::AbstractLogger* logger, Comm::AX12CommManager* commManager,
				 float l0, float l1, float l2,
				 quint8 servo1Id, float servo1MinAngle, float servo1MaxAngle,
				 quint8 servo2Id, float servo2MinAngle, float servo2MaxAngle,
				 quint8 servo3Id, float servo3MinAngle, float servo3MaxAngle);
	void moveTo(double x, double y, double z, float speed);

private:
	Tools::AbstractLogger* _logger;
	Comm::AX12CommManager* _commManager;
	float _l0, _l1, _l2;
	float _servo1MinAngle, _servo1MaxAngle;
	float _servo2MinAngle, _servo2MaxAngle;
	float _servo3MinAngle, _servo3MaxAngle;

	bool checkAngles(float servo1angle, float servo2angle, float servo3angle);
};

#endif // THREEAXISARM_H
