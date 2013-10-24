#include "Hexapod.h"
#include <cmath>
#include <QList>
#include "vmath.h"
#include "ToolsLib.h"

#define PI_SUR_6 0.5235987756
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

using namespace Tools;
using namespace std;

Leg::Leg(AbstractLogger* logger, Comm::AX12CommManager* commManager,
		 Comm::AX12 *coxaServo, Comm::AX12 *femurServo, Comm::AX12 *tibiaServo,
		 double coxaLength, double femurLength, double tibiaLength,
		 double relativeCoxaPosX, double relativeCoxaPosY, double relativeCoxaPosZ,
		 double initFootPosX, double initFootPosY, double initFootPosZ, double legAngle) :
	_logger(logger), _commManager(commManager),
	_legStatus(Landed), _footPosition(0, 0, 0), _footGoalPosition(0, 0, 0),
	_relativeCoxaPosition(relativeCoxaPosX, relativeCoxaPosY, relativeCoxaPosZ), _footInitPosition(initFootPosX, initFootPosY, initFootPosZ),
	_footCoord(relativeCoxaPosX+initFootPosX, relativeCoxaPosY+initFootPosY),
	_coxaLength(coxaLength), _femurLength(femurLength), _tibiaLength(tibiaLength),
	_legAngle(legAngle)
{
	_coxaServo = coxaServo;
	_femurServo = femurServo;
	_tibiaServo = tibiaServo;
}

bool Leg::isOnTheGround()
{
	return (_legStatus == Landed);
}

void Leg::stepRotate(bool direction, double size)
{
	step(direction ? _legAngle + 30 : _legAngle - 30, size);
}

void Leg::step(double angle, double size)
{
	if (isOnTheGround() && _previousLeg->isOnTheGround() && _nextLeg->isOnTheGround())
	{
		QVector3D centerTranslation = QVector3D(size * cos(angle), size * sin(angle), 0);

		QVector3D feetPosRelToCoxa = _footInitPosition + QVector3D(0, 0, size);

		QVector3D relativeFootPos(feetPosRelToCoxa.x() * cos(_legAngle) + feetPosRelToCoxa.y() * sin(_legAngle),
								  -feetPosRelToCoxa.x() * sin(_legAngle) + feetPosRelToCoxa.y() * cos(_legAngle),
								  feetPosRelToCoxa.z());

		double tibiaAngle, femurAngle, coxaAngle;

		reverseKinematics(&relativeFootPos, &coxaAngle, &femurAngle, &tibiaAngle);

		_coxaServo->setGoal(coxaAngle, 100);
		_femurServo->setGoal(femurAngle, 100);
		_tibiaServo->setGoal(tibiaAngle, 100);
        //_commManager->calculateSmoothServosSpeed(50);

		_footGoalPosition = _footInitPosition + _relativeCoxaPosition + centerTranslation;
		_legStatus = Raising;
	}
}

void Leg::stepPartTwo()
{
	QVector3D feetPosRelToCoxa = _footGoalPosition - _relativeCoxaPosition;

	QVector3D relativeFootPos(feetPosRelToCoxa.x() * cos(_legAngle) + feetPosRelToCoxa.y() * sin(_legAngle),
							  -feetPosRelToCoxa.x() * sin(_legAngle) + feetPosRelToCoxa.y() * cos(_legAngle),
							  feetPosRelToCoxa.z());

	double tibiaAngle, femurAngle, coxaAngle;

	reverseKinematics(&relativeFootPos, &coxaAngle, &femurAngle, &tibiaAngle);

	_coxaServo->setGoal(coxaAngle, 100);
	_femurServo->setGoal(femurAngle, 100);
	_tibiaServo->setGoal(tibiaAngle, 100);
//	_commManager->calculateSmoothServosSpeed(50);
}

