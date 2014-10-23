#ifndef ABSTRACTSTRATEGY_H
#define ABSTRACTSTRATEGY_H

#include <QGraphicsItem>
#include <QPointF>
#include <QRectF>
#include <QDir>

#include "Curve.h"
#include "Sharp.h"
#include "DataObject.h"
#include "NGrid.h"
#include "AbstractLogger.h"
#include "NSettings.h"
#include "vmath.h"

class StrategyMap;
class AbstractAction;
class StrategyManager;
class ActionFactory;

class StrategyInterface : public Tools::LoggerInterface
{
public:
    struct StrategyParameters
    {
		StrategyParameters() : loop(false), replanInterval(1000), stopCircleRadius(200), tableSize(3000.0, 2100.0),
			robotRadius(350), opponentRadius(350), noticeOfReceiptTimeOut(300),
			enableAutoAvoiding(true), sharpObjectRadius(50), sharpDetectionOverlapRadius(50), sharpDetectionTime(2000),
			grid("map.ngrid"), start(0, 0), startRotation(0.0), diagonalSmoothingMaxDistance(400), lostReplanInterval (5000), turnThenMoveMinAngle(Tools::pi/6) {}

		//Strategy parameters
		bool loop;		    //if true, the strategy is repeated when finished
		int replanInterval;         //ms
		double stopCircleRadius;    //mm
		QSizeF tableSize;	    //mm
		double robotRadius;	    //mm
		double opponentRadius;	    //mm
		int noticeOfReceiptTimeOut; //ms

		//Avoiding parameters
		bool enableAutoAvoiding;            //enable/disable auto-avoiding
		double sharpObjectRadius;           //mm
		double sharpDetectionOverlapRadius; //mm
		int sharpDetectionTime;             //ms
		QList<QRectF> noDetectionZones;     //zones where detected objects are ignored

		//Pather parameters
		QString grid;
		QPointF start;
		double startRotation;
		double diagonalSmoothingMaxDistance;//mm
		double lostReplanInterval;          //ms
		double turnThenMoveMinAngle;		//radian
	};

	StrategyInterface(const QDir& strategyDirectory, Tools::AbstractLogger* logger = 0);
	virtual ~StrategyInterface();

	void init();

    QDir getStrategyDirectory() const;

	virtual bool load(StrategyManager* manager, bool mirror);
	StrategyManager* manager() const {return _manager;}

	static QString getSettingsFileName();
	virtual void defaultStrategyParameters(StrategyParameters& parameters) const {Q_UNUSED(parameters);}

	const StrategyInterface::StrategyParameters& getParameters() const;
	Tools::NGrid* getGrid() const;

    virtual void mainStrategy(QList<AbstractAction*>& actions) = 0;
	virtual void actionDone(const AbstractAction* action, bool result, bool isLast) {Q_UNUSED(action); Q_UNUSED(result); Q_UNUSED(isLast);}

	virtual QList<QPointF> doDetection(const QMap<int, const Sharp*>& activatedSharps) const;

    virtual void obstacleDetected() {}
    virtual void blockingDeteced() {}

	virtual void activatedSensors(QMap<int, Sharp*>& sharpList) const {Q_UNUSED(sharpList);}

    virtual void defaultObstaclePositions(QList<Tools::DataObject*>& objects) const {Q_UNUSED(objects);}

	Tools::RPoint getStartPosition() const;
	
    Tools::RPoint autoMirror(const Tools::RPoint& point) const;
	double autoMirror(double angle) const;

protected:
    StrategyManager* _manager;
	QDir _strategyDirectory;

	StrategyInterface::StrategyParameters _standardParameters;
	Tools::NGrid* _grid;
	bool _mirrored;

    QHash<int, QString> _additionalFiles;

    void registerAdditionalFile(int additionalFileIndex, const QString& additionalFileName);

    virtual void writeDefaultAdditionalFile(int additionalFileIndex, const QString& filePath);
	virtual void writeDefaultGrid(const QString &filePath);
	virtual bool checkGrid(const Tools::NGrid* grid) const {Q_UNUSED(grid); return true;}
	virtual void readAndDefineParameters(Tools::NSettings& settings);

	QVariant defineSettingValue(Tools::NSettings& settings, const QString& name, const QVariant& defaultValue, const QString& description = QString());
	QVariant defineSettingValueList(Tools::NSettings& settings, const QString& name, const QVariantList& defaultValue, QVariant::Type contentType, const QString& description = QString());
};

#endif // ABSTRACTSTRATEGY_H
