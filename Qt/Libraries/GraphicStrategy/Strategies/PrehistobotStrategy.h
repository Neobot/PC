#ifndef PREHISTOBOTSTRATEGY_H
#define PREHISTOBOTSTRATEGY_H

#include <QObject>
#include "DefaultAIStrategy.h"

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
		QString goToScanInTorche;
		QString moveOutOfTorche;

		QString dropFire;
		QString dropAndTurnFire;
	};

	PrehistobotStrategy(const QDir &strategyDirectory, Tools::AbstractLogger *logger);

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
};

#endif // PREHISTOBOTSTRATEGY_H
