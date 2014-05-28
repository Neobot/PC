#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "SimRobot.h"

#include <QObject>
#include <QTimer>
#include <ToolsLib.h>
#include <RMovement.h>
#include <RPoint.h>
#include "SimulationMap.h"
#include <Instructions.h>


enum SubAction
{
    NO_SUBACTION = -1,
    MOVING = 4
};

enum SubMovement
{
    MOVEMENT_ROTATE_FORWARD,
    MOVEMENT_ROTATE_BACKWARD,
    MOVEMENT_MOVE_FORWARD,
    MOVEMENT_MOVE_BACKWARD
};


class Simulator : public QObject, public SimCommInterface, public Tools::LoggerInterface
{
    Q_OBJECT

    class RSubMovement
    {
    public:
        RSubMovement(const Tools::RPoint& destination = Tools::RPoint(), SubMovement type = MOVEMENT_ROTATE_FORWARD, int speed = 100);
        RSubMovement(const RSubMovement& M);

        const Tools::RPoint& getDestination() const;
        SubMovement getType() const;
        int getSpeed() const;

    private:
        Tools::RPoint _destination;
        SubMovement _type;
        int _speed; //percentage
    };

public:
    enum PlayableColor
    {
        Red,
        Yellow
    };

    Simulator(SimProtocol* protocol, int interval, double speed, Tools::AbstractLogger* logger = 0);
    ~Simulator();

    void setConfiguration(const ChessUpConfiguration& config);

    void start();
    void pause();
    void stop();

    void block(); //Simulates an obstacle which block the robot

    void setRobotDefaultColor(PlayableColor color);
	void setRobotDefaultPosition(bool mirrored);
    void addOpponentMovement(const QPointF& point);
    Tools::RPoint getDefaultRobotPosition();

    double getSpeed() const;
    void setSpeed(double speed);

    SimulationMap* getMap() const;

	void setPosition(qint16 x, qint16 y, double theta);
	void addDestination(qint16 x, qint16 y, double theta, quint8 type, quint8 deplacementType, bool isStopPoint, int speed);
	void executeAction(int actionName, quint16 parameter);
	void flush();
	void askParameters();
	void setParameters(const QList<float>& values);

private:
    QTimer _timer;
    SimRobot* _simRobot;	//comm

    SimulationMap* _map;
    Tools::RPoint _defaultRobotPos;
    Tools::RPoint _defaultOpponentPos;
	Tools::RPoint _currentObjectve;

    Tools::RMovement _destination;
    QQueue<Tools::RMovement> _fifo;
    QQueue<RSubMovement> _subMovements;

    QQueue<QPointF> _opponentMovements;

    double _d, _r;
    int _interval;
    bool _isAtBeginning;
    PlayableColor _color;

	int _currentAction;
    QList<SubAction> _subActionList;
    int _subActionDuration[4];
    int _currentSubActionDuration;
    double _actionDistanceLeft;

    bool _clawsSharp;

    bool _isBlocked;
    bool _moveForward;

    void updateRobotPosition();
    void updateOpponentPosition();
    void processAction();
    void prepareNextMovement();
    void initSimulation();


private slots:
	void simulationTimerEvent();

};

#endif // SIMULATOR_H
