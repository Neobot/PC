#ifndef ABSTRACTACTION_H
#define ABSTRACTACTION_H

#include <QObject>

class StrategyManager;

class AbstractAction : public QObject
{
    Q_OBJECT
public:
    AbstractAction(QObject* parent = 0);

    virtual void execute() = 0;
    virtual void end() {}
    virtual QString getActionName() const = 0;
    virtual void stop() {}

signals:
    void finished(bool result);

protected slots:
    void succeed();
    void failed();
};

class ActionGroup : public AbstractAction
{
	Q_OBJECT
public:
	ActionGroup(bool asynchrone = false, QObject* parent = 0);
	ActionGroup(const QList<AbstractAction*>& subActions, bool asynchrone = false, QObject* parent = 0);
	~ActionGroup();

	void addSubAction(AbstractAction* action);
	void execute();
	void stop();
	QString getActionName() const;

private:
	QList<AbstractAction*> _list;
	QList<AbstractAction*> _finishedList;
	bool _result;
	bool _asynchrone;

	void executeNextSubAction();

private slots:
	void nextAction(bool lastActionResult = true);
};

/**
 * @brief Action which trigger a sub-action if the orientation meet a range condition.
 */
class OrientationSwitchCaseAction : public AbstractAction
{
	Q_OBJECT
public:
	OrientationSwitchCaseAction(StrategyManager* manager, QObject* parent = 0);
	~OrientationSwitchCaseAction();

	/**
	 * @brief addCase Adds a switch case action.
	 * @param min Minimum angle in radian between -pi and pi
	 * @param max Maximum angle in radian between -pi and pi
	 * @param associatedAction Action which should be triggered if the robot orientation is between \p min and \p max.
	 * @note If two cases have a common range, the one first which have been added first is triggered.
	 */
	void addCase(double min, double max, AbstractAction* associatedAction);

	void execute();
	void stop();
	void end();
	QString getActionName() const;

private:
	struct Case
	{
		double min;
		double max;
		AbstractAction* action;
	};

	QList<Case> _cases;
	StrategyManager* _manager;
	AbstractAction* _currentAction;

};

#endif // ABSTRACTACTION_H
