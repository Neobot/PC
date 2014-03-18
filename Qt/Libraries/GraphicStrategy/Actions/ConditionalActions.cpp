#include "ConditionalActions.h"
#include "StrategyManager.h"
#include "StrategyMap.h"
#include "ToolsLib.h"

OrientationSwitchCaseAction::OrientationSwitchCaseAction(StrategyManager *manager, QObject *parent)
	: AbstractAction(parent), _manager(manager), _currentAction(nullptr), _defaultAction(nullptr)
{
}

OrientationSwitchCaseAction::~OrientationSwitchCaseAction()
{
}

void OrientationSwitchCaseAction::addCase(double min, double max, AbstractAction *associatedAction)
{
	Case c;
	c.min = min;
	c.max = max;
	c.action = associatedAction;
	_cases << c;

	associatedAction->setParent(this);
}

void OrientationSwitchCaseAction::setDefaultAction(AbstractAction *defaultAction)
{
	_defaultAction = defaultAction;
}

void OrientationSwitchCaseAction::execute()
{
	double robotOrientation = _manager->getMap()->getRobotPosition().getTheta();
	robotOrientation = Tools::angleInMinusPiPlusPi(robotOrientation);

	_currentAction = nullptr;
	foreach(const Case& c, _cases)
	{
		if (c.min <= robotOrientation && robotOrientation <= c.max)
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

void OrientationSwitchCaseAction::stop()
{
	if (_currentAction)
		_currentAction->stop();
}

void OrientationSwitchCaseAction::end()
{
	if (_currentAction)
	{
		_currentAction->end();
		disconnect(_currentAction, SIGNAL(finished(bool)), this, SIGNAL(finished(bool)));
	}
}

QString OrientationSwitchCaseAction::getActionName() const
{
	return QString("Orientation switch case action");
}

//--------------------------

PositionSwitchCaseAction::PositionSwitchCaseAction(StrategyManager *manager, QObject *parent)
	: AbstractAction(parent), _manager(manager), _currentAction(nullptr), _defaultAction(nullptr)
{
}

PositionSwitchCaseAction::~PositionSwitchCaseAction()
{
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
