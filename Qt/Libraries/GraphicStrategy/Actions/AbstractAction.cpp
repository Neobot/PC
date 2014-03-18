#include "AbstractAction.h"
#include "StrategyManager.h"
#include "StrategyMap.h"
#include "ToolsLib.h"

AbstractAction::AbstractAction(QObject* parent) : QObject(parent)
{
}

void AbstractAction::succeed()
{
    emit finished(true);
}

void AbstractAction::failed()
{
    emit finished(false);
}

//--------------------------------------------------------------------------------


OrientationSwitchCaseAction::OrientationSwitchCaseAction(StrategyManager *manager, QObject *parent)
	: AbstractAction(parent), _manager(manager), _currentAction(0)
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

void OrientationSwitchCaseAction::execute()
{
	double robotOrientation = _manager->getMap()->getRobotPosition().getTheta();
	robotOrientation = Tools::angleInMinusPiPlusPi(robotOrientation);

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
		}
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
