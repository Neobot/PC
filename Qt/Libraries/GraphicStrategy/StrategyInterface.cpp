#include "StrategyInterface.h"
#include "ToolsLib.h"
#include "StrategyManager.h"
#include "StrategyMap.h"
#include "vmath.h"

const QString PARAMETERS_FILE = "parameters.nsettings";

StrategyInterface::StrategyInterface(const QDir &strategyDirectory, Tools::AbstractLogger *logger)
	: LoggerInterface(logger), _manager(0), _strategyDirectory(strategyDirectory), _mirrored(false)
{
	_grid = new Tools::NGrid;
}

StrategyInterface::~StrategyInterface()
{
	delete _grid;
}

QString StrategyInterface::getSettingsFileName()
{
	return PARAMETERS_FILE;
}

bool StrategyInterface::load(StrategyManager* manager, bool mirror)
{
	_manager = manager;
	_mirrored = mirror;

	init();

	if (!_strategyDirectory.exists(PARAMETERS_FILE))
	{
		Q_ASSERT(false);
		return false;
	}

	//Loads the grid
	_grid->clear();
	QString gridPath = _strategyDirectory.absoluteFilePath(_standardParameters.grid);
	if (!_grid->readFromFile(gridPath, _mirrored))
		return false;

	//Check the grid
	if (!checkGrid(_grid))
		return false;

	return true;
}

void StrategyInterface::init()
{
	Tools::NSettings parametersSettings;

	if (_strategyDirectory.exists(PARAMETERS_FILE))
		parametersSettings.loadFrom(_strategyDirectory.absoluteFilePath(PARAMETERS_FILE));

	readAndDefineParameters(parametersSettings);

	if (parametersSettings.hasChanges())
		parametersSettings.writeTo(_strategyDirectory.absoluteFilePath(PARAMETERS_FILE));

	parametersSettings.beginGroup("Pather");
	QString gridPath = _strategyDirectory.absoluteFilePath(_standardParameters.grid);
	if (!_strategyDirectory.exists(gridPath))
		writeDefaultGrid(gridPath);
	parametersSettings.endGroup();
}

QList<QPointF> StrategyInterface::doDetection(const QMap<int, const Sharp *> &activatedSharps) const
{
	QList<QPointF> result;
	foreach(const Sharp* sharp, activatedSharps)
	{
		result << _manager->getMap()->getSharpDetectionPoint(*sharp);
	}

	return result;
}

Tools::RPoint StrategyInterface::autoMirror(const Tools::RPoint& point) const
{
	return Tools::autoMirror(point, _mirrored, manager()->getMap()->getTableRect().height());
}

double StrategyInterface::autoMirror(double angle) const
{
	return _mirrored ? -angle : angle;
}

