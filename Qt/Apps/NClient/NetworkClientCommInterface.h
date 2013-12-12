#ifndef NETWORKCLIENTCOMMINTERFACE_H
#define NETWORKCLIENTCOMMINTERFACE_H

#include "RobotCommInterface.h"
#include "NetworkClientCommListener.h"

#include "Instructions.h"

class NetworkClientCommInterface : public Comm::RobotCommInterface
{
	Q_OBJECT
public:
	NetworkClientCommInterface(Comm::AbstractProtocol* protocol, Tools::AbstractLogger* logger, Comm::CommListener* commListener = 0, NetworkClientCommListener* networkListener = 0, QObject* parent = 0);
	
	void setNetworkListener(NetworkClientCommListener* listener);

	void connectToRobot(bool simulation, const QString &robotPort, const QString &ax12Port);
	void disconnectFromRobot();

	bool updateServer(const QString& serverExecutablePath);
	void sendPingToServer();

	void askStrategies();
	void askStrategyFiles(quint8 strategyNum);
	void askStrategyFileData(quint8 strategyNum, const QString& filename);
	void sendStrategyFileData(quint8 strategyNum, const QString& filename, const QByteArray& data);
	void resetStrategyFileData(quint8 strategyNum, const QString& filename);

	void startStrategy(quint8 strategyNum, bool mirror);
	void stopStrategy();
	void askStrategyStatus();

	void askAutoStrategyInfo();
	void setAutoStrategy(bool enabled, quint8 strategyNum, const QString &robotPort, const QString &ax12Port, bool simulation, bool mirroredSimulation);

	void askSerialPorts();

    void askAx12Positions(const QList<int>& ids, bool recursive = false);
	void moveAx12(const Comm::Ax12Info& ax12Info);
	void moveAx12(const QList<Comm::Ax12Info>& ax12Infos);
	void moveAx12Smoothed(float maxSpeed, const QList<Comm::Ax12Info> &ax12Infos);

	void lockAx12(const QMap<int, bool>& lockedServoInfo);
	
	void askAx12Movements();
	void setAx12Movements(const QByteArray& data);
    void runAx12Movement(const QString& group, const QString& movement, float speedLimit, int lastPositionIndex = -1);

protected slots:
	virtual void read(quint8 instruction, const Comm::Data& data);

private:
	NetworkClientCommListener* _listener;
};

#endif // NETWORKCLIENTCOMMINTERFACE_H
