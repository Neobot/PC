#ifndef AX12TESTSTRATEGY_H
#define AX12TESTSTRATEGY_H

#include "DefaultStrategy.h"

class Ax12TestStrategy : public DefaultStrategy
{
public:
	Ax12TestStrategy(const QDir& strategyDirectory, Tools::AbstractLogger* logger = 0);

    void mainStrategy(QList<AbstractAction*>& actions);
	void actionDone(const AbstractAction *action, bool result, bool isLast);

	virtual void readAndDefineParameters(Tools::NSettings &settings);

private:
	QStringList _movements;
	int _waitTimeBetweenPoints; //ms

	double _speedLimit;
};

#endif // AX12TESTSTRATEGY_H
