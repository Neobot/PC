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

class StrategyManager : public QObject, public Comm::CommListener, public Tools::LoggerInterface
{
    Q_OBJECT

public:
    StrategyManager(Comm::RobotCommInterface* robot, Pather* pather, Tools::AbstractLogger* logger = 0);
    ~StrategyManager();

	void connectToRobot();
	void disconnectFromRobot();

    void setStrategy(StrategyInterface* strategy);
	
	void setAx12MovementManager(Tools::Ax12MovementManager* movementManager);

    void addAtion(AbstractAction* action);
    void insertAction(int index, AbstractAction* action);
    void insertActionHere(AbstractAction* action);

    bool isLastAction() const;
    bool isFirstAction() const;
    bool isEmpty() const;

    bool movementInProgress() const;
    void cancelCurrentAction();

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
    const Sharp* getOtherSharp(int index) const;
    const QMap<int, Sharp*>& getAvoidingSharps() const;
    const QMap<int, Sharp*>& getOtherSharps() const;

    StrategyMap* getMap() const;
    ActionFactory* getActionFactory() const;
    Tools::NGrid* getGrid() const;

    bool isMirrored() const;
    int getMatchTime();

    int getActionCount() const;

    GameState& getCurrentState();
    double getFuturePathingDistance(const GameState& state, Tools::NGridNode *from, Tools::NGridNode *to);

    void setPause(bool value);

private:    
    ActionFactory*	_actionFactory;
    StrategyInterface*	_strategy;
    TrajectoryFinder*	_trajectoryFinder;
    Comm::RobotCommInterface*	_robot;
    StrategyMap*	_map;
    Tools::NGrid*       _grid;
	Tools::Ax12MovementManager* _ax12MovementManager;
    QMap<int, Sharp*>   _sharps;
    QMap<int, Sharp*>   _scannerSharps;
	QList<bool>			_microswitchStates;
    GameState           _currentState;

    //Objects used to perform calculation on a future state of the game
    Pather*		_futurePather;
    StrategyMap*	_futureMap;

    bool _initDone;
    bool _goAsked;
    bool _autoQuit;
    bool _loop;
    bool _reversed;

    int _currentActionIndex;
    QList<AbstractAction*> _actions;

    bool _debugPositions;
    bool _debugCaptors;

    bool _record;
    QDataStream* _recorder;
    QFile* _recordFile;

	bool init();

    //Action management
    void next();
    void execute(AbstractAction*action);

    //Listener
	void coordinates(qint16 x, qint16 y, double theta, quint8 forward);
    void isArrived();
    void isBlocked();
    void opponentPosition(qint16 x, qint16 y);
	void avoidingSensors(const QList<quint8> &values);
	void microswitchs(const QList<quint8>& values);
	void otherSensors(const QList<quint8>& values);
    bool initDone();
    bool go(bool mirrored);
    bool pingReceived();
    void noticeOfReceipt(quint8 instruction, bool result);
    bool restart();
    void quit();
	void log(const QByteArray& text);

    void updateFutureMap(const GameState& state);

    QTimer _recordTimer;
    QTimer _matchTimer;
    int _matchTime;
    bool _isPaused;

private slots:
    void actionFinished(bool result);
    void record();
    void incrementMatchTime();
	void beginStartSequence();

signals:
    void actionDone(bool result);
	void strategyFinished();
};

#endif // ACTIONMANAGER_H
