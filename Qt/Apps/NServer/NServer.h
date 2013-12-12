#ifndef NSERVER_H
#define NSERVER_H

#include <QObject>
#include <QMap>
#include <QSignalMapper>
#include <QTcpServer>
#include <QSettings>

#include "AbstractLogger.h"
#include "NetworkCommInterface.h"
#include "NetworkCommListener.h"
#include "Simulator.h"
#include "Strategies/StrategyEnumerator.h"
#include "Ax12MovementRunner.h"
#include "AX12StatusListener.h"
#include "ServerAX12RequestManager.h"

class Pather;

class NServer : public QObject, public Tools::LoggerInterface, public NetworkCommListener
{
	Q_OBJECT

public:
	explicit NServer(Tools::AbstractLogger* logger = 0, QObject *parent = 0);
	~NServer();
	
	bool start();

private:
	QDir _dataDirectory;
	QSettings _settings;
	QByteArray _autoStartMessage;

	QTcpServer* _tcpServer;
	QMap<unsigned int, NetworkCommInterface*> _connections;
	QList<NetworkCommInterface*> _robotConnections;
	unsigned int _nextConnectionIndex;
	QSignalMapper* _disconnectionMapper;

	Comm::RobotCommInterface* _robotInterface;
	Simulator* _simulator;

	StrategyEnumerator _strategiesEnumerator;
	StrategyManager* _strategyManager;
	Pather* _pather;
	StrategyInterface* _currentStrategy;
	int _currentStrategyId;

	bool _robotConnected;

	Comm::AX12CommManager* _ax12Manager;
	Tools::Ax12MovementManager _ax12Movements;
    Comm::Ax12MovementRunner* _ax12MovementRunner;
    QHash<NetworkCommInterface*, ServerAX12RequestManager*> _ax12Requests;

    void initServerSettings();

	void sendGlobalAnnoucement(const QByteArray &message);
	
	void cleanRobotConnection();

	bool pingReceived();
	void noticeOfReceipt(quint8 instruction, bool result);
	bool connectToRobot(NetworkCommInterface* networkInterface, bool simulation, const QString& robotPort, const QString& ax12Port, QByteArray &message);
	bool disconnectToRobot(NetworkCommInterface* networkInterface);
	bool updateServer(const QByteArray &data);
	QStringList askStrategyFiles(int strategyNum);
	QByteArray askStrategyFileData(int strategyNum, const QString& filename);
	void setStrategyFileData(int strategyNum, const QString& filename, const QByteArray &data);
	void resetStrategyFile(int strategyNum, const QString& filename);

    void askAx12Positions(NetworkCommInterface* networkInterface, const QList<quint8>& ids, bool recursive);
	void moveAx12(float maxSpeed, QList<Comm::Ax12Info>& ax12s);
	void lockAx12(const QMap<quint8, bool>& servoLockInfo);

	void updateRobotConnection();

	bool startStrategy(int strategyNum, bool mirror);
	bool stopStrategy(int& currentStrategyNum) ;
	void askStrategyStatus(int &currentStrategyNum, bool &isRunning);

	void askAutoStrategyInfo(bool &isEnabled, int &strategyNum, QString &robotPort, QString &ax12Port, bool &simulation, bool &mirror);
	void setAutoStrategy(bool enabled, int strategyNum, const QString &robotPort, const QString &ax12Port, bool simulation, bool mirror);
	
	QByteArray askAx12MovementFile();
	void setAx12MovementFile(const QByteArray& data);
    void runAx12Movement(const QString& group, const QString& movement, float speedLimit, int lastPositionIndex);


private slots:
	void newConnection();
	void removeConnection(int connectionIndex);

	void ax12MovementFinished();

	void strategyFinished();

	void serialPortStatusChanged(bool status);
	void startPing();
	void autoStart();

};

#endif // NSERVER_H
