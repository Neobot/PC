#ifndef NETWORKCOMMINTERFACE_H
#define NETWORKCOMMINTERFACE_H

#include "RobotCommInterface.h"

namespace Tools
{
	class AbstractLogger;
}

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
	void sendFiles(quint8 strategyNum, const QStringList& filenames);
	void sendFileData(quint8 strategyNum, const QString &filename, const QByteArray& fileData);
	void sendFileEvent(quint8 strategyNum, const QString &filename, quint8 event);

	void sendStrategyStatus(quint8 strategyNum, bool isRunning);
	void sendAutoStrategyInfo(bool enabled, quint8 strategyNum, const QByteArray &robotPort, const QByteArray &ax12Port, bool simulation, bool mirror, quint8 delayInSeconds);

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

class CommLogger : public Tools::AbstractLogger
{
public:
	CommLogger(NetworkCommInterface* comm);
	void log(const char* text);
	void log(char text);
	void noLogger(bool value) {_quiet = value;}

	/**
	 * \brief Do nothing.
	 */
	void clear() {}

	NetworkCommInterface* getComm() const;
	void setComm(NetworkCommInterface* comm);
private:
	bool _quiet;
	NetworkCommInterface* _comm;
	QByteArray _str;
};

#endif // NETWORKCOMMINTERFACE_H
