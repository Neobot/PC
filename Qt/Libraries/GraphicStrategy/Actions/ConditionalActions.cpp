#include "ConditionalActions.h"
#include "StrategyManager.h"
#include "StrategyMap.h"
#include "ToolsLib.h"

SingleValueSwitchCaseAction::SingleValueSwitchCaseAction(QObject *parent)
	: AbstractAction(parent), _currentAction(nullptr), _defaultAction(nullptr)
{
}

SingleValueSwitchCaseAction::~SingleValueSwitchCaseAction()
{
	foreach(const Case& c, _cases)
	{
		delete c.action;
	}
}

void SingleValueSwitchCaseAction::addCase(double min, double max, AbstractAction *associatedAction)
{
	Case c;
	c.min = min;
	c.max = max;
	c.action = associatedAction;
	_cases << c;

	associatedAction->setParent(this);
}

void SingleValueSwitchCaseAction::addCase(double value, AbstractAction *associatedAction)
{
	addCase(value, value, associatedAction);
}

void SingleValueSwitchCaseAction::setDefaultAction(AbstractAction *defaultAction)
{
	_defaultAction = defaultAction;
}

void SingleValueSwitchCaseAction::execute()
{
	double value = getSwitchValue();

	_currentAction = nullptr;
	foreach(const Case& c, _cases)
	{
		if (c.min <= value && value <= c.max)
		{
			_currentAction = c.action;
			if (_currentAction)
			{
				connect(_currentAction, SIGNAL(finished(bool)), this, SIGNAL(finished(bool)));
				_currentAction->execute();
			}
			else
			{
				succeed();
			}

			break;
		}
	}

	if (!_currentAction && _defaultAction)
	{
		_currentAction = _defaultAction;
		connect(_currentAction, SIGNAL(finished(bool)), this, SIGNAL(finished(bool)));
		_currentAction->execute();
	}
	else
	{
		failed();
	}
}

void SingleValueSwitchCaseAction::stop()
{
	if (_currentAction)
		_currentAction->stop();
}

void SingleValueSwitchCaseAction::end()
{
	if (_currentAction)
	{
		_currentAction->end();
		disconnect(_currentAction, SIGNAL(finished(bool)), this, SIGNAL(finished(bool)));
	}
}

//----------------------------------------------------------------------------------------------------------------------------------

OrientationSwitchCaseAction::OrientationSwitchCaseAction(StrategyManager *manager, QObject *parent)
	: SingleValueSwitchCaseAction(parent), _manager(manager)
{
}

OrientationSwitchCaseAction::~OrientationSwitchCaseAction()
{
}

QString OrientationSwitchCaseAction::getActionName() const
{
	return QString("Orientation switch case action");
}

double OrientationSwitchCaseAction::getSwitchValue() const
{
	double robotOrientation = _manager->getMap()->getRobotPosition().getTheta();
	robotOrientation = Tools::angleInMinusPiPlusPi(robotOrientation);

	return robotOrientation;
}

//----------------------------------------------------------------------------------------------------------------------------------

SensorSwitchCaseAction::SensorSwitchCaseAction(int sensorId, SensorType type, StrategyManager* manager, QObject *parent)
	: SingleValueSwitchCaseAction(parent), _sensorId(sensorId), _type(type), _manager(manager)
{

}

SensorSwitchCaseAction::~SensorSwitchCaseAction()
{
}

QString SensorSwitchCaseAction::getActionName() const
{
	return QString("Sensor switch case action");
}

double SensorSwitchCaseAction::getSwitchValue() const
{
	Sensor* sensor = _manager->getSensor(_sensorId, _type);
	if (sensor)
		return sensor->getState();
		
	return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------

PositionSwitchCaseAction::PositionSwitchCaseAction(StrategyManager *manager, QObject *parent)
	: AbstractAction(parent), _manager(manager), _currentAction(nullptr), _defaultAction(nullptr)
{
}

PositionSwitchCaseAction::~PositionSwitchCaseAction()
{
	foreach(const Case& c, _cases)
	{
		delete c.action;
	}
}

void PositionSwitchCaseAction::addCase(const QRectF& zone, AbstractAction *associatedAction)
{
	Case c;
	c.rect = zone;
	c.action = associatedAction;
	_cases << c;

	associatedAction->setParent(this);
}

void PositionSwitchCaseAction::setDefaultAction(AbstractAction *defaultAction)
{
	_defaultAction = defaultAction;
}

void PositionSwitchCaseAction::execute()
{
	QPointF robotPos = _manager->getMap()->getRobotPosition().toQPointF();

	_currentAction = nullptr;
	foreach(const Case& c, _cases)
	{
		if (c.rect.contains(robotPos))
		{
			_currentAction = c.action;
			if (_currentAction)
			{
				connect(_currentAction, SIGNAL(finished(bool)), this, SIGNAL(finished(bool)));
				_currentAction->execute();
			}
			else
			{
				succeed();
			}

			break;
		}
	}

	if (!_currentAction && _defaultAction)
	{
		_currentAction = _defaultAction;
		connect(_currentAction, SIGNAL(finished(bool)), this, SIGNAL(finished(bool)));
		_currentAction->execute();
	}
	else
	{
		failed();
	}
}

void PositionSwitchCaseAction::stop()
{
	if (_currentAction)
		_currentAction->stop();
}

void PositionSwitchCaseAction::end()
{
	if (_currentAction)
	{
		_currentAction->end();
		disconnect(_currentAction, SIGNAL(finished(bool)), this, SIGNAL(finished(bool)));
	}
}

QString PositionSwitchCaseAction::getActionName() const
{
	return QString("Position switch case action");
}
