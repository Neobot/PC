#include "TrajectoryFinder.h"
#include <QTimer>
#include "Instructions.h"
#include "ToolsLib.h"
#include "Pather.h"

#include <cmath>
#include "vmath.h"
#include <QtDebug>

using namespace Comm;
using namespace Tools;

const int FORECAST_POINTS = 2;

TrajectoryFinder::TrajectoryFinder(Comm::RobotCommInterface* robot, Pather* pather, Tools::AbstractLogger* logger)
	: LoggerInterface(logger), _debugTrajectories(false), _debugIntermediatePoints(true),
	  _pause(false), _isBlocked(false), _forceUnblock(false), _isArrived(false),
	  _avoidingEnabled(true), _manualAvoidingInProgress(false), _isAvoiding(false),
	  _map(0), _robot(robot), _pather(pather),
	  _previousPoint(-1000, -1000), _hasObjective(false), _forceStart(false)
{
	_lostReplanTimer.setSingleShot(true);
	connect(&_lostReplanTimer, SIGNAL(timeout()), this, SLOT(flushReplanAndStart()));
}

TrajectoryFinder::~TrajectoryFinder()
{
}

void TrajectoryFinder::init(StrategyMap* map, Tools::NGrid* grid, const Tools::RPoint &startPoint, int replanInterval,
							int lostReplanInterval, double stopCircleRadius, double diagonalSmoothingMaxDistance, double turnAndMoveMinAngle)
{
	_forcedNextDeplacementType = -1;
	_isArrived = false;
	_isBlocked = false;
	_forceUnblock = true;
	_hasObjective = false;
	_forceStart = false;
	_previousPoint = Tools::RPoint(-1000, -1000);
	_manualAvoidingInProgress = false;
	_pause = false;

	_replanInterval = replanInterval;
	_stopCircle = stopCircleRadius;
	_diagSmoothingMaxDist = diagonalSmoothingMaxDistance;
	_turnAndMoveMinAngle = turnAndMoveMinAngle;

	_lostReplanTimer.setInterval(lostReplanInterval);

	_map = map;
	
	_deplacementType = Tools::TURN_THEN_MOVE;
	_speed = 50;

	_trajectory.clear();
	_currentDestinations.clear();
	_previousPoint = RPoint(-1000, -1000);

	//int Pather
	NGridNode* startNode = grid->getNearestNode(startPoint.toQPointF());
	_pather->init(_map, grid, startNode);
}

QRectF TrajectoryFinder::getForcedRect(const QPointF& p, double radius) const
{
	Q_ASSERT(radius > 0);

	QPointF br = p;
	br.rx() -= radius;
	br.ry() -= radius;

	return QRectF(br, QSizeF(radius*2, radius*2));
}

bool TrajectoryFinder::planTrajectory(Tools::NGridNode* node, NGridArea *area)
{
	Tools::RPoint robotPosition = _map->getRobotPosition();

	QList<QRectF> forcedRects;
	if (_forceStart)
	{
		forcedRects << getForcedRect(_map->getRobot()->getPosition(), _map->getRobot()->getRadius());
		if (_debugTrajectories)
			logger() << "START FORCED\n";
	}

	if (node && area)
		_pather->prepareReplan(area, node, forcedRects);
	else if (node)
		_pather->prepareReplan(node, forcedRects);
	else if (area)
		_pather->prepareReplan(area, forcedRects);


	_currentDestinations.clear();
	_trajectory.clear();

	_pather->replanTrajectory(_trajectory);

	if (_debugTrajectories)
	{
		logger() << "Trajectory before smoothing:"  << Tools::endl;
		Tools::displayTrajectory(_trajectory, &logger());
	}

	bool ok = !_trajectory.isEmpty();


	//Check first and last point
	if(ok)
	{
		if (_trajectory.count() > 1)
		{
			if (pointsEqualsInPather(_trajectory.first(), robotPosition))
			{
				_trajectory.removeFirst();
				if (_debugTrajectories)
					logger() << "Removing the first point because it is the current position." << Tools::endl;
			}

			//The trajectory has probably been cut down by the area
			//Reset the objective
			if (area)
				_objective = _trajectory.last();
			else if (node)
			{
				RPoint destinationPoint(node->getPosition());
				if (!pointsEqualsInPather(_trajectory.last(), destinationPoint))
				{
					_trajectory.removeLast();
					_trajectory << destinationPoint;
					if (_debugTrajectories)
						logger() << "Arriving far from the objective. Add it at the end of the trajectory." << Tools::endl;
				}
			}

		}
	}

	//smoothing
	Tools::trajectoryStraightLineSmoothing(_trajectory);
	Tools::trajectoryDiagonalSmoothing(_trajectory, _diagSmoothingMaxDist);

	if (_debugTrajectories)
	{
		logger() << "Trajectory after smoothing:"  << Tools::endl;
		Tools::displayTrajectory(_trajectory, &logger());

		logger() << "New trajectory of "  << _trajectory.count() << " points calculated." << Tools::endl;
	}



	if (!_trajectory.isEmpty())
	{
		if (!pointsEqualsInPather(robotPosition, _trajectory.first()))
			_previousPoint = robotPosition;
		else
			_previousPoint = RPoint(-1000, -1000);
	}

	return !_trajectory.isEmpty();
}