void Leg::moveBy(double x, double y, double z, double maxTorque)
{
	QVector3D centerTranslation = QVector3D(x, y, z);

	QVector3D feetPosRelToCoxa = _footPosition - _relativeCoxaPosition + centerTranslation;

	QVector3D relativeFootPos(feetPosRelToCoxa.x() * cos(_legAngle) + feetPosRelToCoxa.y() * sin(_legAngle),
							  -feetPosRelToCoxa.x() * sin(_legAngle) + feetPosRelToCoxa.y() * cos(_legAngle),
							  feetPosRelToCoxa.z());

	double tibiaAngle, femurAngle, coxaAngle;

	reverseKinematics(&relativeFootPos, &coxaAngle, &femurAngle, &tibiaAngle);

	_coxaServo->setGoal(coxaAngle, maxTorque);
	_femurServo->setGoal(femurAngle, maxTorque);
	_tibiaServo->setGoal(tibiaAngle, maxTorque);
	_footGoalPosition = feetPosRelToCoxa + _relativeCoxaPosition;
}

void Leg::rotateBy(double rotZ, double maxTorque)
{
	_coxaServo->setGoal(_coxaServo->getPosition() + rotZ, maxTorque);
}

void Leg::moveTo(double x, double y, double z, double maxTorque)
{
	//*_logger << "move leg to : " << x << "," << y << "," << z << Tools::endl;

	QVector3D feetPosRelToCoxa = _footInitPosition + QVector3D(x, y, z);

	QVector3D relativeFootPos(feetPosRelToCoxa.x() * cos(_legAngle) + feetPosRelToCoxa.y() * sin(_legAngle),
							  -feetPosRelToCoxa.x() * sin(_legAngle) + feetPosRelToCoxa.y() * cos(_legAngle),
							  feetPosRelToCoxa.z());

	double tibiaAngle, femurAngle, coxaAngle;

	reverseKinematics(&relativeFootPos, &coxaAngle, &femurAngle, &tibiaAngle);

	_coxaServo->setGoal(coxaAngle, maxTorque);
	_femurServo->setGoal(femurAngle, maxTorque);
	_tibiaServo->setGoal(tibiaAngle, maxTorque);
	_footGoalPosition = feetPosRelToCoxa + _relativeCoxaPosition;
}

void Leg::initPosition()
{
	QVector3D feetPosRelToCoxa = _footInitPosition;

	QVector3D relativeFootPos(feetPosRelToCoxa.x() * cos(_legAngle) + feetPosRelToCoxa.y() * sin(_legAngle),
							  -feetPosRelToCoxa.x() * sin(_legAngle) + feetPosRelToCoxa.y() * cos(_legAngle),
							  feetPosRelToCoxa.z());

	double coxaAngle, femurAngle, tibiaAngle;

	reverseKinematics(&relativeFootPos, &coxaAngle, &femurAngle, &tibiaAngle);

	_coxaServo->setGoal(coxaAngle, 100);
	_coxaServo->setSpeed(20);
	_femurServo->setGoal(femurAngle, 100);
	_femurServo->setSpeed(20);
	_tibiaServo->setGoal(tibiaAngle, 100);
	_tibiaServo->setSpeed(20);
	_footGoalPosition = feetPosRelToCoxa + _relativeCoxaPosition;
}

void Leg::reverseKinematics(QVector3D *relativeFootPosition, double *coxaAngle, double *femurAngle, double *tibiaAngle)
{
	double coxaFeetDist = QVector2D(*relativeFootPosition).length();	// distance in (x, y) plan

	double sw, a1, a2, ta;
	sw = sqrt(square(coxaFeetDist - _coxaLength) + square(relativeFootPosition->z()));
	a1 = atan2(relativeFootPosition->z(), coxaFeetDist - _coxaLength);
	a2 = acos((square(_tibiaLength)-square(_femurLength)-square(sw))/(-2 * sw * _femurLength));
	ta = acos((square(sw)-square(_tibiaLength)-square(_femurLength))/(-2 * _tibiaLength * _femurLength));

	*coxaAngle = 60 - Tools::radianToDegree(Tools::angleInMinusPiPlusPi(-atan2(relativeFootPosition->x(), relativeFootPosition->y())));
	*femurAngle = Tools::radianToDegree(a1 + a2) + 150;
	*tibiaAngle = 90 - Tools::radianToDegree(ta) + 240;
}

