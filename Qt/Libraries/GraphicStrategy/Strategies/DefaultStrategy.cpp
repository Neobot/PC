#include "DefaultStrategy.h"
#include <SharpFactory.h>
#include "StrategyManager.h"
#include "StrategyMap.h"
#include <vmath.h>
#include <cmath>
#include <QtDebug>
#include "ToolsLib.h"

DefaultStrategy::DefaultStrategy(const QDir& strategyDirectory, Tools::AbstractLogger *logger)
	: StrategyInterface(strategyDirectory, logger)
{
}

void DefaultStrategy::defaultStrategyParameters(StrategyInterface::StrategyParameters &parameters) const
{
	parameters. tableSize = QSizeF(2000.0, 3000.0);	//mm
    parameters. robotRadius = 270.0;			//mm

    parameters. noDetectionZones = QList<QRectF>();

	parameters. start = QPointF(250, 250);
    parameters. startRotation = Tools::pi/2;
}

QList<QPointF> DefaultStrategy::doDetection(const QMap<int, const Sharp *> &activatedSharps) const
{
	QList<QPointF> result;

	QMap<int, const Sharp *> activatedSharpsCopy(activatedSharps);
	if (activatedSharpsCopy.contains(FRONT_LEFT_SONAR) && activatedSharpsCopy.contains(FRONT_RIGHT_SONAR))
	{
		result << doDetection(activatedSharpsCopy.value(FRONT_LEFT_SONAR), activatedSharpsCopy.value(FRONT_RIGHT_SONAR));
		activatedSharpsCopy.remove(FRONT_LEFT_SONAR);
		activatedSharpsCopy.remove(FRONT_RIGHT_SONAR);
	}

	if (activatedSharpsCopy.contains(REAR_LEFT_SONAR) && activatedSharpsCopy.contains(REAR_RIGHT_SONAR))
	{
		result << doDetection(activatedSharpsCopy.value(REAR_LEFT_SONAR), activatedSharpsCopy.value(REAR_RIGHT_SONAR));
		activatedSharpsCopy.remove(REAR_LEFT_SONAR);
		activatedSharpsCopy.remove(REAR_RIGHT_SONAR);
	}

	for(QMap<int, const Sharp *>::const_iterator it = activatedSharpsCopy.constBegin(); it != activatedSharpsCopy.constEnd(); ++it)
	{
		QPointF p = doDetection(*it);
		result << p;
	}

    return result;
}

QPointF DefaultStrategy::doDetection(const Sharp *s1, const Sharp *s2) const
{
	double averageDistance = s1->getDistance() + s2->getDistance();
	averageDistance /= 2;
	QVector2D averageDirection = s1->getDirection() + s2->getDirection();
	averageDirection/= 2;
	return _manager->getMap()->getSharpDetectionPoint(QPointF(50, 0), averageDirection, averageDistance);
}

QPointF DefaultStrategy::doDetection(const Sharp *s1) const
{
	return _manager->getMap()->getSharpDetectionPoint(*s1);
}

void DefaultStrategy::avoidingSharps(QMap<int, Sharp *> &sharpList) const
{
    //front
	sharpList.insert( FRONT_LEFT_SONAR, SharpFactory::MaxSonar_MB1240(400, QVector2D(40, -40), QVector2D(1, sin(Tools::degreeToRadian(10))), 0.0) );
	sharpList.insert( FRONT_RIGHT_SONAR, SharpFactory::MaxSonar_MB1240(400, QVector2D(40, -40), QVector2D(1, sin(Tools::degreeToRadian(-10))), 0.0) );

    //rear
	sharpList.insert( REAR_LEFT_SONAR, SharpFactory::MaxSonar_MB1240(400, QVector2D(-40, 40), QVector2D(-1, sin(Tools::degreeToRadian(170))), 0.0) );
	sharpList.insert( REAR_RIGHT_SONAR, SharpFactory::MaxSonar_MB1240(400, QVector2D(-40, -40), QVector2D(-1, sin(Tools::degreeToRadian(190))), 0.0) );
}
