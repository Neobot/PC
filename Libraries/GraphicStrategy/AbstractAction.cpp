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

ActionGroup::ActionGroup(bool asynchrone, QObject *parent)
	: AbstractAction(parent), _result(true), _asynchrone(asynchrone)
{
}

ActionGroup::ActionGroup(const QList<AbstractAction*>& subActions, bool asynchrone, QObject *parent)
	: AbstractAction(parent), _list(subActions), _result(true), _asynchrone(asynchrone)
{
}

ActionGroup::~ActionGroup()
{
	qDeleteAll(_list);
	qDeleteAll(_finishedList);
}

void ActionGroup::execute()
{
	if (!_list.isEmpty())
		executeNextSubAction();
		
	if (_asynchrone)
		succeed();
}

void ActionGroup::stop()
{
	if (_list.isEmpty())
		return;

	AbstractAction* currentAction = _list.first();
	disconnect(currentAction, SIGNAL(finished(bool)), this, SLOT(nextAction(bool)));
	currentAction->stop();

	if (!_asynchrone)
		failed();
}

void ActionGroup::nextAction(bool lastActionResult)
{
	AbstractAction* prevAction = _list.takeFirst();
	prevAction->end();
	disconnect(prevAction, SIGNAL(finished(bool)), this, SLOT(nextAction(bool)));
	_finishedList << prevAction;

	_result = _result && lastActionResult;

	if (!_list.isEmpty())
		executeNextSubAction();
	else if (!_asynchrone)
		emit finished(_result);
}

void ActionGroup::executeNextSubAction()
{
	AbstractAction* nextAct = _list.first();
	connect(nextAct, SIGNAL(finished(bool)), this, SLOT(nextAction(bool)));
    nextAct->execute();
}

QString ActionGroup::getActionName() const
{
	return QString("Action group");
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
