#ifndef GRAPHICSTRATEGY_H
#define GRAPHICSTRATEGY_H

#include <QStringList>
#include <QEvent>
#include <QObject>
#include <QTimer>

#include "StrategyMap.h"
#include "AbstractLogger.h"
#include "NGrid.h"
#include "RobotCommInterface.h"
#include "Instructions.h"

#include "StrategyInterface.h"

class Pather;
class GraphicStrategyTests;
class StrategyDialog;

class TrajectoryFinder : public QObject, public Tools::LoggerInterface
{
    Q_OBJECT

public:
    TrajectoryFinder(Comm::RobotCommInterface* robot, Pather* pather, Tools::AbstractLogger* logger = 0);
    virtual ~TrajectoryFinder();

    /**
      * \brief Initialize the strategy and the pathfinder.
      */
	void init(StrategyMap* map, Tools::NGrid* grid, const Tools::RPoint& startPoint, int replanInterval,
			  int lostReplanInterval, double stopCircleRadius, double diagonalSmoothingMaxDistance, double turnAndMoveMinAngle);

    void findTrajectory(Tools::NGridNode* node, int speed, Tools::Direction dir, Tools::Deplacement deplacementType = Tools::TURN_THEN_MOVE);
    void findTrajectory(Tools::NGridArea* area, int speed, Tools::Direction dir, Tools::Deplacement deplacementType = Tools::TURN_THEN_MOVE);
    void findTrajectory(Tools::NGridArea* area, Tools::NGridNode* node, int speed, Tools::Direction dir, Tools::Deplacement deplacementType = Tools::TURN_THEN_MOVE);

    /**
      * \brief Set manually the trajectory.
      * \param index The index associated to this trajectory. Used with StrategyInterface::actionAt().
      *
      * If the trajectory is not set manually, the pathfinder try to find the best path to the next strategy point.
      */
    void setManualTrajectory(const Tools::Trajectory& trajectory, int speed, Tools::Movement movement, Tools::Deplacement deplacementType = Tools::TURN_THEN_MOVE);

    /**
      * \brief Sets the debug modes.
      * \param intermediates Display data at intermediates points.
      * \param trajectories Display the calculated trajectories.
      */
    void setDebugMode(bool intermediates, bool trajectories);

    void isArrivedAtIntermediatePoint(const Tools::RPoint& pos);

    void setArrived();

    Tools::Direction getDirection() const;
    void setDirection(Tools::Direction d);
    bool hasObjective() const;
    const Tools::RPoint& getObjective() const;

    void setAvoidingEnabled(bool value);
    bool avoidingIsEnabled() const;

    void checkAvoiding();
    void clearAvoiding();

    void cancel();

public slots:
    void start();
    void pause();
    void stop();
    void replanAndStart();
	bool replanTrajectory(); //replan the path to the current objective
    void flushReplanAndStart();
	void flushReplanAndStart(bool stopRobot);

private: 
    bool _debugTrajectories;
    bool _debugIntermediatePoints;

    bool _pause;

    bool _isBlocked;
    bool _forceUnblock; //Sets this variable to true to force the setp out of the blocked planning loop

    int _replanInterval;
    double _stopCircle;
    double _diagSmoothingMaxDist;
	double _turnAndMoveMinAngle;

    Tools::Movement _mouvementType;
	Tools::Deplacement _deplacementType;
	int _forcedNextDeplacementType;

    bool _isArrived;
    bool _avoidingEnabled;
    bool _isManual;
    bool _manualAvoidingInProgress;
    bool _isAvoiding;
    bool _isRelative;
    bool _isPureRotation;
    Tools::Direction _direction;
	int _speed;

    StrategyMap*	_map;
    Comm::RobotCommInterface*	_robot;
    Pather*		_pather;
    QTimer		_lostReplanTimer;

    Tools::Trajectory   _trajectory;     // Points to go to the next key point

	Tools::RPoint        _previousPoint;
    Tools::Trajectory	 _currentDestinations; //Next trajectory points already send to the robots but not reached
    Tools::RPoint	 _objective;
    Tools::NGridNode*   _objectiveNode;
    Tools::NGridArea*   _objectiveArea;
    bool _hasObjective;
    bool _forceStart;
    
    bool planTrajectory(Tools::NGridNode* node, Tools::NGridArea* area);
    bool sendNextTrajectoryPoints();

	void sendMovementToRobot(const Tools::RPoint& point, bool stop, Tools::Movement movementType, Tools::Deplacement deplacementType);

    QRectF getForcedRect(const QPointF& p, double radius) const;

    bool pointsEqualsInPather(const Tools::RPoint& p1, const Tools::RPoint& p2) const;
    bool isArrived(const Tools::RPoint& point) const;

    void resetPatherPosition();
    bool isSingularNode(Tools::NGridNode* node) const;

private slots:
//    void avoidingReplanningSequence();
//    void manualAvoidingSequence();

    void checkRestartAvoiding();
    void enableAvoiding();

signals:
    void objectiveReached();
    void objectiveCanceled();
};

#endif // GRAPHICSTRATEGY_H
