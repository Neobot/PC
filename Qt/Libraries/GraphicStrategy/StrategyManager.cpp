#include "StrategyManager.h"

#include <QCoreApplication>
#include <QtDebug>

#include "TrajectoryFinder.h"
#include "RobotCommInterface.h"
#include "ToolsLib.h"
#include "NMicropather.h"

using namespace Tools;

StrategyManager::StrategyManager(Comm::RobotCommInterface* robot, Pather* pather, Tools::AbstractLogger* logger) :
	LoggerInterface(logger),  _strategy(0), _robot(robot), _map(0), _ax12MovementManager(0), _initDone(false), _goAsked(false), _autoQuit(false),
	_loop(false), _reversed(false), _debugPositions(false), _debugCaptors(false), _record(false), _matchTime(0), _isPaused(false)
{
	_actionFactory = 0;
	_trajectoryFinder = new TrajectoryFinder(robot, pather, logger);

	_currentActionIndex = 0;

	_recorder = 0;
	_recordFile = 0;

	connect(&_recordTimer, SIGNAL(timeout()), this, SLOT(record()));
	_recordTimer.setInterval(100);


	connect(&_matchTimer, SIGNAL(timeout()), this, SLOT(incrementMatchTime()));
	_matchTimer.setInterval(1000);

	_futureMap = 0;
	_futurePather = 0;
}

StrategyManager::~StrategyManager()
{
	delete _trajectoryFinder;
	delete _futureMap;

	qDeleteAll(_actions);
	_actions.clear();

	qDeleteAll(_sharps);
	qDeleteAll(_scannerSharps);

	delete _recorder;
	delete _recordFile;
}

void StrategyManager::connectToRobot()
{
	_robot->setListener(this);
	_robot->ping();
}

void StrategyManager::disconnectFromRobot()
{
	_robot->setListener(0);
}

void StrategyManager::setStrategy(StrategyInterface* strategy)
{
	_strategy = strategy;
	_trajectoryFinder->stop();
}

void StrategyManager::setAx12MovementManager(Tools::Ax12MovementManager* movementManager)
{
	_ax12MovementManager = movementManager;
}

bool StrategyManager::init()
{
	if (!_strategy)
	{
		logger() << "ERROR: No strategy has been set !!!" << Tools::endl;
		return false;
	}

	if (!_strategy->load(this, _reversed))
	{
		logger() << "ERROR: The strategy cannot be loaded !!!" << Tools::endl;
		return false;
	}

	_goAsked = false;
	_initDone = false;

	const StrategyInterface::StrategyParameters& parameters = _strategy->getParameters();
	_grid = _strategy->getGrid();
	_loop = parameters.loop;

	//init DataMap
	if (!_map)
		_map = new StrategyMap(parameters.tableSize, parameters.robotRadius, parameters.opponentRadius);
	else
	{
		_map->clear();
		_map->init(parameters.tableSize, parameters.robotRadius, parameters.opponentRadius);
	}

	_map->setSharpObjectsRadius(parameters.sharpObjectRadius, parameters.sharpDetectionOverlapRadius);
	_map->setSharpsDetectionTime(parameters.sharpDetectionTime);
	_map->setNoDetectionZones(parameters.noDetectionZones);

	//init Future object
	if (!_futureMap)
	{
		_futureMap = new StrategyMap(parameters.tableSize, parameters.robotRadius, parameters.opponentRadius);
		_futurePather = new NMicropather(0, NMicropather::Manhattan);
	}
	else
	{
		_futureMap->clear();
		_futureMap->init(parameters.tableSize, parameters.robotRadius, parameters.opponentRadius);
	}

	_futureMap->setSharpObjectsRadius(parameters.sharpObjectRadius, parameters.sharpDetectionOverlapRadius);
	_futureMap->setSharpsDetectionTime(parameters.sharpDetectionTime);
	_futurePather->init(_futureMap, _grid, 0);


	//init obstacles
	QList<DataObject*> objects;
	_strategy->defaultObstaclePositions(objects);
	foreach(DataObject* c, objects)
		_map->addObstacle(c);

	//init start position
	Tools::RPoint startPoint = _strategy->getStartPosition();
	_map->moveRobot(startPoint);

	_robot->setNoticeOfReceiptTimeOut(parameters.noticeOfReceiptTimeOut);
	_trajectoryFinder->init(_map, _grid, startPoint, parameters.replanInterval, parameters.lostReplanInterval,
							parameters.stopCircleRadius, parameters.diagonalSmoothingMaxDistance, parameters.turnThenMoveMinAngle);
	_trajectoryFinder->setAvoidingEnabled(parameters.enableAutoAvoiding);

	//init actions
	qDeleteAll(_actions);
	_actions.clear();
	_currentActionIndex = 0;

	if (_actionFactory)
		delete _actionFactory;

	_actionFactory = new ActionFactory(this, _trajectoryFinder, _map, _robot, _ax12MovementManager);
	_strategy->mainStrategy(_actions);

	//init sharps
	_sharps.clear();
	_strategy->avoidingSharps(_sharps);

	_scannerSharps.clear();

	_initDone = true;
	logger() << "Init done. Start your engines..." << Tools::endl;

	return true;
}

