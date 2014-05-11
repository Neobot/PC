#ifndef PREHISTOBOTSTRATEGY_H
#define PREHISTOBOTSTRATEGY_H

#include <QObject>
#include "DefaultAIStrategy.h"
#include "ActionFactory.h"

class PBActionFactory;

class PrehistobotStrategy : public QObject, public DefaultAIStrategy
{
	Q_OBJECT

public:
	struct AX12MovementNames
	{
		QString leftArmGroup;
		QString rightArmGroup;

		QString goToRest;
		QString goToScan;
		QString pump;
		QString moveFire;
		QString turnFire;
		QString holdFire;
		QString scanInTorche;
		QString moveOutOfTorche;

		QString dropFire;
		QString dropAndTurnFire;
	};

	PrehistobotStrategy(const QDir &strategyDirectory, Tools::AbstractLogger *logger);

	bool load(StrategyManager* manager, bool mirror);

	virtual void defaultStrategyParameters(StrategyParameters& parameters) const;
	virtual void readAndDefineParameters(Tools::NSettings &settings);

	void initGameState(GameState& state) const;
	void createActions();

	void defaultObstaclePositions(QList<Tools::DataObject *> &objects) const;

	void microswitchChanged(const QList<int> &raisingEdge, const QList<int> &faillingEdge);

private:
	bool checkGrid(const Tools::NGrid *grid) const;

	//parameters
	AX12MovementNames _ax12MvtNames;
	PBActionFactory* _pbActionFactory;
};

class PBActionFactory
{
public:
	PBActionFactory(ActionFactory* standardFactory, const PrehistobotStrategy::AX12MovementNames& ax12Movements, bool reversed)
		: _factory(standardFactory), _mvt(ax12Movements), _reversed(reversed) {}

	AbstractAction* scanAndTurnFires(Tools::NGridNode* destination, int speed = 100, int timeout = -1) const;
	AbstractAction* scanAndHoldFires(Tools::NGridNode* destination, int speed = 100, int timeout = -1) const;
	AbstractAction* dropHeldFires(DefaultStrategy::PumpId pumpId);
	AbstractAction* takeAndHoldFireInTorche(DefaultStrategy::PumpId pumpId);

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
