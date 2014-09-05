#include "ContainerActions.h"

ActionGroup::ActionGroup(QObject *parent)
	: ActionGroup(QList<AbstractAction*>(), parent)
{
}

ActionGroup::ActionGroup(const QList<AbstractAction*>& subActions, QObject *parent)
	: AbstractAction(parent), _list(subActions), _result(true)
{
}

ActionGroup::~ActionGroup()
{
	qDeleteAll(_list);
	qDeleteAll(_finishedList);
	_finishedList.clear();
}

void ActionGroup::execute()
{
	if (!_list.isEmpty())
		executeNextSubAction();
	else
		succeed();
}

void ActionGroup::stop()
{
	if (!_list.isEmpty())
	{
		_result = false;
		AbstractAction* currentAction = _list.takeFirst();

		//transfer the remaining actions in the finished list
		_finishedList.append(_list);
		_list.clear();

		//Keep the current action to be able to retrieve it later
		_list.append(currentAction);

		currentAction->stop();
	}
}

void ActionGroup::end()
{
	while(!_finishedList.isEmpty())
	{
		_list.insert(0, _finishedList.takeLast());
	}
	_result = true;
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
	else
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
	return QString("Asynchrone action group with ").append(QString::number(_list.count() + _finishedList.count())).append(" actions.");
}

//--------------------------------------------------------------------------------

AsynchroneActionGroup::AsynchroneActionGroup(StopCondition stopCondition, QObject *parent)
	: AsynchroneActionGroup(QList<AbstractAction*>(), stopCondition, parent)
{
}

AsynchroneActionGroup::AsynchroneActionGroup(const QList<AbstractAction*>& subActions, StopCondition stopCondition, QObject *parent)
	: AbstractAction(parent), _list(subActions), _firstAction(nullptr), _lastAction(nullptr), _globalResult(true), _stopCondition(stopCondition), _isCanceled(false)
{
}

AsynchroneActionGroup::~AsynchroneActionGroup()
{
	qDeleteAll(_list);
	qDeleteAll(_finishedList);
}

void AsynchroneActionGroup::execute()
{
	if (!_list.isEmpty())
	{
		_firstAction = _list.first();
		_lastAction = _list.last();
		
		foreach(AbstractAction* action, _list)
		{
			connect(action, SIGNAL(finished(bool)), this, SLOT(subActionFinished(bool)));
			action->execute();
		}
	}
	else
		succeed();
}

void AsynchroneActionGroup::stop()
{
	_stopCondition = AllActionFinished; //We need to wait for all action to stop.

	stopRemainingActions();
}

void AsynchroneActionGroup::end()
{
	while(!_finishedList.isEmpty())
	{
		_list.insert(0, _finishedList.takeLast());
	}

	if (_firstAction)
	{
		//ensure _firstAction is first
		_list.removeOne(_firstAction);
		_list.insert(0, _firstAction);
	}

	if (_lastAction)
	{
		//ensure _lastAction is last
		_list.removeOne(_lastAction);
		_list.append(_lastAction);
	}

	_firstAction = nullptr;
	_lastAction = nullptr;
	_globalResult = true;
	_isCanceled = false;
}

void AsynchroneActionGroup::subActionFinished(bool lastActionResult)
{
	AbstractAction* action = qobject_cast<AbstractAction*>(sender());
	if (action)
	{
		action->end();

		_finishedList << action;
		_list.removeOne(action);

		bool stop = false;
		bool result = true;
		switch (_stopCondition)
		{
			case OneActionFinished:
				stop = true;
				result = lastActionResult;
				break;
			case FirstActionFinished:
				stop = action == _firstAction;
				result = lastActionResult;
				break;
			case LastActionFinished:
				stop = action == _lastAction;
				result = lastActionResult;
				break;
			case AllActionFinished:
				stop = _list.isEmpty();
				_globalResult = _globalResult && lastActionResult;
				result = _globalResult;
			default:
				break;
		}

		if (stop)
		{
			stopRemainingActions();
			emit finished(result);
		}
	}
}

QString AsynchroneActionGroup::getActionName() const
{
	return QString("Asynchrone action group with ").append(QString::number(_list.count())).append(" actions.");
}

void AsynchroneActionGroup::stopRemainingActions()
{
	foreach(AbstractAction* action, _list)
	{
		action->stop();
	}
}