bool StrategyManager::go(bool mirrored)
{
	setReversedStrategy(mirrored);
	logger() << (mirrored ? "YELLOW STRATEGY" : "RED STRATEGY") << Tools::endl;

	if (!init())
		return false;

	_goAsked = true;
	if (!_initDone)
	{
		logger() << "Can't go now. Waiting for PC Initialisation !" << Tools::endl;
	}
	else
	{
		beginStartSequence();
	}
	return true;
}

void StrategyManager::beginStartSequence()
{
	logger() << "Go go go !" << Tools::endl;

	_matchTimer.start();
	_matchTime = 0;

	const RPoint& startPoint = _map->getRobotPosition();
	double theta = Tools::angleInZeroTwoPi(startPoint.getTheta());
	_robot->setPosition(startPoint.getX(), startPoint.getY(), theta);

	next();
}

void StrategyManager::coordinates(qint16 x, qint16 y, double theta, quint8 forward)
{
	if (!_initDone)
		return;

	RPoint current(x, y, theta);
	_map->moveRobot(current);

	if (_debugPositions)
	{
		logger() << "I'm at the point " << current;

		if (_trajectoryFinder->hasObjective())
			logger() << " - Still " << current.distance(_trajectoryFinder->getObjective().toPoint()) << " mm to go.";

		logger() << Tools::endl;
	}



	_trajectoryFinder->setDirection((Tools::Direction)forward);
	_trajectoryFinder->isArrivedAtIntermediatePoint(current);
}

void StrategyManager::isArrived()
{
	_trajectoryFinder->setArrived();
}

void StrategyManager::isBlocked()
{
	return;

	QVector2D dir(_trajectoryFinder->getDirection() == Tools::Forward ? 1 : -1, 0);
	QPointF p = _map->getSharpDetectionPoint(QPointF(0,0), dir, _map->getRobot()->getRadius());
	bool ok = _map->addTemporaryBlockingSharpObject(p);
	_futureMap->addTemporaryBlockingSharpObject(p);

	if (!ok)
		logger() << "Blocked !!!" << Tools::endl;

	_strategy->blockingDeteced();
}

void StrategyManager::opponentPosition(qint16 x, qint16 y)
{
	if (_debugCaptors)
		logger() << "Ennemy spotted in " << x << "," << y << Tools::endl;

	_map->moveOpponent(QPointF(x, y));
}

void StrategyManager::robotEvent(Comm::RobotEvent event)
{
	switch(event)
	{
		case Comm::EVENT_IS_ARRIVED: isArrived(); break;
		case Comm::EVENT_IS_BLOCKED: isBlocked(); break;
		case Comm::EVENT_RESTART:	 restart(); break;
		case Comm::EVENT_QUIT:		 quit(); break;
		case Comm::NO_EVENT: break;
	}
}

void StrategyManager::sensorEvent(Comm::SensorType type, int sensorId, int value)
{
	QMap<int, Sensor*>* family = nullptr;
	switch(type)
	{
		case Comm::SharpSensor: 			family = &_scannerSharps; break;
		case Comm::MicroswitchSensor:		family = &_microswitchs; break;
		case Comm::ColorSensor:  			family = &_colorSensors; break;
	}
	
	if (family)
	{
		Sensor* sensor = family->value(sensorId, nullptr);
		if (!sensor)
		{
			sensor = new Sensor;
			family->insert(sensorId, sensor);
		}
		
		sensor->setState(value);
		emit sensorStateChanged(type, sensorId, value);
	}
}