void StrategyInterface::readAndDefineParameters(Tools::NSettings &settings)
{
	settings.beginGroup("Strategy");

	_standardParameters.loop = defineSettingValue(settings, "loop", _standardParameters.loop, "If true, the strategy will be restarted once finished.").toBool();
	_standardParameters.replanInterval = defineSettingValue(settings, "replanInterval", _standardParameters.replanInterval, "Interval in ms before a replan if the previous one was not possible.").toInt();
	_standardParameters.stopCircleRadius = defineSettingValue(settings, "stopCircleRadius", _standardParameters.stopCircleRadius, "Radius in mm of the circle where the robot is considered arrived at its point.").toDouble();
	_standardParameters.tableSize = defineSettingValue(settings, "tableSize", _standardParameters.tableSize, "Size of the table in mm.").toSizeF();
	_standardParameters.robotRadius = defineSettingValue(settings, "robotRadius", _standardParameters.robotRadius, "Maximum radius of the robot in mm.").toDouble();
	_standardParameters.opponentRadius = defineSettingValue(settings, "opponentRadius", _standardParameters.opponentRadius, "Maximum radius of the opponent in mm.").toDouble();
	_standardParameters.noticeOfReceiptTimeOut = defineSettingValue(settings, "noticeOfReceiptTimeOut", _standardParameters.noticeOfReceiptTimeOut, "Timeout in ms for notice of receipt in comm messages.").toInt();
	settings.endGroup();

	settings.beginGroup("Avoiding");
	_standardParameters.enableAutoAvoiding = defineSettingValue(settings, "enableAutoAvoiding", _standardParameters.enableAutoAvoiding, "If true, the automatic avoiding is enabled.").toBool();
	_standardParameters.sharpObjectRadius = defineSettingValue(settings, "sharpObjectRadius", _standardParameters.sharpObjectRadius, "Radius in mm of an object detected by a sensor.").toDouble();
	_standardParameters.sharpDetectionOverlapRadius = defineSettingValue(settings, "sharpDetectionOverlapRadius", _standardParameters.sharpDetectionOverlapRadius, "Radius in mm where 2 sharp object are considered overlapping.").toDouble();
	_standardParameters.sharpDetectionTime = defineSettingValue(settings, "sharpDetectionTime", _standardParameters.sharpDetectionTime, "Time in ms where a sharp object stay on the map.").toInt();
	_standardParameters.noDetectionZones = Tools::convertVariantListToList<QRectF>(
				defineSettingValueList(settings, "noDetectionZones", Tools::convertListToVariantList<QRectF>(_standardParameters.noDetectionZones), QVariant::RectF, "List of area on the map where the no robot can go.").toList());
	settings.endGroup();

	settings.beginGroup("Pather");
	_standardParameters.start = defineSettingValue(settings, "start", _standardParameters.start, "Start point of the robot.").toPointF();
	_standardParameters.startRotation = defineSettingValue(settings, "startRotation", _standardParameters.startRotation, "Start rotation of the robot.").toDouble();
	_standardParameters.diagonalSmoothingMaxDistance = defineSettingValue(settings, "diagonalSmoothingMaxDistance", _standardParameters.diagonalSmoothingMaxDistance, "Maximum distance for the diagonal smoothing to apply.").toInt();
	_standardParameters.lostReplanInterval = defineSettingValue(settings, "lostReplanInterval", _standardParameters.lostReplanInterval, "Time in ms which define the maximum time allowed to the robot to accomplish his trajectory.").toDouble();
	_standardParameters.turnThenMoveMinAngle = Tools::degreeToRadian(defineSettingValue(settings, "turnThenMoveMinAngle", Tools::radianToDegree(_standardParameters.turnThenMoveMinAngle), "Minimum angle to the next point where the turn and move is allowed in degrees.").toDouble());
	settings.endGroup();
}

void StrategyInterface::writeDefaultGrid(const QString& filePath)
{
	Tools::NGrid defaultGrid;
	defaultGrid.makeStandardGrid(QPointF(250,250), 50, 50, QSizeF(1500, 2500), Tools::NGrid::HeightConnections);
	defaultGrid.writeToFile(filePath);
}

QVariant StrategyInterface::defineSettingValue(Tools::NSettings &settings, const QString &name, const QVariant &defaultValue, const QString &description)
{
	if (settings.contains(name))
	{
		settings.updateDescription(name, description);
		return settings.value(name);
	}
	else
	{
		settings.setValue(name, defaultValue, description);
		return defaultValue;
	}
}

QVariant StrategyInterface::defineSettingValueList(Tools::NSettings &settings, const QString &name, const QVariantList &defaultValue, QVariant::Type contentType, const QString &description)
{
	if (settings.contains(name))
	{
		settings.updateDescription(name, description);
		return settings.value(name);
	}
	else
	{
		settings.setValueList(name, defaultValue, contentType, description);
		return defaultValue;
	}
}

const StrategyInterface::StrategyParameters &StrategyInterface::getParameters() const
{
	return _standardParameters;
}

Tools::RPoint StrategyInterface::getStartPosition() const
{
	Tools::RPoint startPoint(_standardParameters.start, _standardParameters.startRotation);
	return autoMirror(startPoint);
}

Tools::NGrid *StrategyInterface::getGrid() const
{
	return _grid;
}
