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

	if (!_strategyDirectory.exists(PARAMETERS_FILE))
	{
		writeDefaultParametersToFile(parametersSettings);
		parametersSettings.writeTo(_strategyDirectory.absoluteFilePath(PARAMETERS_FILE));
	}

	parametersSettings.loadFrom(_strategyDirectory.absoluteFilePath(PARAMETERS_FILE));
	readParametersFromFile(parametersSettings);

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

void StrategyInterface::readParametersFromFile(Tools::NSettings &settings)
{
	settings.beginGroup("Strategy");

	double fileVersion = manageParameterVersion(settings, "basic_parameters", 1.2);
	Q_UNUSED(fileVersion); //unused for now

	_standardParameters.loop = settings.value("loop").toBool();
	_standardParameters.replanInterval = settings.value("replanInterval").toInt();
	_standardParameters.stopCircleRadius = settings.value("stopCircleRadius").toDouble();
	_standardParameters.tableSize = settings.value("tableSize").toSizeF();
	_standardParameters.robotRadius = settings.value("robotRadius").toDouble();
	_standardParameters.opponentRadius = settings.value("opponentRadius").toDouble();
	_standardParameters.noticeOfReceiptTimeOut = settings.value("noticeOfReceiptTimeOut").toInt();
	settings.endGroup();

	settings.beginGroup("Avoiding");
	_standardParameters.enableAutoAvoiding = settings.value("enableAutoAvoiding").toBool();
	_standardParameters.sharpObjectRadius = settings.value("sharpObjectRadius").toDouble();
	_standardParameters.sharpDetectionOverlapRadius = settings.value("sharpDetectionOverlapRadius").toDouble();
	_standardParameters.sharpDetectionTime = settings.value("sharpDetectionTime").toInt();
	_standardParameters.noDetectionZones = Tools::convertVariantListToList<QRectF>(settings.value("noDetectionZones").toList());
	settings.endGroup();

	settings.beginGroup("Pather");
	_standardParameters.start = settings.value("start").toPointF();
	_standardParameters.startRotation = settings.value("startRotation").toDouble();

	_standardParameters.diagonalSmoothingMaxDistance = settings.value("diagonalSmoothingMaxDistance").toInt();
	_standardParameters.lostReplanInterval = settings.value("lostReplanInterval").toDouble();
	if (fileVersion == 1.1)
		settings.setValue("turnThenMoveMinAngle", Tools::radianToDegree(_standardParameters.turnThenMoveMinAngle), "Minimum angle to the next point where the turn and move is allowed");
	_standardParameters.turnThenMoveMinAngle = Tools::degreeToRadian(settings.value("turnThenMoveMinAngle").toDouble());
	settings.endGroup();

	settings.beginGroup("AX-12");
	if (fileVersion == 1.0)
		settings.setValueList("ids", Tools::convertListToVariantList<int>(_standardParameters.ax12Ids), QVariant::Int, "List of AX-12 which can be controlled.");
	_standardParameters.ax12Ids = Tools::convertVariantListToList<int>(settings.value("ids").toList());
	settings.endGroup();
}

void StrategyInterface::writeDefaultParametersToFile(Tools::NSettings &settings)
{
	StrategyInterface::StrategyParameters defaultParameters;
	defaultStrategyParameters(defaultParameters);

	settings.beginGroup("Strategy");
	settings.setValue("loop", defaultParameters.loop, "If true, the strategy will be restarted once finished.");
	settings.setValue("replanInterval", defaultParameters.replanInterval, "Interval in ms before a replan if the previous one was not possible.");
	settings.setValue("stopCircleRadius", defaultParameters.stopCircleRadius, "Radius in mm of the circle where the robot is considered arrived at its point.");
	settings.setValue("tableSize", defaultParameters.tableSize, "Size of the table in mm.");
	settings.setValue("robotRadius", defaultParameters.robotRadius, "Maximum radius of the robot in mm.");
	settings.setValue("opponentRadius", defaultParameters.opponentRadius, "Maximum radius of the opponent in mm.");
	settings.setValue("noticeOfReceiptTimeOut", defaultParameters.noticeOfReceiptTimeOut, "Timeout in ms for notice of receipt in comm messages.");
	settings.endGroup();

	settings.beginGroup("Avoiding");
	settings.setValue("enableAutoAvoiding", defaultParameters.enableAutoAvoiding, "If true, the automatic avoiding is enabled.");
	settings.setValue("sharpObjectRadius", defaultParameters.sharpObjectRadius, "Radius in mm of an object detected by a sensor.");
	settings.setValue("sharpDetectionOverlapRadius", defaultParameters.sharpDetectionOverlapRadius, "Radius in mm where 2 sharp object are considered overlapping.");
	settings.setValue("sharpDetectionTime", defaultParameters.sharpDetectionTime, "Time in ms where a sharp object stay on the map.");
	settings.setValueList("noDetectionZones", Tools::convertListToVariantList<QRectF>(defaultParameters.noDetectionZones), QVariant::RectF, "List of area on the map where the no robot can go.");
	settings.endGroup();

	settings.beginGroup("Pather");
	settings.setValue("start", defaultParameters.start, "Start point of the robot.");
	settings.setValue("startRotation", defaultParameters.startRotation, "Start rotation of the robot.");
	settings.setValue("diagonalSmoothingMaxDistance", defaultParameters.diagonalSmoothingMaxDistance, "Maximum distance for the diagonal smoothing to apply.");
	settings.setValue("lostReplanInterval", defaultParameters.lostReplanInterval, "Time in ms which define the maximum time allowed to the robot to accomplish his trajectory.");
	settings.setValue("turnThenMoveMinAngle", Tools::radianToDegree(defaultParameters.turnThenMoveMinAngle), "Minimum angle to the next point where the turn and move is allowed in degrees.");
	settings.endGroup();

	settings.beginGroup("AX-12");
	settings.setValueList("ids", Tools::convertListToVariantList<int>(defaultParameters.ax12Ids), QVariant::Int, "List of AX-12 which can be controlled.");
	settings.endGroup();
}

double StrategyInterface::manageParameterVersion(Tools::NSettings &settings, const QString &prefix, double actualVersion) const
{
	QString key(prefix);
	key += "_version";

	QString currentGroup = settings.getCurrentGroup();
	settings.beginGroup("Versions");

	double version = settings.value(key, actualVersion).toDouble();

	if (!settings.contains(key) || settings.value(key) != actualVersion)
		settings.setValue(key, actualVersion, "version number", true);

	settings.endGroup();

	settings.beginGroup(currentGroup);

	return version;
}

void StrategyInterface::writeDefaultGrid(const QString& filePath)
{
	Tools::NGrid defaultGrid;
	defaultGrid.makeStandardGrid(QPointF(250,250), 50, 50, QSizeF(1500, 2500), Tools::NGrid::HeightConnections);
	defaultGrid.writeToFile(filePath);
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
