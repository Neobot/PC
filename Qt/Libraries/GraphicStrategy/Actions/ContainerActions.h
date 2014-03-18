#ifndef CONTAINERACTIONS_H
#define CONTAINERACTIONS_H

#include "AbstractAction.h"

/**
 * @brief A group of synchrone actions.
 * Subactions are launched order they have been added. The group takes their ownership and is in charge of their destruction.
 */
class ActionGroup : public AbstractAction
{
	Q_OBJECT
public:
	ActionGroup(QObject* parent = 0);
	ActionGroup(const QList<AbstractAction*>& subActions, QObject* parent = 0);
	~ActionGroup();

	void addSubAction(AbstractAction* action);

	void execute();
	void stop();

	QString getActionName() const;

private:
	QList<AbstractAction*> _list;
	QList<AbstractAction*> _finishedList;
	bool _result;

	void executeNextSubAction();

private slots:
	void nextAction(bool lastActionResult = true);
};

/**
 * @brief A group of asynchrone actions.
 * All subactions are launched at the same time. The group takes their ownership and is in charge of their destruction.
 * Sub-actions should support cancelation to work properly with all stop conditions.
 */
class AsynchroneActionGroup : public AbstractAction
{
	Q_OBJECT
public:
	enum StopCondition
	{
		OneActionFinished,
		AllActionFinished,
		FirstActionFinished,
		LastActionFinished
	};

	AsynchroneActionGroup(StopCondition stopCondition, QObject* parent = 0);
	AsynchroneActionGroup(const QList<AbstractAction*>& subActions, StopCondition stopCondition, QObject* parent = 0);
	~AsynchroneActionGroup();

	void addSubAction(AbstractAction* action);

	void execute();
	void stop();

	QString getActionName() const;

private:
	QList<AbstractAction*> _list;
	QList<AbstractAction*> _finishedList;
	AbstractAction* _firstAction;
	AbstractAction* _lastAction;
	bool _globalResult;
	StopCondition _stopCondition;
	bool _isCanceled;

	void stopRemainingActions();

private slots:
	void subActionFinished(bool lastActionResult);
};

#endif // CONTAINERACTIONS_H