void Leg::updatePosition(RPoint *robotCoord)
{
	double coxaAngle, femurAngle, tibiaAngle;
	coxaAngle = Tools::degreeToRadian(_coxaServo->getPosition() - 150);
	femurAngle = Tools::degreeToRadian(_femurServo->getPosition() - 150);
	tibiaAngle = Tools::degreeToRadian(150 - _tibiaServo->getPosition());

	double x, y, z, theta;
	x = cos(coxaAngle) * (_tibiaLength * cos(femurAngle + tibiaAngle) + _femurLength * cos(femurAngle) + _coxaLength);
	y = -sin(coxaAngle) * (_tibiaLength * cos(femurAngle + tibiaAngle) + _femurLength * cos(femurAngle) + _coxaLength);
	z = _tibiaLength * sin(femurAngle + tibiaAngle) + _femurLength * sin(femurAngle);

	_footPosition.setX(x * cos(_legAngle) - y * sin(_legAngle) + _relativeCoxaPosition.x());
	_footPosition.setY(x * sin(_legAngle) + y * cos(_legAngle) + _relativeCoxaPosition.y());
	_footPosition.setZ(z + _relativeCoxaPosition.z());

	theta = robotCoord->getTheta();

	if (_legStatus != Landed)
	{
		// update foot coordinate

		_footCoord.setX(robotCoord->getX() + _footPosition.x() * cos(theta) - _footPosition.y() * sin(theta));
		_footCoord.setY(robotCoord->getY() + _footPosition.x() * sin(theta) + _footPosition.y() * cos(theta));

		if (_legStatus == Raising && _footPosition.z() > -40)
		{
			_legStatus = Lowering;
			stepPartTwo();
		}

		if (_coxaServo->getAngleDifference() < 3 && _femurServo->getAngleDifference() < 3 && _tibiaServo->getAngleDifference() < 3)
			_legStatus = Landed;
	}
	else
	{
		// update robot coordinate
		robotCoord->setX(_footCoord.getX() - (_footPosition.x() * cos(theta) - _footPosition.y() * sin(theta)));
		robotCoord->setY(_footCoord.getY() - (_footPosition.x() * sin(theta) + _footPosition.y() * cos(theta)));
	}
}

bool Leg::isStepRequired(double angle)
{
	QVector3D coord = _footInitPosition  - _footPosition + _relativeCoxaPosition;
	double d = coord.x() * cos(angle) + coord.y() * sin(angle);
	return (d > 10);
}

bool Leg::isStepRotateRequired(bool direction)
{
	double d = _coxaServo->getPosition() - 150;
	return (direction ? d < -10 : d > -10);
}

QVector3D Leg::getFootRelativePosition()
{
	return _footPosition;
}

void Leg::setPreviousLeg(Leg *leg)
{
	_previousLeg = leg;
}

void Leg::setNextLeg(Leg *leg)
{
	_nextLeg = leg;
}

