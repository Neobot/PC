#ifndef PREHISTOBOTSTRATEGY_H
#define PREHISTOBOTSTRATEGY_H

#include <QObject>
#include <QPointF>
#include "DefaultAIStrategy.h"
#include "ActionFactory.h"

class PBActionFactory;

class PrehistobotStrategy : public QObject, public DefaultAIStrategy
{
	Q_OBJECT

public:
	struct AX12MovementNames
	{
		QString leftArmGroup = "leftArmGroup";
		QString rightArmGroup = "rightArmGroup";

		QString goToRest = "goToRest";
		QString goToScan = "goToScan";
		QString pump = "pump";
		QString moveFire = "moveFire";
		QString turnFire = "turnFire";
		QString holdFire = "holdFire";
		QString scanInMobileTorche = "scanInMobileTorche";
		QString moveOutOfMobileTorche = "moveOutOfMobileTorche";
		QString scanInFixedTorche = "scanInFixedTorche";
		QString moveOutOfFixedTorche = "moveOutOfFixedTorche";

		QString dropFire = "dropFire";
		QString dropAndTurnFire = "dropAndTurnFire";
	};

	PrehistobotStrategy(const QDir &strategyDirectory, Tools::AbstractLogger *logger);

	virtual void defaultStrategyParameters(StrategyParameters& parameters) const;
	virtual void readAndDefineParameters(Tools::NSettings &settings);

	virtual void mainStrategy(QList<AbstractAction*>& actions);
	void initGameState(GameState& state) const;
	void createActions();

	void defaultObstaclePositions(QList<Tools::DataObject *> &objects) const;

	void microswitchChanged(const QList<int> &raisingEdge, const QList<int> &faillingEdge);

protected:
	virtual bool checkGrid(const Tools::NGrid *grid) const;

	//parameters
	AX12MovementNames _ax12MvtNames;
	QList<QPointF> _searchFiresPoints;
	PBActionFactory* _pbActionFactory;
	
	double _frescoValue = 6.0;
	double _fruitPickupvalue = 3.0;
	double _fruitDropValue = 3.0;
	double _fixedTorcheValue = 1.0;
	double _mobileTorchevalue = 3.0;
	double _easyFireValue = 5.0;
	double _dropDireValue = 1.0;

	QList<QPointF> autoMirrorList(const QList<QPointF> &points);
};

class PrehistobotTestStrategy : public PrehistobotStrategy
{
	public:
		PrehistobotTestStrategy(const QDir &strategyDirectory, Tools::AbstractLogger *logger);
		
		void defaultStrategyParameters(StrategyParameters& parameters) const;
		void mainStrategy(QList<AbstractAction*>& actions);
		void readAndDefineParameters(Tools::NSettings &settings);
		void writeDefaultGrid(const QString& filePath);
};

class PBActionFactory
{
public:
	PBActionFactory(ActionFactory* standardFactory, const PrehistobotStrategy::AX12MovementNames& ax12Movements, bool reversed)
		: _factory(standardFactory), _mvt(ax12Movements), _reversed(reversed) {}

	AbstractAction* scanAndTurnFires(Tools::NGridNode* destination, int speed = 100, int timeout = -1) const;
	AbstractAction* scanAndHoldFires(Tools::NGridNode* destination, int speed = 100, int timeout = -1) const;
	AbstractAction* dropHeldFires(DefaultStrategy::PumpId pumpId);
	AbstractAction* takeAndHoldFireInMobileTorche(DefaultStrategy::PumpId pumpId);
	AbstractAction* takeAndHoldFireInFixedTorche(DefaultStrategy::PumpId pumpId);

private:
	ActionFactory* _factory;
	PrehistobotStrategy::AX12MovementNames _mvt;
	bool _reversed;

	ActionGroup* hanfleFireAction(bool left, bool ourColor, const QString &finalMvt) const;
	ActionGroup* holdFireAction(bool left) const;
	ActionGroup* dropFireAction(bool left, bool ourColor) const;

	AbstractAction* defaultColorScanAction(bool reversed, Tools::NGridNode* destination, int speed, int timeoutMs,
											AbstractAction* startAction,
											AbstractAction* leftOpponentColorAction, AbstractAction* rightOpponentColorAction,
											AbstractAction* leftOurColorAction, AbstractAction* rightOurColorAction,
											AbstractAction* endAction) const;

	int getOurColor(bool reversed) const;
	int getOpponentColor(bool reversed) const;
};


#endif // PREHISTOBOTSTRATEGY_H
