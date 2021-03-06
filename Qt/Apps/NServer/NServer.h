#ifndef NSERVER_H
#define NSERVER_H

#include <QObject>
#include <QMap>
#include <QSignalMapper>
#include <QTcpServer>
#include <QSettings>
#include <QSerialPort>

#include "AbstractLogger.h"
#include "NetworkCommInterface.h"
#include "NetworkCommListener.h"
#include "Simulator.h"
#include "Strategies/StrategyEnumerator.h"
#include "Ax12MovementRunner.h"
#include "AX12StatusListener.h"
#include "ServerAX12RequestManager.h"
#include "CommDispatcher.h"
#include "PersistentFilesManager.h"

class Pather;
class NSRunner;

class NServer : public QObject, public Tools::LoggerInterface, public NetworkCommListener, Comm::CommListener
{
	Q_OBJECT

public:
	explicit NServer(Tools::AbstractLogger* logger = 0, QObject *parent = 0);
	~NServer();
	
	bool start();

private:
	QDir _dataDirectory;
	QSettings _settings;
	QSettings _parametersSettings;
	PersistentFilesManager _fileManager;
	QByteArray _autoStartMessage;

	QTcpServer* _tcpServer;
	QMap<unsigned int, NetworkCommInterface*> _connections;
	QList<NetworkCommInterface*> _robotConnections;
	unsigned int _nextConnectionIndex;
	QSignalMapper* _disconnectionMapper;

	Comm::RobotCommInterface* _robotInterface;
	Comm::CommDispatcher _dispatcher;
	Simulator* _simulator;

	StrategyEnumerator _strategiesEnumerator;
	StrategyManager* _strategyManager;
	Pather* _pather;
	StrategyInterface* _currentStrategy;
	int _currentStrategyId;
	CommLogger* _commLogger;

	bool _robotConnected;

	Comm::AX12CommManager* _ax12Manager;
	Tools::Ax12MovementManager _ax12Movements;
    Comm::Ax12MovementRunner* _ax12MovementRunner;
    QHash<NetworkCommInterface*, ServerAX12RequestManager*> _ax12Requests;

	NSRunner* _nsRunner;

    void initServerSettings();

	void sendGlobalAnnoucement(const QByteArray &message);
	void notifyFileChanged(int category, const QString& filename, Comm::SeverFileEvent event, NetworkCommInterface* modifier);

	void cleanRobotConnection();
	void updateRobotConnection();

	//Listener from the client
	bool networkPingReceived();
	void networkNoticeOfReceipt(quint8 instruction, bool result);
	bool connectToRobot(NetworkCommInterface* networkInterface, bool simulation, const QString& robotPort, const QString& ax12Port, QByteArray &message);
	bool disconnectToRobot(NetworkCommInterface* networkInterface);
	bool updateServer(const QByteArray &data);
	QStringList askFiles(int category);
	QByteArray askFileData(int category, const QString& filename);
	void setFileData(int category, const QString& filename, const QByteArray &data, NetworkCommInterface* sender);
	void resetFile(int category, const QString& filename, NetworkCommInterface* sender);

    void askAx12Positions(NetworkCommInterface* networkInterface, const QList<quint8>& ids, bool recursive);
	void moveAx12(float maxSpeed, QList<Comm::Ax12Info>& ax12s);
	void lockAx12(const QMap<quint8, bool>& servoLockInfo);

	bool startStrategy(int strategyNum, bool mirror);
	bool stopStrategy(int& currentStrategyNum) ;
	void askStrategyStatus(int &currentStrategyNum, bool &isRunning);

	int getAutoStrategyDelay() const;
	void askAutoStrategyInfo(bool &isEnabled, int &strategyNum, QString &robotPort, QString &ax12Port, bool &simulation, bool &mirror, int& startDelay);
	void setAutoStrategy(bool enabled, int strategyNum, const QString &robotPort, const QString &ax12Port, bool simulation, bool mirror, int startDelay);
	
	QByteArray askAx12MovementFile();
	void setAx12MovementFile(const QByteArray& data);
    void runAx12Movement(const QString& group, const QString& movement, float speedLimit, int lastPositionIndex);

	void saveParameters(const QList<float>& values);
	QList<float> getParameters();
	void resetParameters();

	bool runScript(const QByteArray& script);

	//Listener from the robot
	void initReceived();

private slots:
	void newConnection();
	void removeConnection(int connectionIndex);

	void ax12MovementFinished();
	void strategyFinished();
	void scriptFinished();

	void closeRobotConnection();
	void handleSerialError(QSerialPort::SerialPortError error);

	void autoStart();
	void sendParameters();
};


#endif // NSERVER_H
