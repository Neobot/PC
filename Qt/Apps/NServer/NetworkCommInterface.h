#ifndef NETWORKCOMMINTERFACE_H
#define NETWORKCOMMINTERFACE_H

#include "RobotCommInterface.h"

class NetworkCommListener;

class NetworkCommInterface : public Comm::AbstractCommInterface
{
	Q_OBJECT
public:
	NetworkCommInterface(Comm::AbstractProtocol* protocol, Comm::RobotCommInterface* robotInterface, Tools::AbstractLogger* logger, QObject* parent = 0);
	
	void setListener(NetworkCommListener *listener);
	NetworkCommListener *getListener() const;

	void sendAnnouncement(const QByteArray& message);

	void sendStrategies();
	void sendStrategyFiles(quint8 strategyNum, const QStringList& filenames);
	void sendStrategyFileData(quint8 strategyNum, const QString &filename, const QByteArray& fileData);

	void sendStrategyStatus(quint8 strategyNum, bool isRunning);
	void sendAutoStrategyInfo(bool enabled, quint8 strategyNum, const QByteArray &robotPort, const QByteArray &ax12Port, bool simulation, bool mirror);

	void sendAvailableSerialPorts();

	void sendAx12Positions(const QList<quint8> ids, const QList<float>& positions, const QList<float>& loads);
	void sendAx12Movements(const QByteArray& data);

	void sendNoticeOfReceipt(quint8 instruction, bool result);
	void ping();

	void setRobotInterface(Comm::RobotCommInterface* robotInterface);

private:
	Comm::RobotCommInterface* _robotInterface;
	NetworkCommListener* _listener;
	bool _connected;

private slots:
	void read(quint8 instruction, const Comm::Data& data);
	void readFromRobot(quint8 instruction, const Comm::Data& data);
};

#endif // NETWORKCOMMINTERFACE_H
