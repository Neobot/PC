#include "ThreeAxisArm.h"
#include <cmath>
#include "vmath.h"
#include "ToolsLib.h"

using namespace Tools;
using namespace std;

ThreeAxisArm::ThreeAxisArm(AbstractLogger* logger, Comm::AX12CommManager* commManager,
						   float l0, float l1, float l2,
						   quint8 servo1Id, float servo1MinAngle, float servo1MaxAngle,
						   quint8 servo2Id, float servo2MinAngle, float servo2MaxAngle,
						   quint8 servo3Id, float servo3MinAngle, float servo3MaxAngle) :
	_logger(logger), _commManager(commManager), _l0(l0), _l1(l1), _l2(l2),
	_servo1MinAngle(servo1MinAngle), _servo1MaxAngle(servo1MaxAngle),
	_servo2MinAngle(servo2MinAngle), _servo2MaxAngle(servo2MaxAngle),
	_servo3MinAngle(servo3MinAngle), _servo3MaxAngle(servo3MaxAngle)
{
	_commManager->addServo(servo1Id, servo1MinAngle, servo1MaxAngle);
	_commManager->addServo(servo2Id, servo2MinAngle, servo2MaxAngle);
	_commManager->addServo(servo3Id, servo3MinAngle, servo3MaxAngle);
}

bool ThreeAxisArm::checkAngles(float servo1angle, float servo2angle, float servo3angle)
{
	return !(isnan(servo1angle) || isnan(servo2angle) || isnan(servo3angle) ||
			servo1angle < _servo1MinAngle || servo1angle > _servo1MaxAngle ||
			servo2angle < _servo2MinAngle || servo2angle > _servo2MaxAngle ||
			servo3angle < _servo3MinAngle || servo3angle > _servo3MaxAngle);
}

void ThreeAxisArm::moveTo(double x, double y, double z, float speed)
{
	double servo1goal, servo2goal, servo3goal;

	double c2, s2, k1, k2, x1;
	double theta1, theta2, theta3;

	theta1 = atan2(y, x);
	x1 = x/cos(theta1);

	c2 = (square(x1) + square(z) - square(_l1) - square(_l2)) / (2*_l1*_l2);
	s2 = sqrt(1 - square(c2));

	k1 = _l1 + _l2*c2;
	k2 = _l2*s2;

	theta2 = atan2(z, x1) - atan2(k2, k1);
	theta3 = atan2(s2, c2);

	servo1goal = radianToDegree(angleInZeroTwoPi(theta1 + 2.62));
	servo2goal = radianToDegree(angleInZeroTwoPi(theta2 + 1.05));
	servo3goal = radianToDegree(angleInZeroTwoPi(theta3 + 2.62));

	*_logger << "c2=" << c2 << ", s2=" << s2 << ", k1=" << k1 << ", k2=" << k2 << Tools::endl;
	*_logger << "theta1=" << theta1 << ", theta2=" << theta2 << ", theta3=" << theta3 << Tools::endl;
	*_logger << "Calculated servos angles: " << servo1goal << ", " << servo2goal << ", " << servo3goal << Tools::endl;

	if (!checkAngles(servo1goal, servo2goal, servo3goal))
	{
		*_logger << "Destination not reachable!" << Tools::endl;

		return;
	}

	_commManager->setGoal(1, servo1goal, speed, 100.0, false);
	_commManager->setGoal(2, servo2goal, speed, 100.0, false);
	_commManager->setGoal(3, servo3goal, speed, 100.0, false);

	_commManager->synchronize();
}
