#include "NetworkConnection.h"
#include "NetworkProtocol.h"
#include "NetworkClientCommInterface.h"
#include "Instructions.h"

#include <QHostAddress>

NetworkConnection::NetworkConnection()
{
	_status = Disconnected;
	_socket = new QTcpSocket;
	connect(_socket, SIGNAL(connected()), this, SLOT(socketConnected()));
	connect(_socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
	connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));

	Comm::NetworkProtocol* protocol = new Comm::NetworkProtocol(_socket);
	_comm = new NetworkClientCommInterface(protocol, 0, this, this);
}

NetworkConnection::~NetworkConnection()
{
	delete _comm->getProtocol();
	delete _comm;
	delete _socket;
}

void NetworkConnection::connectToServer(const QString &adress, int port)
{
	QHostAddress host(QHostAddress::LocalHost);
	if (!adress.isEmpty())
		host.setAddress(adress);

	_socket->connectToHost(host, port);
	changeStatus(Lookup);
}

void NetworkConnection::disconnectFromServer()
{
	if (_socket->isOpen())
		_socket->abort();
}

void NetworkConnection::askRobotControl(bool simulation, const QString &robotPort, const QString &ax12Port)
{
	_comm->connectToRobot(simulation, robotPort, ax12Port);
}

void NetworkConnection::closeRobotControl()
{
	_comm->disconnectFromRobot();
}

void NetworkConnection::registerNetworkResponder(NetworkClientCommListener *responder)
{
	_networkResponders << responder;
}

void NetworkConnection::unregisterNetworkResponder(NetworkClientCommListener *responder)
{
	_networkResponders.removeAll(responder);
}

void NetworkConnection::registerRobotResponder(Comm::CommListener *responder)
{
	registerResponder(responder);
}

void NetworkConnection::unregisterRobotResponder(Comm::CommListener *responder)
{
	unregisterResponder(responder);
}

NetworkConnection::ConnectionStatus NetworkConnection::getConnectionStatus() const
{
	return _status;
}

NetworkClientCommInterface *NetworkConnection::getComm() const
{
	return _comm;
}

void NetworkConnection::changeStatus(NetworkConnection::ConnectionStatus status)
{
	_status = status;
	emit statusChanged(_status);
}

void NetworkConnection::serverAnnouncement(const QByteArray &message)
{
	foreach(NetworkClientCommListener* responder, _networkResponders)
		responder->serverAnnouncement(message);
}

void NetworkConnection::networkNoticeOfReceipt(quint8 instruction, bool result)
{
	switch(instruction)
	{
		case Comm::CONNECT:
			if (result)
				changeStatus(Controled);
			break;
		case Comm::DISCONNECT:
			if (result)
				changeStatus(Connected);
			break;
	}

	foreach(NetworkClientCommListener* responder, _networkResponders)
		responder->networkNoticeOfReceipt(instruction, result);
}

void NetworkConnection::setStrategies(const QStringList &strategies)
{
	foreach(NetworkClientCommListener* responder, _networkResponders)
		responder->setStrategies(strategies);
}

void NetworkConnection::setStrategyFiles(int strategyNum, const QStringList &filenames)
{
	foreach(NetworkClientCommListener* responder, _networkResponders)
		responder->setStrategyFiles(strategyNum, filenames);
}

void NetworkConnection::setStrategyFileData(int strategyNum, const QString &filename, const QByteArray &data)
{
	foreach(NetworkClientCommListener* responder, _networkResponders)
		responder->setStrategyFileData(strategyNum, filename, data);
}

void NetworkConnection::setSerialPorts(const QStringList &ports)
{
	foreach(NetworkClientCommListener* responder, _networkResponders)
		responder->setSerialPorts(ports);
}

void NetworkConnection::setAx12Positions(const QList<int> ids, const QList<float> &positions)
{
	foreach(NetworkClientCommListener* responder, _networkResponders)
		responder->setAx12Positions(ids, positions);
}

void NetworkConnection::setAx12MovementsFile(const QByteArray& data)
{
	foreach(NetworkClientCommListener* responder, _networkResponders)
		responder->setAx12MovementsFile(data);
}

void NetworkConnection::strategyStatusUpdated(int strategyNum, bool isRunning)
{
	foreach(NetworkClientCommListener* responder, _networkResponders)
		responder->strategyStatusUpdated(strategyNum, isRunning);
}

void NetworkConnection::autoStrategyInfo(bool enabled, int strategyNum, const QString &robotPort, const QString &ax12Port, bool simulation, bool mirror)
{
	foreach(NetworkClientCommListener* responder, _networkResponders)
		responder->autoStrategyInfo(enabled, strategyNum, robotPort, ax12Port, simulation, mirror);
}

void NetworkConnection::socketConnected()
{
	changeStatus(Connected);
}

void NetworkConnection::socketDisconnected()
{
	changeStatus(Disconnected);
}

void NetworkConnection::socketError(QAbstractSocket::SocketError error)
{
	Q_UNUSED(error);
	emit socketError(_socket->errorString());
	changeStatus(Disconnected);
}