Hexapod::Hexapod(AbstractLogger* logger, Comm::AX12CommManager* commManager, Comm::UM6* um6,
				double hexaRadius, double coxaLength, double femurLength, double tibiaLength,
				double initHeight, double initLegLength, double maxStepSize,
				float coxaMinAngle, float coxaMaxAngle, float femurMinAngle,
				float femurMaxAngle, float tibiaMinAngle, float tibiaMaxAngle) :
	_logger(logger), _commManager(commManager), _um6(um6),
	_hexaRadius(hexaRadius), _coxaLength(coxaLength), _femurLength(femurLength), _tibiaLength(tibiaLength),
	_initHeight(initHeight), _initLegLength(initLegLength), _maxStepSize(maxStepSize),
	_coxaMinAngle(coxaMinAngle), _coxaMaxAngle(coxaMaxAngle),
	_femurMinAngle(femurMinAngle), _femurMaxAngle(femurMaxAngle),
	_tibiaMinAngle(tibiaMinAngle), _tibiaMaxAngle(tibiaMaxAngle),
	_gaitCurrentStep(0), _gaitMaxStep(3), _autoWalk(false),
	_currentCoord(0, 0, 0), _goalCoord(0, 0, 0), _currentAngle(0), _initOrientation(0)
{
    /*double offsetX = _hexaRadius * cos(Tools::pi/6);
	double offsetY = _hexaRadius * 0.5;

	double legCos = _initLegLength * cos(Tools::pi/6);
	double legSin = _initLegLength * 0.5;

    _legs[0] = new Leg(_logger, _commManager,
                       _commManager->resetServo(1, _coxaMinAngle, _coxaMaxAngle),
                       _commManager->resetServo(7, _femurMinAngle, _femurMaxAngle),
                       _commManager->resetServo(13, _tibiaMinAngle, _tibiaMaxAngle),
					   _coxaLength, _femurLength, _tibiaLength,
					   offsetX, -offsetY, 0,
					   legCos, -legSin, -_initHeight,
					   -PI_SUR_6);

	_legs[1] = new Leg(_logger, _commManager,
                       _commManager->resetServo(2, _coxaMinAngle, _coxaMaxAngle),
                       _commManager->resetServo(8, _femurMinAngle, _femurMaxAngle),
                       _commManager->resetServo(14, _tibiaMinAngle, _tibiaMaxAngle),
                       _coxaLength, _femurLength, _tibiaLength,
					   0, -_hexaRadius, 0,
					   0, -_initLegLength, -_initHeight,
					   -3 * PI_SUR_6);

	_legs[2] = new Leg(_logger, _commManager,
                       _commManager->resetServo(3, _coxaMinAngle, _coxaMaxAngle),
                       _commManager->resetServo(9, _femurMinAngle, _femurMaxAngle),
                       _commManager->resetServo(15, _tibiaMinAngle, _tibiaMaxAngle),
					   _coxaLength, _femurLength, _tibiaLength,
					   -offsetX, -offsetY, 0,
					   -legCos, -legSin, -_initHeight,
					   -5 * PI_SUR_6);

	_legs[3] = new Leg(_logger, _commManager,
                       _commManager->resetServo(4, _coxaMinAngle, _coxaMaxAngle),
                       _commManager->resetServo(10, _femurMinAngle, _femurMaxAngle),
                       _commManager->resetServo(16, _tibiaMinAngle, _tibiaMaxAngle),
					   _coxaLength, _femurLength, _tibiaLength,
					   -offsetX, offsetY, 0,
					   -legCos, legSin, -_initHeight,
					   5 * PI_SUR_6);

	_legs[4] = new Leg(_logger, _commManager,
                       _commManager->resetServo(5, _coxaMinAngle, _coxaMaxAngle),
                       _commManager->resetServo(11, _femurMinAngle, _femurMaxAngle),
                       _commManager->resetServo(17, _tibiaMinAngle, _tibiaMaxAngle),
					   _coxaLength, _femurLength, _tibiaLength,
					   0, _hexaRadius, 0,
					   0, _initLegLength, -_initHeight,
					   3 * PI_SUR_6);

	_legs[5] = new Leg(_logger, _commManager,
                       _commManager->resetServo(6, _coxaMinAngle, _coxaMaxAngle),
                       _commManager->resetServo(12, _femurMinAngle, _femurMaxAngle),
                       _commManager->resetServo(18, _tibiaMinAngle, _tibiaMaxAngle),
					   _coxaLength, _femurLength, _tibiaLength,
					   offsetX, offsetY, 0,
					   legCos, legSin, -_initHeight,
					   PI_SUR_6);

	_legs[0]->setPreviousLeg(_legs[5]);
	_legs[1]->setPreviousLeg(_legs[0]);
	_legs[2]->setPreviousLeg(_legs[1]);
	_legs[3]->setPreviousLeg(_legs[2]);
	_legs[4]->setPreviousLeg(_legs[3]);
	_legs[5]->setPreviousLeg(_legs[4]);
	_legs[0]->setNextLeg(_legs[1]);
	_legs[1]->setNextLeg(_legs[2]);
	_legs[2]->setNextLeg(_legs[3]);
	_legs[3]->setNextLeg(_legs[4]);
	_legs[4]->setNextLeg(_legs[5]);
    _legs[5]->setNextLeg(_legs[0]);*/

    //connect(_commManager, SIGNAL(servosStatusUpdated()), this, SLOT(servosStatusUpdated()));

    //for (int i = 0; i < 6; i++)
        //_legs[i]->initPosition();

    //_commManager->synchronize();
}

