#ifndef TESTSTRATEGY_H
#define TESTSTRATEGY_H

#include "DefaultStrategy.h"

class TestStrategy : public DefaultStrategy
{
public:
	TestStrategy(const QDir& strategyDirectory, Tools::AbstractLogger* logger = 0);

    void mainStrategy(QList<AbstractAction*>& actions);
	void actionDone(const AbstractAction *action, bool result, bool isLast);

	virtual void readAndDefineParameters(Tools::NSettings &settings);

private:
	QList<QPointF> _points;
	int _waitTimeBetweenPoints; //ms

	bool _forceForward;
	bool _forceBackward;
};

#endif // TESTSTRATEGY_H
