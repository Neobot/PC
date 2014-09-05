#ifndef CONDITIONALACTIONS_H
#define CONDITIONALACTIONS_H

#include "AbstractAction.h"
#include "Instructions.h"

#include <QRectF>
#include <QPair>

class Sensor;


template <class TL, class TR>
class ConditionTest
{
public:
	enum ConditionType
	{
		Inside,
		Equals
	};

	ConditionTest() {_neg = false; _type=Equals;}
	ConditionTest(TL left, TR right) : ConditionTest(), _left(left), _right(right) {}

	virtual bool isTrue() const = 0;

	void setLeftOperand(TL left) {_left = left;}
	void setRightOperand(TR right) {_right = right;}

	void setType(int type) {_type = type;}

	virtual void updateTest() = 0;

	protected:
		TL _left;
		TR _right;
		bool _neg;
		int _type;
};

//----------------------------------------------------------------------------------------------------------------------------------

template <class TL, class TR>
class AbstractSwitch : public AbstractAction
{
	public:
		AbstractSwitch(ConditionTest<TL, TR>* test, QObject* parent = 0)
			: AbstractAction(parent), _test(test), _currentAction(nullptr), _defaultAction(nullptr) {}

		virtual ~AbstractSwitch()
		{
			deleteCases();
			delete _defaultAction;
		}

		void addCase(TR c, AbstractAction* associatedAction) {_cases << Case<TR>(c, associatedAction);}

		void execute()
		{
			if (!_test)
			{
				failed();
				return;
			}

			_test->updateTest();

			for(const Case<TR>& c: _cases)
			{
				_test->setRightOperand(c.value);
				if (_test->isTrue())
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

			if (!_currentAction)
			{
				failed();
			}
		}

		void stop()
		{
			if (_currentAction)
				_currentAction->stop();
		}

		void end()
		{
			if (_currentAction)
			{
				_currentAction->end();
				disconnect(_currentAction, SIGNAL(finished(bool)), this, SIGNAL(finished(bool)));
			}
		}

		void setDefaultAction(AbstractAction *defaultAction)
		{
			_defaultAction = defaultAction;
		}

		void setTestType(int type)
		{
			if (_test)
				_test->setType(type);
		}


	protected:
		void setTest(ConditionTest<TL, TR>* test) {_test = test;}

		TL _switchValue;

	private:
		template <class T>
		struct Case
		{
			Case(T v, AbstractAction* a) : value(v), action(a) {}

			T value;
			AbstractAction* action;
		};

		ConditionTest<TL, TR>* _test;
		QList<Case<TR>> _cases;


		AbstractAction* _currentAction;
		AbstractAction* _defaultAction;

		void deleteCases()
		{
			for(const Case<TR>& c : _cases)
			{
				delete c.action;
			}
		}
};

//----------------------------------------------------------------------------------------------------------------------------------

class PointTest : public ConditionTest<QPointF, QRectF>
{
public:
	bool isTrue() const;
};

class DoubleTest : public ConditionTest<double, QPair<double, double>>
{
public:
	bool isTrue() const;
};

class IntTest : public ConditionTest<int, QPair<int, int>>
{
public:
	bool isTrue() const;
};


class SingleBoolTest : public ConditionTest<bool, bool>
{
public:
	bool isTrue() const;
};

//----------------------------------------------------------------------------------------------------------------------------------

class PositionTest : public PointTest
{
public:
	PositionTest(StrategyManager* manager);

	void updateTest();

private:
	StrategyManager* _manager;
};

class OpponentTest : public PointTest
{
public:
	OpponentTest(StrategyManager* manager);

	void updateTest();

private:
	StrategyManager* _manager;
};

class OrientationTest : public DoubleTest
{
public:
	OrientationTest(bool degrees, StrategyManager* manager);

	void updateTest();

private:
	StrategyManager* _manager;
	bool _degrees;
};

class SensorTest : public IntTest
{
public:
	SensorTest(int sensorId, Comm::SensorType type, StrategyManager* manager);

	void updateTest();

private:
	StrategyManager* _manager;
	int _sensorId;
	Comm::SensorType _type;
};

class StrategyReversedTest : public SingleBoolTest
{
public:
	StrategyReversedTest(StrategyManager* manager);

	void updateTest();

private:
	StrategyManager* _manager;
};

//----------------------------------------------------------------------------------------------------------------------------------
class PositionSwitchCaseAction : public AbstractSwitch<QPointF, QRectF>
{
public:
	PositionSwitchCaseAction(StrategyManager* manager, QObject* parent = 0);

	QString getActionName() const;
};

class OpponentSwitchCaseAction : public AbstractSwitch<QPointF, QRectF>
{
public:
	OpponentSwitchCaseAction(StrategyManager* manager, QObject* parent = 0);

	QString getActionName() const;
};

class OrientationSwitchCaseAction : public AbstractSwitch<double, QPair<double, double>>
{
public:
	OrientationSwitchCaseAction(StrategyManager* manager, bool degrees = false, QObject* parent = 0);

	QString getActionName() const;
};

class SensorSwitchCaseAction : public AbstractSwitch<int, QPair<int, int>>
{
public:
	SensorSwitchCaseAction(int sensorId, Comm::SensorType type, StrategyManager* manager, QObject* parent = 0);

	void addCase(int value, AbstractAction* associatedAction);

	QString getActionName() const;
};

class StrategyReversedSwitchCaseAction : public AbstractSwitch<bool, bool>
{
public:
	StrategyReversedSwitchCaseAction(StrategyManager* manager, QObject* parent = 0);

	QString getActionName() const;
};



#endif // CONDITIONALACTIONS_H
