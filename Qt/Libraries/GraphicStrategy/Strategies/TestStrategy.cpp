#include "TestStrategy.h"

#include <SharpFactory.h>
#include <StrategyManager.h>
#include <StrategyMap.h>
#include <vmath.h>
#include <cmath>
#include "ToolsLib.h"

#include <QtDebug>

using namespace Tools;

TestStrategy::TestStrategy(const QDir &strategyDirectory, AbstractLogger *logger)
	: DefaultStrategy(strategyDirectory, logger), _waitTimeBetweenPoints(1000), _forceForward(false), _forceBackward(false)
{
	//default points
	_points << QPointF(300, 1000) << QPointF(1000, 1000) << QPointF(1000, 300) << QPointF(300, 300);

    LoggerInterface::logger() << "---TEST STRATEGY---" << Tools::endl;
}

void TestStrategy::mainStrategy(QList<AbstractAction *> &actions)
{
    LoggerInterface::logger() << "TEST STRATEGY.......GO !" << Tools::endl;

	QList<AbstractAction *> armActions;
	armActions	<< _manager->getActionFactory()->ax12Movement("BrasGauche", "GoToInit")
				<< _manager->getActionFactory()->ax12Movement("BrasDroit", "GoToInit");

	actions << _manager->getActionFactory()->asynchroneActionList(armActions, AsynchroneActionGroup::AllActionFinished);

	foreach(const QPointF& p, _points)
	{
		actions << _manager->getActionFactory()->moveAction(autoMirror(Tools::RPoint(p)).toQPointF(), 100, _forceForward, _forceBackward);
		if (_waitTimeBetweenPoints > 0)
			actions << _manager->getActionFactory()->waitAction(_waitTimeBetweenPoints);
	}
}

void TestStrategy::actionDone(const AbstractAction *action, bool result, bool isLast)
{
	Q_UNUSED(isLast);
	LoggerInterface::logger() << "ACTION \"" << action->getActionName() << (result ? "\" SUCCEDED" : "\" FAILED") << Tools::endl;
}

void TestStrategy::readAndDefineParameters(NSettings &settings)
{
	DefaultStrategy::readAndDefineParameters(settings);

	settings.beginGroup("Default Strategy");

	_waitTimeBetweenPoints = defineSettingValue(settings, "wait_time_between_points", _waitTimeBetweenPoints, "Time in ms to wait beetween the points of the trajectory.").toInt();
	_forceForward = defineSettingValue(settings, "forceForward", _forceForward, "If true, the robot will always go forward.").toBool();
	_forceBackward = defineSettingValue(settings, "forceBackward", _forceBackward, "If true, the robot will always go backward except if forceForward is true.").toBool();

	_points = convertVariantListToList<QPointF>(defineSettingValueList(settings, "trajectory", Tools::convertListToVariantList<QPointF>(_points), QVariant::PointF, "The list of points which define the trajectory of the robot.").toList());

	settings.endGroup();
}
