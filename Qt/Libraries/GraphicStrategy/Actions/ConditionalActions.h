#ifndef CONDITIONALACTIONS_H
#define CONDITIONALACTIONS_H

#include "AbstractAction.h"
#include <QRectF>

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

	/**
	 * @brief Sets the action which should be executed if no case has been triggered.
	 */
	void setDefaultAction(AbstractAction* defaultAction);

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
	AbstractAction* _defaultAction;
};

class PositionSwitchCaseAction : public AbstractAction
{
	Q_OBJECT
public:
	PositionSwitchCaseAction(StrategyManager* manager, QObject* parent = 0);
	~PositionSwitchCaseAction();

	/**
	 * @brief addCase Adds a switch case action.
	 * @param zone Position zone in mm.
	 * @param associatedAction Action which should be triggered if the robot orientation is between \p min and \p max.
	 * @note If two cases have a common zone, the one first which have been added first is triggered.
	 */
	void addCase(const QRectF& zone, AbstractAction* associatedAction);

	/**
	 * @brief Sets the action which should be executed if no case has been triggered.
	 */
	void setDefaultAction(AbstractAction* defaultAction);

	void execute();
	void stop();
	void end();
	QString getActionName() const;

private:
	struct Case
	{
		QRectF rect;
		AbstractAction* action;
	};

	QList<Case> _cases;
	StrategyManager* _manager;
	AbstractAction* _currentAction;
	AbstractAction* _defaultAction;
};

#endif // CONDITIONALACTIONS_H
