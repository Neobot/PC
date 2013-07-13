#ifndef HEXAPOD_H
#define HEXAPOD_H

#include <QVector3D>
#include <QVector2D>
#include "AbstractLogger.h"
#include "AX12CommManager.h"
#include "UM6.h"
#include "RPoint.h"
#include "Point.h"

using namespace Tools;

class Leg
{
	enum LegStatus
	{
		Raising,
		Lowering,
		Landed,
		Collided
	};

public:
	Leg(Tools::AbstractLogger* logger, Comm::AX12CommManager* commManager,
		Comm::AX12* coxaServo, Comm::AX12* femurServo, Comm::AX12* tibiaServo,
		double coxaLength, double femurLength, double tibiaLength,
		double relativeCoxaPosX, double relativeCoxaPosY, double relativeCoxaPosZ,
		double initFootPosX, double initFootPosY, double initFootPosZ,
		double legAngle);
	bool isOnTheGround();
	void step(double angle, double size);
	void stepRotate(bool direction, double size);
	void stepPartTwo();
	void moveBy(double x, double y, double z, double maxTorque);
	void rotateBy(double rotZ, double maxTorque);
	void moveTo(double x, double y, double z, double maxTorque);
	void initPosition();
	void updatePosition(RPoint *robotCoord);
	bool isStepRequired(double angle);
	bool isStepRotateRequired(bool direction);
	QVector3D getFootRelativePosition();
	void setPreviousLeg(Leg * leg);
	void setNextLeg(Leg *leg);

private:
	Tools::AbstractLogger* _logger;
	Comm::AX12CommManager* _commManager;
	LegStatus _legStatus;
	QVector3D _footPosition;
	QVector3D _footGoalPosition;
	QVector3D _relativeCoxaPosition;
	QVector3D _footInitPosition;
	Point _footCoord;
	double _coxaLength, _femurLength, _tibiaLength;
	double _legAngle;
	Comm::AX12 *_coxaServo, *_femurServo, *_tibiaServo;
	Leg *_previousLeg, *_nextLeg;
	void reverseKinematics(QVector3D *relativeFootPosition, double *coxaAngle, double *femurAngle, double *tibiaAngle);
};

class Hexapod : public QObject
{
	Q_OBJECT

public:
	Hexapod(Tools::AbstractLogger* logger, Comm::AX12CommManager* commManager, Comm::UM6* um6,
				 double hexaRadius, double coxaLength, double femurLength, double tibiaLength,
				 double initHeight, double initLegLength, double maxStepSize,
				 float coxaMinAngle, float coxaMaxAngle, float femurMinAngle,
				 float femurMaxAngle, float tibiaMinAngle, float tibiaMaxAngle);

	void moveCenterTo(double x, double y, double z,  double rotX, double rotY, double rotZ, float maxTorque);
	void moveBy(double length, double angle, double speed);
	void rotateBy(double angle, double speed);
	void setAutoWalk(bool autoWalk);
	Leg *getLeg(quint8 legNo);
	RPoint getRobotCoord();
	void setGoal(double x, double y, double theta);
	void initOrientation();

private:
	void calculateServosCommands();
	void updatePosition();
	Tools::AbstractLogger* _logger;
	Comm::AX12CommManager* _commManager;
	Comm::UM6* _um6;
	double _hexaRadius, _coxaLength, _femurLength, _tibiaLength;
	double _initHeight, _initLegLength, _maxStepSize;
	float _coxaMinAngle, _coxaMaxAngle;
	float _femurMinAngle, _femurMaxAngle;
	float _tibiaMinAngle, _tibiaMaxAngle;
	quint8 _gaitCurrentStep, _gaitMaxStep;
	bool _autoWalk;
	RPoint _currentCoord, _goalCoord;
	double _currentAngle;
	double _initOrientation;
	Leg *_legs[6];

private slots:
	void servosStatusUpdated();
};


#endif // HEXAPOD_H
