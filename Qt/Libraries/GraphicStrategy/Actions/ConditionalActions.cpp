#include "ConditionalActions.h"
#include "StrategyManager.h"
#include "StrategyMap.h"
#include "ToolsLib.h"


bool PointTest::isTrue() const
{
	bool ok = true;
	if (_type == Inside)
	{
		ok = _right.contains(_left);
	}
	else //_type == Equals
	{
		ok = _left == _right.topLeft();
	}

	return _neg ? !ok : ok;
}

bool DoubleTest::isTrue() const
{
	bool ok = true;
	if (_type == Inside)
	{
		ok = _right.first <= _left  &&  _left <= _right.second;
	}
	else //_type == Equals
	{
		ok = _left == _right.first;
	}

	return _neg ? !ok : ok;
}

bool IntTest::isTrue() const
{
	bool ok = true;
	if (_type == Inside)
	{
		ok = _right.first <= _left  &&  _left <= _right.second;
	}
	else //_type == Equals
	{
		ok = _left == _right.first;
	}

	return _neg ? !ok : ok;
}


bool SingleBoolTest::isTrue() const
{
	bool ok = _left;
	return _neg ? !ok : ok;
}


bool AlwaysTrueTest::isTrue() const
{
	return true;
}

//----------------------------------------------------------------------------------------------------------------------------------


PositionTest::PositionTest(StrategyManager *manager) : _manager(manager){}

void PositionTest::updateTest()
{
	setLeftOperand(_manager->getMap()->getRobotPosition().toQPointF());
}


OpponentTest::OpponentTest(StrategyManager *manager) : _manager(manager) {}

void OpponentTest::updateTest()
{
	setLeftOperand(_manager->getMap()->getOpponentPosition().toQPointF());
}


OrientationTest::OrientationTest(bool degrees, StrategyManager *manager) : _manager(manager), _degrees(degrees) {}

void OrientationTest::updateTest()
{
	if (_degrees)
		setLeftOperand(_manager->getMap()->getRobotPosition().getThetaDegree());
	else
		setLeftOperand(_manager->getMap()->getRobotPosition().getTheta());
}

SensorTest::SensorTest(int sensorId, Comm::SensorType type, StrategyManager *manager) : _manager(manager), _sensorId(sensorId), _type(type) {}

void SensorTest::updateTest()
{
	const Sensor* sensor = _manager->getSensor(_sensorId, _type);
	if (sensor)
		setLeftOperand(sensor->getState());
	else
		setLeftOperand(0);
}

StrategyReversedTest::StrategyReversedTest(StrategyManager *manager) : _manager(manager){}

void StrategyReversedTest::updateTest()
{
	setLeftOperand(_manager->isMirrored());
}


//----------------------------------------------------------------------------------------------------------------------------------

PositionSwitchCaseAction::PositionSwitchCaseAction(StrategyManager* manager, QObject* parent)
	: AbstractSwitch<QPointF, QRectF>(new PositionTest(manager), parent)
{
	setTestType(ConditionTest<QPointF, QRectF>::Inside);
}

QString PositionSwitchCaseAction::getActionName() const
{
	return QString("Position switch case action");
}

//----------------------------------------------------------------------------------------------------------------------------------

OpponentSwitchCaseAction::OpponentSwitchCaseAction(StrategyManager* manager, QObject* parent)
	: AbstractSwitch<QPointF, QRectF>(new OpponentTest(manager), parent)
{
	setTestType(ConditionTest<QPointF, QRectF>::Inside);
}

QString OpponentSwitchCaseAction::getActionName() const
{
	return QString("Opponent switch case action");
}

//----------------------------------------------------------------------------------------------------------------------------------

OrientationSwitchCaseAction::OrientationSwitchCaseAction(StrategyManager* manager, bool degrees, QObject* parent)
	: AbstractSwitch<double, QPair<double, double>>(new OrientationTest(degrees, manager), parent)
{
	setTestType(ConditionTest<double, QPair<double, double>>::Inside);
}

QString OrientationSwitchCaseAction::getActionName() const
{
	return QString("Orientation switch case action");
}

//----------------------------------------------------------------------------------------------------------------------------------

SensorSwitchCaseAction::SensorSwitchCaseAction(int sensorId, Comm::SensorType type, StrategyManager* manager, QObject* parent)
	: AbstractSwitch<int, QPair<int, int>>(new SensorTest(sensorId, type, manager), parent)
{
}

void SensorSwitchCaseAction::addCase(int value, AbstractAction *associatedAction)
{
	QPair<int, int> pair;
	pair.first = value;
	pair.second = value;

	AbstractSwitch::addCase(pair, associatedAction);
}

QString SensorSwitchCaseAction::getActionName() const
{
	return QString("Sensor switch case action");
}

//----------------------------------------------------------------------------------------------------------------------------------

StrategyReversedSwitchCaseAction::StrategyReversedSwitchCaseAction(StrategyManager *manager, QObject *parent)
	: AbstractSwitch<bool, bool>(new StrategyReversedTest(manager), parent)
{
}

QString StrategyReversedSwitchCaseAction::getActionName() const
{
	return QString("Sensor switch case action");
}




