#ifndef PREHISTOBOTSTRATEGY_H
#define PREHISTOBOTSTRATEGY_H

#include <QObject>
#include "DefaultAIStrategy.h"

class PrehistobotStrategy : public QObject, public DefaultAIStrategy
{
	Q_OBJECT

public:
	PrehistobotStrategy(const QDir &strategyDirectory, Tools::AbstractLogger *logger);

	virtual void defaultStrategyParameters(StrategyParameters& parameters) const;

	virtual void readParametersFromFile(Tools::NSettings &settings);
	virtual void writeDefaultParametersToFile(Tools::NSettings& settings);

	void initGameState(GameState& state) const;
	void createActions();

	void defaultObstaclePositions(QList<Tools::DataObject *> &objects) const;

	void microswitchChanged(const QList<int> &raisingEdge, const QList<int> &faillingEdge);

private:
	bool checkGrid(const Tools::NGrid *grid) const;

	//parameters
	QString _leftArmAX12Group;
	QString _rightArmAX12Group;
};

#endif // PREHISTOBOTSTRATEGY_H
