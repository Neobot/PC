#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

#include "NetworkClientCommListener.h"
#include "CommDispatcher.h"

class NetworkClientCommInterface;


class NetworkConnection : public QObject, public Comm::CommDispatcher, public NetworkClientCommListener
{
	Q_OBJECT

public:
	enum ConnectionStatus
	{
		Lookup,
		Disconnected,
		Connected,
		Controled
	};

signals:
	void statusChanged(NetworkConnection::ConnectionStatus status);
	void socketError(const QString& message);

public:
	NetworkConnection();
	~NetworkConnection();

	void connectToServer(const QString& adress, int port);
	void disconnectFromServer();

	void askRobotControl(bool simulation, const QString& robotPort, const QString& ax12Port);
	void closeRobotControl();

	void registerNetworkResponder(NetworkClientCommListener* responder);
	void unregisterNetworkResponder(NetworkClientCommListener* responder);
	void registerRobotResponder(Comm::CommListener* responder);
	void unregisterRobotResponder(Comm::CommListener* responder);

	ConnectionStatus getConnectionStatus() const;

	NetworkClientCommInterface* getComm() const;

private:
	NetworkClientCommInterface* _comm;
	QTcpSocket*	_socket;

	ConnectionStatus _status;

	QList<NetworkClientCommListener*> _networkResponders;

	void changeStatus(ConnectionStatus status);

	//network dispatch
	void serverAnnouncement(const QByteArray& message);
	void networkNoticeOfReceipt(quint8 instruction, bool result);
	void setStrategies(const QStringList& strategies);
	void setStrategyFiles(int strategyNum, const QStringList& filenames);
	void setStrategyFileData(int strategyNum, const QString& filename, const QByteArray& data);
	void setSerialPorts(const QStringList& ports);
	void setAx12Positions(const QList<int> ids, const QList<float>& positions, const QList<float>& loads);
	void setAx12MovementsFile(const QByteArray& data);
	void strategyStatusUpdated(int strategyNum, bool isRunning);
	void autoStrategyInfo(bool enabled, int strategyNum, const QString &robotPort, const QString &ax12Port, bool simulation, bool mirror, int delayInSeconds);



private slots:
	void socketConnected();
	void socketDisconnected();
	void socketError(QAbstractSocket::SocketError error);
};

#endif // TCPCLIENT_H