bool TrajectoryFinder::replanTrajectory()
{
	if (!_hasObjective)
		return false;

	if (_forceUnblock)
	{
		_forceUnblock = false;
		if (_isBlocked)
		{
			_isBlocked = false;
			return true;
		}
	}

	if (!planTrajectory(_objectiveNode, _objectiveArea))
	{
		_isBlocked = true;
		if (_debugTrajectories)
			logger() << "Planning failed. New attempt in "  << _replanInterval << "ms." << Tools::endl;

		QTimer::singleShot(_replanInterval, this, SLOT(replanAndStart()));

		//TODO: try to go to the closest accessible node

		return false;
	}

	return true;
}

void TrajectoryFinder::start()
{
	_pause = false;

	if (!sendNextTrajectoryPoints())
	{
		logger() << "Hmmm... No trajectory has been set. I can't go anywhere." << Tools::endl;
	}
}

void TrajectoryFinder::replanAndStart()
{
	if (hasObjective() && replanTrajectory())
		start();
}

void TrajectoryFinder::flushReplanAndStart()
{
	//_previousPoint = _map->getRobotPosition(); //RPoint(-1000, -1000);
	_robot->flush();
	resetPatherPosition();
	_currentDestinations.clear();
	replanAndStart();
}


void TrajectoryFinder::pause()
{
	logger() << "Trajectory finder break !!!" << Tools::endl;
	_pause = true;
}

void TrajectoryFinder::stop()
{
	_hasObjective = false;
	_trajectory.clear();
}

void TrajectoryFinder::setDebugMode(bool intermediates, bool trajectories)
{
	_debugIntermediatePoints = intermediates;
	_debugTrajectories = trajectories;
}

void TrajectoryFinder::sendMovementToRobot(const RPoint& point, bool stop, Tools::Movement movementType, Tools::Deplacement deplacementType)
{
	double theta = Tools::angleInZeroTwoPi(point.getTheta());
	Tools::RMovement pos(RPoint(point.getX(), point.getY(), theta), movementType, deplacementType, stop, _speed);

	_robot->setDestination(pos);
}

bool TrajectoryFinder::sendNextTrajectoryPoints()
{
	while(_currentDestinations.count() < FORECAST_POINTS && !_trajectory.isEmpty())
	{
		RPoint p = _trajectory.takeFirst();

		bool isStopPoint = false;
		if (_trajectory.isEmpty())
		{
			isStopPoint = true;
			_forcedNextDeplacementType = -1;
		}
		else
		{
			RPoint next = _trajectory.first();
			double angleToNext = Tools::angleInMinusPiPlusPi(_previousPoint.angle(p)) - Tools::angleInMinusPiPlusPi(p.angle(next));
			angleToNext = Tools::angleInMinusPiPlusPi(angleToNext);
			isStopPoint = angleToNext < -_turnAndMoveMinAngle || angleToNext > _turnAndMoveMinAngle;
			if (isStopPoint && _deplacementType == Tools::TURN_AND_MOVE)
				_forcedNextDeplacementType = Tools::TURN_THEN_MOVE;
		}

		Tools::Deplacement depType = _deplacementType;
		if (_forcedNextDeplacementType >= 0)
		{
			depType = (Tools::Deplacement)_forcedNextDeplacementType;
			_forcedNextDeplacementType = -1;
		}

		sendMovementToRobot(p, isStopPoint, _mouvementType, depType);

		if (!_isManual)
			_lostReplanTimer.start();
		_currentDestinations << p;
		if (_debugIntermediatePoints)
		{
			logger() << "Send the robot in point (" << p.getX() << "," << p.getY() << "," << Tools::angleInZeroTwoPi(p.getTheta()) << ") ";
			if (_currentDestinations.isEmpty())
				logger() << " and stop.";
			logger() << Tools::endl;
		}
	}

	return !_currentDestinations.isEmpty();
}

