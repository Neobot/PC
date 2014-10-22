#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <CommListener.h>
#include <AbstractLogger.h>
#include "ActionFactory.h"
#include "Sharp.h"
#include "GameState.h"

#include <QObject>
#include <QQueue>
#include <QMap>
#include <QTimer>
#include <QDir>


namespace Comm
{
    class RobotCommInterface;
}

namespace Tools
{
	class Ax12MovementManager;
}

class StrategyInterface;
class Pather;
class TrajectoryFinder;
class NSRunner;

class StrategyManager : public QObject, public Comm::CommListener, public Tools::LoggerInterface
{
    Q_OBJECT
	friend class NSRunner;

public:
    StrategyManager(Comm::RobotCommInterface* robot, Pather* pather, Tools::AbstractLogger* logger = 0);
    ~StrategyManager();

	void connectToRobot();
	void disconnectFromRobot();

    void addScriptsSearchDirectory(const QDir& dir);

    void setStrategy(StrategyInterface* strategy);
	
	void setAx12MovementManager(Tools::Ax12MovementManager* movementManager);

    void addAtion(AbstractAction* action);
	void addActionsFromScript(const QString &scriptCode);
    void insertAction(int index, AbstractAction* action);
    void insertActionHere(AbstractAction* action);

    bool isLastAction() const;
    bool isFirstAction() const;
    bool isEmpty() const;

    bool isRunning() const;

    bool movementInProgress() const;
    void cancelCurrentAction();
	void stopStrategy();

    /**
      * \brief Enable or disable auto-quit.
      * \details If auto-quit is enabled, the program will stop when there is no more objectives.
      * By default, it is disabled.
      */
    void setAutoQuit(bool value);

    /**
      * \brief Sets the debug modes.
      * \param intermediates Display data at intermediates points.
      * \param positions Display all positions of the robot and the opponent.
      * \param trajectories Display the calculated trajectories.
      */
    void setDebugMode(bool intermediates, bool positions, bool trajectories, bool captors);

    void setReversedStrategy(bool value);

    void startRecording(const QString& filename);
    void stopRecording();
    bool isRecording();

    const Sharp* getAvoidingSharp(int index) const;
	const QMap<int, Sharp*>& getAvoidingSharps() const;
	
    const Sensor* getOtherSharp(int index) const;
    const Sensor* getColorSensor(int index) const;
	const Sensor* getMicroswitch(int index) const;
    
    const QMap<int, Sensor*>& getOtherSharps() const;
    const QMap<int, Sensor *> &getColorSensors() const;
	const QMap<int, Sensor *> &getMicroswitchs() const;

	const Sensor *getSensor(int index, Comm::SensorType type) const;

	double getParameter(int index) const;
	QString getParameterName(int index) const;
	void setParameter(int index, double value);

    StrategyMap* getMap() const;
    ActionFactory* getActionFactory() const;
    Tools::NGrid* getGrid() const;

    bool isMirrored() const;
    int getMatchTime();

    int getActionCount() const;

    GameState& getCurrentState();
    double getFuturePathingDistance(const GameState& state, Tools::NGridNode *from, Tools::NGridNode *to);

	QList<float> params;

private:    
    ActionFactory*	_actionFactory;
    StrategyInterface*	_strategy;
    TrajectoryFinder*	_trajectoryFinder;
    Comm::RobotCommInterface*	_robot;
    StrategyMap*	_map;
    Tools::NGrid*       _grid;
	Tools::Ax12MovementManager* _ax12MovementManager;
    QMap<int, Sharp*>   _sharps;
    QMap<int, Sensor*>   _scannerSharps;
    QMap<int, Sensor*>   _colorSensors;
	QMap<int, Sensor*>   _microswitchs;
    GameState           _currentState;
	QList<float>		_parameters;
	QStringList			_parameterNames;

    QList<QDir> _scriptsSearchDirectories;

    //Objects used to perform calculation on a future state of the game
    Pather*		_futurePather;
    StrategyMap*	_futureMap;

    bool _initDone;
    bool _autoQuit;
    bool _loop;
    bool _reversed;

    int _currentActionIndex;
    QList<AbstractAction*> _actions;
    bool _isRunning;

    bool _debugPositions;
    bool _debugCaptors;

    bool _record;
    QDataStream* _recorder;
    QFile* _recordFile;

	bool init();

    //Action management
    void next();
    void execute(AbstractAction*action);
	ActionFactory* createActionFactory();

    //Listener
	void coordinates(qint16 x, qint16 y, double theta, quint8 forward);
    void opponentPosition(qint16 x, qint16 y);
	void activatedSensors(const QList<quint8> &values);
    bool go(bool mirrored);
    bool pingReceived();
    void noticeOfReceipt(quint8 instruction, bool result);
	void log(const QByteArray& text);
	void parameters(const QList<float>& values);
	void parameterNames(const QStringList& names);
	void robotEvent(Comm::RobotEvent event);
	void sensorEvent(Comm::SensorType type, int sensorId, int value);
	
	//Others
	bool restart();
	void quit();
	void isArrived();
    void isBlocked();

    void updateFutureMap(const GameState& state);

    QTimer _recordTimer;
    QTimer _matchTimer;
    int _matchTime;


private slots:
    void actionFinished(bool result);
    void record();
    void incrementMatchTime();
	void beginStartSequence();

signals:
    void actionDone(bool result);
	void strategyFinished();

	void sensorStateChanged(Comm::SensorType type, int sensorId, int state);
};

#endif // ACTIONMANAGER_H