Leg *Hexapod::getLeg(quint8 legNo)
{
	return _legs[legNo-1];
}

void Hexapod::moveCenterTo(double x, double y, double z, double rotX, double rotY, double rotZ, float maxTorque)
{
	Q_UNUSED(rotX);	Q_UNUSED(rotY);	Q_UNUSED(rotZ);

	for (int legNo = 0; legNo < 6; legNo++)
	{
		if (_legs[legNo]->isOnTheGround())
		{
			//QVector3D currentPos = _legs[legNo]->getFootRelativePosition();
			_legs[legNo]->moveTo(-x, -y, -z, maxTorque);
		}
	}
}

void Hexapod::setAutoWalk(bool autoWalk)
{
	_autoWalk = autoWalk;
}

void Hexapod::servosStatusUpdated()
{
	calculateServosCommands();
}

void Hexapod::moveBy(double length, double angle, double speed)
{
	for (int legNo = 0; legNo < 6; legNo++)
	{
		if (_legs[legNo]->isStepRequired(angle))
			_legs[legNo]->step(angle, _maxStepSize);

		if (_legs[legNo]->isOnTheGround())
			_legs[legNo]->moveBy(-length * cos(angle), -length * sin(angle), 0, 100);

//		_commManager->calculateSmoothServosSpeed(speed);
	}
    //_commManager->synchronize();
}

void Hexapod::rotateBy(double angle, double speed)
{
	bool direction = (angle > 0);

	for (int legNo = 0; legNo < 6; legNo++)
	{
		if (_legs[legNo]->isStepRotateRequired(direction))
			_legs[legNo]->stepRotate(direction, _maxStepSize);

		if (_legs[legNo]->isOnTheGround())
			_legs[legNo]->rotateBy(-angle, 100);

//		_commManager->calculateSmoothServosSpeed(speed);
	}
    //_commManager->synchronize();
}

void Hexapod::updatePosition()
{
	quint8 legNo;
	RPoint robotCoord(0, 0, 0);

	// set angle from orientation sensor
	_currentCoord.setTheta(Tools::angleInMinusPiPlusPi(Tools::degreeToRadian(_um6->getPsi() - _initOrientation)));

	for (legNo = 0; legNo < 6; legNo++)
	{
		RPoint robotCoordTemp = _currentCoord;
		_legs[legNo]->updatePosition(&robotCoordTemp);

		robotCoord += robotCoordTemp;
	}

	_currentCoord = robotCoord / 6;
}

void Hexapod::calculateServosCommands()
{
	updatePosition();

	if (_autoWalk)
	{
		RPoint diff = _goalCoord - _currentCoord;

		if (fabs(diff.getThetaDegree()) > 5)
			rotateBy(MIN(diff.getThetaDegree(), 15), 10);
		else if (fabs(diff.getX()) > 10)
			moveBy(MIN(diff.getX(), 25.0), 0, 10);
	}
}

RPoint Hexapod::getRobotCoord()
{
	return _currentCoord;
}

void Hexapod::initOrientation()
{
	_initOrientation = _um6->getPsi();
}

void Hexapod::setGoal(double x, double y, double theta)
{
	_goalCoord = RPoint(x, y, Tools::degreeToRadian(theta));
}