bool TrajectoryFinder::isArrived(const Tools::RPoint &point) const
{
	if (!_hasObjective || _currentDestinations.isEmpty())
	{
		return false;
	}

	if ((_isPureRotation && _mouvementType == Tools::TOURNE_VERS_XY) || (_mouvementType == Tools::TOURNE_VERS_XY_AR))
	{
		if (fabs(point.angle(_currentDestinations.first()) - point.getTheta()) < 0.2)
			return true;
	}

	if (_isPureRotation &&  fabs(point.diffAngle(_currentDestinations.first())) < 0.2)
		return true;

	if (!_isPureRotation && point.isInCircle(_currentDestinations.first(), 40))
		return true;


	return false;
}

void TrajectoryFinder::isArrivedAtIntermediatePoint(const Tools::RPoint& pos)
{
	if (!_hasObjective || _manualAvoidingInProgress)
	{
		return;
	}

	//    if (!_isPureRotation && pointsEqualsInPather(pos, _previousPoint))
	//    {
	//        logger() << "WARNING: Not really arrived... current=(" << pos << ") previous=(" << _previousPoint << ")" << Tools::endl;
	//	return; //not really arrived
	//    }
	//    else if (_isPureRotation && fabs(pos.diffAngle(_previousPoint)) < 0.1)
	//    {
	//        logger() << "WARNING: Rotation : not really arrived... current=(" << pos << ") previous=(" << _previousPoint << ")" << Tools::endl;
	//        return; //not really arrived
	//    }


	if (_currentDestinations.isEmpty())
	{
		//logger() << "WARNING: The robot is arrived at an unforcasted position !!!" << Tools::endl;
		return;
	}

	if (_isRelative || !_isArrived)//!isArrived(pos))
		return;

	_isArrived = false;

	_lostReplanTimer.stop();

	RPoint arrivedPoint = _currentDestinations.first();
	_previousPoint = arrivedPoint;
	if (!_isManual && !pointsEqualsInPather(pos, _currentDestinations.first()))
	{
		logger() << "WARNING: The robot say that it is arrived but it's look like is not." << Tools::endl;
		logger() << "Expected point is Point(" << arrivedPoint.getX() << "," << arrivedPoint.getY() << ")" << Tools::endl;
	}


	_currentDestinations.takeFirst();

	if (_debugIntermediatePoints)
	{
		logger() << "Intermediate point."
				 << " Current=" << pos
				 << " Intermediate=" << arrivedPoint
				 << " Objective=" << _objective
				 << Tools::endl;
	}

	if (_objective.toPoint() == arrivedPoint.toPoint())
	{
		_hasObjective = false;
		if (_debugTrajectories)
			logger() << "Objective reached..." << Tools::endl;
		if (_isManual)
			resetPatherPosition();
		emit objectiveReached(); //The trajectory is finished
	}
	else if (!_pause)
		sendNextTrajectoryPoints();
}

void TrajectoryFinder::setArrived()
{
	if (_hasObjective)
		_isArrived = true;
}

Direction TrajectoryFinder::getDirection() const
{
	return _direction;
}

void TrajectoryFinder::setDirection(Direction d)
{
	_direction = d;
}

void TrajectoryFinder::findTrajectory(Tools::NGridNode* node, int speed, Tools::Direction dir, Tools::Deplacement deplacementType)
{
	findTrajectory(0, node, speed, dir, deplacementType);
}

void TrajectoryFinder::findTrajectory(NGridArea *area, int speed, Direction dir, Tools::Deplacement deplacementType)
{
	findTrajectory(area, 0, speed, dir, deplacementType);
}