void StrategyManager::avoidingSensors(const QList<quint8> &values)
{
	if (!_initDone)
		return;

	//Update the sharps values and check if they are activated
	int index = 0;
	QMap<int, const Sharp*> activatedSharps;

	for(QList<quint8>::const_iterator it = values.constBegin(); it != values.constEnd(); ++it)
	{
		quint8 value = *it;

		Sharp* sharp = _sharps.value(index);
		if (!sharp)
			break;

		sharp->update(value);

		if (_trajectoryFinder->hasObjective() &&
			sharp->isActive() &&
			sharp->getMovementDirection() == _trajectoryFinder->getDirection())
		{
			activatedSharps[index] = sharp;
		}

		index++;
	}

	//Get detection points for activated sharps
	bool avoidingNecessary = false;
	bool hasAvoidingSharp = !activatedSharps.isEmpty();

	QList<QPointF> obstacles = _strategy->doDetection(activatedSharps);
	foreach(const QPointF& o, obstacles)
	{
		bool sharpObjectAdded = _map->addTemporarySharpObject(o);
		avoidingNecessary = sharpObjectAdded | avoidingNecessary;

		if (sharpObjectAdded && _trajectoryFinder->avoidingIsEnabled())
			logger() << "Object spotted in " << o << ", Robot is in " <<  _map->getRobot()->getPosition() << Tools::endl;

		_futureMap->addTemporarySharpObject(o);
	}

	//Launch the avoiding if necessary
	if (_trajectoryFinder->avoidingIsEnabled())
	{
		if (avoidingNecessary)
			_trajectoryFinder->checkAvoiding();
		else if (!hasAvoidingSharp)
			_trajectoryFinder->clearAvoiding();
	}
	else if (avoidingNecessary)
		_strategy->obstacleDetected();
}

bool StrategyManager::pingReceived()
{
	logger() << "Ping received ..." << Tools::endl;
	return true;
}

void StrategyManager::noticeOfReceipt(quint8 instruction, bool result)
{
	if(instruction == Comm::ACTIONS)
	{
		emit actionDone(result);
		//if (instruction == Comm::DEST_ADD && getNextPoint(Current))
		//    sendPositionToRobot(*getNextPoint(Current));
		//TODO
	}
}

bool StrategyManager::restart()
{
	logger() << "Restarting program..." << Tools::endl;
	_robot->ping();
	if (!init())
		return false;

	return true;
}

void StrategyManager::quit()
{
	logger() << "See you space robot..." << Tools::endl;
	qApp->quit();
}

void StrategyManager::log(const QByteArray &text)
{
	logger() << "The robot say \"" << text << "\"" << Tools::endl;
}

void StrategyManager::parameters(const QList<float> &values)
{
	_parameters = values;
}

void StrategyManager::parameterNames(const QStringList &names)
{
	_parameterNames = names;
}

void StrategyManager::setAutoQuit(bool value)
{
	_autoQuit = value;
}

void StrategyManager::setDebugMode(bool intermediates, bool positions, bool trajectories, bool captors)
{
	_debugPositions = positions;
	_debugCaptors = captors;
	_trajectoryFinder->setDebugMode(intermediates, trajectories);
}

void StrategyManager::addAtion(AbstractAction *action)
{
	_actions.append(action);
}

int StrategyManager::getActionCount() const
{
	return _actions.count();
}

GameState &StrategyManager::getCurrentState()
{
	_currentState._robotposition = _grid->getNearestNode(_map->getRobot()->getPosition());
	_currentState._opponentPosition = _map->getOpponent()->getPosition();
	_currentState._remainingTime = 90 - getMatchTime();
	return _currentState;
}

void StrategyManager::updateFutureMap(const GameState &state)
{
	_futureMap->moveRobot(state._robotposition->getPosition());
	_futureMap->moveOpponent(state._opponentPosition);
}

double StrategyManager::getFuturePathingDistance(const GameState &state, Tools::NGridNode* from, Tools::NGridNode* to)
{
	updateFutureMap(state);
	_futurePather->prepareReplan(from, to);

	Tools::Trajectory t;
	_futurePather->replanTrajectory(t);

	return t.manhattanDistance();
}

void StrategyManager::setPause(bool value)
{
	if (_isPaused != value)
	{
		if (value)
		{
			_matchTimer.stop();
			_recordTimer.stop();
		}
		else
		{
			_matchTimer.start();

			if (_record)
				_recordTimer.start();
		}

		_isPaused = value;
	}
}

void StrategyManager::execute(AbstractAction *action)
{
	if (!action)
		return;

	connect(action, SIGNAL(finished(bool)), this, SLOT(actionFinished(bool)));
	action->execute();
}

void StrategyManager::actionFinished(bool result)
{
	AbstractAction* action = _actions.value(_currentActionIndex);
	Q_ASSERT(action);
	if (!action)
	{
		logger() << "Hmmm, an unknown action has finished..." << Tools::endl;
		return;
	}

	disconnect(action, SIGNAL(finished(bool)), this, SLOT(actionFinished(bool)));
	action->end();

	++_currentActionIndex;

	if (_strategy)
		_strategy->actionDone(action, result, isLastAction());

	if (isLastAction() && _loop)
	{
		logger() << "Looping back at the begining..." << Tools::endl;
		_currentActionIndex = 0;
	}

	if (isLastAction())
	{
		logger() << "No more actions to do." << Tools::endl;
		emit strategyFinished();

		if (_autoQuit)
			quit();
	}

	next();
}

