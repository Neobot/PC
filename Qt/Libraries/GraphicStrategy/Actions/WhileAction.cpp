#include "WhileAction.h"

PositionWhileAction::PositionWhileAction(StrategyManager* manager, const QRectF& rect, AbstractAction* loopedAction, bool neg, QObject* parent)
	: AbstractWhile<QPointF, QRectF>(new PositionTest(manager), neg, rect, loopedAction, parent)
{
	setTestType(ConditionTest<QPointF, QRectF>::Inside);
}

QString PositionWhileAction::getActionName() const
{
	return QString("While position");
}

//----------------------------------------------------------------------------------------------------------------------------------

OpponentWhileAction::OpponentWhileAction(StrategyManager* manager, const QRectF& rect, AbstractAction* loopedAction, bool neg, QObject* parent)
	: AbstractWhile<QPointF, QRectF>(new OpponentTest(manager), neg, rect, loopedAction, parent)
{
	setTestType(ConditionTest<QPointF, QRectF>::Inside);
}

QString OpponentWhileAction::getActionName() const
{
	return QString("While opponent");
}

//----------------------------------------------------------------------------------------------------------------------------------

OrientationWhileAction::OrientationWhileAction(StrategyManager *manager, double angleMin, double angleMax, AbstractAction* loopedAction, bool neg, bool degrees, QObject *parent)
	: AbstractWhile<double, QPair<double, double>>(new OrientationTest(degrees, manager), neg, qMakePair(angleMin, angleMax), loopedAction, parent)
{
	setTestType(ConditionTest<double, QPair<double, double>>::Inside);
}

QString OrientationWhileAction::getActionName() const
{
	return QString("While orientation");
}

//----------------------------------------------------------------------------------------------------------------------------------

SensorWhileAction::SensorWhileAction(StrategyManager *manager, int sensorId, Comm::SensorType type, int value, AbstractAction *loopedAction, bool neg, QObject *parent)
	: AbstractWhile<int, QPair<int, int>>(new SensorTest(sensorId, type, manager), neg, qMakePair(value, value), loopedAction, parent)
{
}

QString SensorWhileAction::getActionName() const
{
	return QString("While sensor");
}

//----------------------------------------------------------------------------------------------------------------------------------

StrategyReversedWhileAction::StrategyReversedWhileAction(StrategyManager *manager, AbstractAction *loopedAction, bool neg, QObject *parent)
	: AbstractWhile<bool, bool>(new StrategyReversedTest(manager), neg, true, loopedAction, parent)
{
}

QString StrategyReversedWhileAction::getActionName() const
{
	return QString("While strategy reversed");
}