void TrajectoryFinder::findTrajectory(NGridArea *area, NGridNode *node, int speed, Direction dir, Tools::Deplacement deplacementType)
{
	_isArrived = false;
	_forceStart = false;
	_isManual = false;
	_hasObjective = true;
	_objectiveArea = area;
	_objectiveNode = node;
	if (node)
		_objective = RPoint(node->getPosition());
	else
		_objective = RPoint(area->getRect().center());
	_isRelative = false;
	_isPureRotation = false;

	_direction = dir;
	if (dir == Forward)
		_mouvementType = AVANT_XY;
	else if (dir == Backward)
		_mouvementType = ARRIERE_XY;
	else
		_mouvementType = AUTO;
		
	_deplacementType = deplacementType;
	_speed = speed;

	replanAndStart();
}


void TrajectoryFinder::setManualTrajectory(const Tools::Trajectory &trajectory, int speed, Tools::Movement movement, Tools::Deplacement deplacementType)
{
	if (!trajectory.isEmpty())
	{
		_isArrived = false;
		_isManual = true;
		_hasObjective = true;
		_objectiveNode = 0;
		_objectiveArea = 0;
		_objective = trajectory.last();
		_mouvementType = movement;
		_deplacementType = deplacementType;
		_speed = speed;

		_isRelative = RMovement::isRelative(_mouvementType);
		_isPureRotation = RMovement::isPureRotation(_mouvementType);
		_direction = RMovement::getMouvementDirection(_mouvementType);

		_trajectory = trajectory;

		start();
	}
}

const Tools::RPoint & TrajectoryFinder::getObjective() const
{
	return _objective;
}

bool TrajectoryFinder::hasObjective() const
{
	return _hasObjective;
}

void TrajectoryFinder::checkAvoiding()
{
	if (!_avoidingEnabled)
		return;

	if (_hasObjective && !_currentDestinations.isEmpty())
	{
		if (!_isManual)
		{
			if (!_isAvoiding && _map->checkNextTrajectoryForCollision(_currentDestinations.first()))
			{
				_isAvoiding = true;
				QTimer::singleShot(300, this, SLOT(enableAvoiding()));
			}
			else
			{
				if (_debugTrajectories)
					logger() << "No flushing here.........." << Tools::endl;
			}
		}
		else if (!_isPureRotation)
		{
			if (!_manualAvoidingInProgress)
			{
				if (_debugTrajectories)
					logger() << "Manual avoiding......" << Tools::endl;
				_robot->flush();


				for(Tools::Trajectory::const_iterator it = _currentDestinations.constEnd() - 1; it != _currentDestinations.constBegin() - 1; --it)
					_trajectory.prepend(*it);

				if (_isRelative && !_trajectory.isEmpty())
					_trajectory.removeFirst();

				_currentDestinations.clear();

				_manualAvoidingInProgress = true;

				QTimer::singleShot(_replanInterval, this, SLOT(checkRestartAvoiding()));
			}
		}
	}
}

void TrajectoryFinder::enableAvoiding()
{
	if (_debugTrajectories)
		logger() << "Avoiding......" << Tools::endl;
	flushReplanAndStart();
	_isAvoiding = false;
}

void TrajectoryFinder::clearAvoiding()
{
	_manualAvoidingInProgress = false;
}

void TrajectoryFinder::cancel()
{
	_robot->flush();
	_currentDestinations.clear();
	_hasObjective = false;
	//_isOnSingulariy = false;
	resetPatherPosition();
	emit objectiveCanceled();
}

void TrajectoryFinder::checkRestartAvoiding()
{
	if (!_manualAvoidingInProgress)
	{
		if (!_isRelative)
			start();
		else
		{
			resetPatherPosition();
			emit objectiveReached();
		}
	}
	else
		QTimer::singleShot(_replanInterval, this, SLOT(checkRestartAvoiding()));
}

bool TrajectoryFinder::pointsEqualsInPather(const Tools::RPoint &p1, const Tools::RPoint &p2) const
{
	return p1.isInCircle(p2, 50); //TODO, make 50 a parameter (minStopRadius)
}

void TrajectoryFinder::setAvoidingEnabled(bool value)
{
	_avoidingEnabled = value;
}

bool TrajectoryFinder::avoidingIsEnabled() const
{
	return _avoidingEnabled;
}

void TrajectoryFinder::resetPatherPosition()
{
	_pather->setCurrentNode(_pather->getGrid()->getNearestNode(_map->getRobot()->getPosition()));
}

bool TrajectoryFinder::isSingularNode(NGridNode *node) const
{
	return node->getReachableNodes().count() == 1 && node->getRevertedConnections().count() == 1;
}