void StrategyManager::insertAction(int index, AbstractAction *action)
{
	_actions.insert(index, action);
}

void StrategyManager::insertActionHere(AbstractAction *action)
{
	insertAction(_currentActionIndex, action);
}

void StrategyManager::cancelCurrentAction()
{
	AbstractAction* currentAction = _actions.value(_currentActionIndex);
	if (currentAction)
		currentAction->stop();
}

void StrategyManager::next()
{
	if (isLastAction() || _currentActionIndex < 0)
		return;

	AbstractAction* a = _actions.value(_currentActionIndex, 0);

	execute(a);
}

bool StrategyManager::isLastAction() const
{
	return _currentActionIndex >= _actions.count();
}

bool StrategyManager::isFirstAction() const
{
	return _currentActionIndex== 0;
}

bool StrategyManager::isEmpty() const
{
	return _actions.isEmpty();
}

bool StrategyManager::movementInProgress() const
{
	return _trajectoryFinder->hasObjective();
}

void StrategyManager::setReversedStrategy(bool value)
{
	_reversed = value;
}

StrategyMap * StrategyManager::getMap() const
{
	return _map;
}

const Sharp *StrategyManager::getAvoidingSharp(int index) const
{
    return _sharps.value(index, nullptr);
}

const Sensor *StrategyManager::getOtherSharp(int index) const
{
    return _scannerSharps.value(index, nullptr);
}

const Sensor *StrategyManager::getColorSensor(int index) const
{
    return _colorSensors.value(index, nullptr);
}

const Sensor* StrategyManager::getMicroswitch(int index) const
{
	return _microswitchs.value(index, nullptr);
}

const QMap<int, Sharp *> &StrategyManager::getAvoidingSharps() const
{
	return _sharps;
}

const QMap<int, Sensor *> &StrategyManager::getOtherSharps() const
{
    return _scannerSharps;
}

const QMap<int, Sensor *> &StrategyManager::getColorSensors() const
{
    return _colorSensors;
}

const QMap<int, Sensor *> &StrategyManager::getMicroswitchs() const
{
	return _microswitchs;
}

const Sensor *StrategyManager::getSensor(int index, Comm::SensorType type) const
{
    switch(type)
    {
		case Comm::SharpSensor:   		return getOtherSharp(index);
		case Comm::MicroswitchSensor:   return getMicroswitch(index);
        case Comm::ColorSensor:     	return getColorSensor(index);
    }

	return nullptr;
}

double StrategyManager::getParameter(int index) const
{
	return _parameters.value(index, 0);
}

QString StrategyManager::getParameterName(int index) const
{
	return _parameterNames.value(index, "Unknown parameter");
}

void StrategyManager::setParameter(int index, double value)
{
	if (index >= 0 && index < _parameters.count())
	{
		_parameters[index] = value;
		_robot->setParameters(_parameters);
	}
}

ActionFactory * StrategyManager::getActionFactory() const
{
	return _actionFactory;
}

NGrid *StrategyManager::getGrid() const
{
	return _grid;
}

bool StrategyManager::isMirrored() const
{
	return _reversed;
}

void StrategyManager::startRecording(const QString& filename)
{
	_record = true;

	if (_recordFile)
	{
		_recordFile->close();
		_recordFile->setFileName(filename);
	}
	else
		_recordFile = new QFile(filename);

	_recordFile->open(QIODevice::WriteOnly);

	if (_recorder)
		_recorder->setDevice(_recordFile);
	else
		_recorder = new QDataStream(_recordFile);

	_recordTimer.start();
}

void StrategyManager::stopRecording()
{
	if (_record == true)
	{
		_recordFile->close();
		_recorder->setDevice(0);
		_record = false;
		_recordTimer.stop();
	}
}

bool StrategyManager::isRecording()
{
	return _record;
}

void StrategyManager::record()
{
	if (!_initDone)
		return;

	QList<double> sharps;
	QList<DataObject *> objects = _map->getObjects(StrategyMap::SharpGroup);
	CircleObject* object;

	sharps.clear();
	for(QMap<int, Sharp*>::const_iterator it = _sharps.constBegin(); it != _sharps.constEnd(); ++it)
		sharps << (*it)->getDistance();

	*_recorder << _map->getRobotPosition().getX()
			   << _map->getRobotPosition().getY()
			   << _map->getRobotPosition().getTheta()
			   << sharps
			   << objects.count();

	while (!objects.empty())
	{
		object = static_cast<CircleObject*>(objects.takeFirst());
		*_recorder << object->getRadius()
				   << object->getPosition().x()
				   << object->getPosition().y()
				   << object->getRotation();
	}
}

void StrategyManager::incrementMatchTime()
{
	_matchTime++;
}

int StrategyManager::getMatchTime()
{
	return _matchTime;
}
