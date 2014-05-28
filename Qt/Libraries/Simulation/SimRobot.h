#ifndef SIMROBOT_H
#define SIMROBOT_H

#include <AbstractCommInterface.h>
#include "SimProtocol.h"
#include <Instructions.h>

class SimCommInterface
{
public:
    SimCommInterface();
    virtual void setPosition(qint16 x, qint16 y, double theta) = 0;
	virtual void addDestination(qint16 x, qint16 y, double theta, quint8 type, quint8 deplacementType, bool isStopPoint, int speed) = 0;
	virtual void executeAction(int action, quint16 parameter) = 0;
    virtual void flush() = 0;
	virtual void askParameters() = 0;
	virtual void setParameters(const QList<float>& values) = 0;
};

class SimRobot : public Comm::AbstractCommInterface
{
public:

    SimRobot(SimProtocol* protocol, SimCommInterface* networkInterface);
    ~SimRobot();

	void sendCoordinates(double x, double y, double theta, bool forward);
	void sendObjective(double x, double y, double theta);
    void sendAvoidingSensorsValues(const QList<quint8> &sharps);
    void go(bool mirrored);
	void sendNoticeOfReceipt(quint8 instruction, bool result);
    void sendArrived();
    void sendBlocked();
	void sendEvent(Comm::RobotEvent event);
	void sendSensorEvent(Comm::SensorType type, int sensorId, int value);
    void restart();
	void sendLog(const QByteArray& log);
	void sendParameters(const QList<float>& values, const QList<QByteArray> &names);

private:
    SimProtocol* _protocol;
    SimCommInterface* _interface;

    void read(quint8 instruction, const Comm::Data& data);
};

#endif // SIMROBOT_H
