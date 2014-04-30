#ifndef CONDITIONALACTIONS_H
#define CONDITIONALACTIONS_H

#include "AbstractAction.h"
#include "Instructions.h"

#include <QRectF>

class Sensor;

/**
 * @brief Abstract action which trigger a sub-action if the a value meet a range condition.
 * The switch case action takes the ownership of the sub-actions and is in charge of their destruction.
 */
class SingleValueSwitchCaseAction : public AbstractAction
{
	Q_OBJECT
public:
	SingleValueSwitchCaseAction(QObject* parent = 0);
	virtual ~SingleValueSwitchCaseAction();

	/**
	 * @brief addCase Adds a switch case action.
	 * @param min Minimum value
	 * @param max Maximum value
	 * @param associatedAction Action which should be triggered if the value is between \p min and \p max.
	 * @note If two cases have a common range, the one first which have been added first is triggered.
	 */
	void addCase(double min, double max, AbstractAction* associatedAction);

	/**
	 * @brief addCase Adds a switch case action.
	 * @param value A value
	 * @param associatedAction Action which should be triggered if the value is equals to \p value.
	 */
	void addCase(double value, AbstractAction* associatedAction);

	/**
	 * @brief Sets the action which should be executed if no case has been triggered.
	 */
	void setDefaultAction(AbstractAction* defaultAction);

	void execute();
	void stop();
	void end();


protected:
	virtual double getSwitchValue() const = 0;

private:
	struct Case
	{
		double min;
		double max;
		AbstractAction* action;
	};

	QList<Case> _cases;
	AbstractAction* _currentAction;
	AbstractAction* _defaultAction;
};

/**
 * @brief Switch between actions depending on robot orientation.
 * @note Orientation is between -pi and pi
 */
class OrientationSwitchCaseAction : public SingleValueSwitchCaseAction
{
public:
	OrientationSwitchCaseAction(StrategyManager* manager, QObject* parent = 0);
	~OrientationSwitchCaseAction();

	QString getActionName() const;

protected:
	virtual double getSwitchValue() const;

private:
	StrategyManager* _manager;
};

/**
 * @brief Switch between actions depending on the value of a sensor
 * @note Orientation is between -pi and pi
 */
class SensorSwitchCaseAction : public SingleValueSwitchCaseAction
{
public:
	SensorSwitchCaseAction(int sensorId, Comm::SensorType type, StrategyManager* manager, QObject* parent = 0);
	~SensorSwitchCaseAction();

	QString getActionName() const;

protected:
	virtual double getSwitchValue() const;

private:
	int _sensorId;
	Comm::SensorType _type;
	StrategyManager* _manager;
};

/**
 * @brief Action which trigger a sub-action if the position of the robot is in an specific area.
 * The switch case action takes the ownership of the sub-actions and is in charge of their destruction.
 */
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
