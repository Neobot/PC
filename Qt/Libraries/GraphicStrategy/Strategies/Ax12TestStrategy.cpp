#include "Ax12TestStrategy.h"

#include <SharpFactory.h>
#include <StrategyManager.h>
#include <StrategyMap.h>
#include <vmath.h>
#include <cmath>
#include "ToolsLib.h"

#include <QtDebug>

using namespace Tools;

Ax12TestStrategy::Ax12TestStrategy(const QDir &strategyDirectory, AbstractLogger *logger)
	: DefaultStrategy(strategyDirectory, logger), _waitTimeBetweenPoints(1000), _speedLimit(50.0)
{
	//default points

	LoggerInterface::logger() << "--- AX12 TEST STRATEGY---" << Tools::endl;
}

void Ax12TestStrategy::mainStrategy(QList<AbstractAction *> &actions)
{
	LoggerInterface::logger() << "AX12 TEST STRATEGY.......GO !" << Tools::endl;

	actions << _manager->getActionFactory()->waitAction(2000);

	foreach(const QString& m, _movements)
	{
		QString group = m.section("/", 0, 0);
		QString mvt = m.section("/", -1, -1);
		actions << _manager->getActionFactory()->ax12Movement(group, mvt, _speedLimit);
		if (_waitTimeBetweenPoints > 0)
			actions << _manager->getActionFactory()->waitAction(_waitTimeBetweenPoints);
	}
}

void Ax12TestStrategy::actionDone(const AbstractAction *action, bool result, bool isLast)
{
	Q_UNUSED(isLast);
	LoggerInterface::logger() << "ACTION \"" << action->getActionName() << (result ? "\" SUCCEDED" : "\" FAILED") << Tools::endl;
}

void Ax12TestStrategy::readAndDefineParameters(NSettings &settings)
{
	DefaultStrategy::readAndDefineParameters(settings);

	settings.beginGroup("AX-12 test strategy");

	_waitTimeBetweenPoints = defineSettingValue(settings, "wait_time_between_points", _waitTimeBetweenPoints, "Time in ms to wait beetween the movements.").toInt();
	_speedLimit = defineSettingValue(settings, "speed_limit", _speedLimit, "Speed limit of the movements.").toDouble();

	_movements = convertVariantListToList<QString>(defineSettingValueList(settings, "movement_list", convertListToVariantList<QString>(_movements), QVariant::String, "The list of movements: group/movement").toList());

	settings.endGroup();
}
