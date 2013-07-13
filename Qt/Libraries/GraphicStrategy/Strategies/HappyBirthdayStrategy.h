#ifndef HAPPYBIRTHDAYSTRATEGY_H
#define HAPPYBIRTHDAYSTRATEGY_H

#include <QObject>
#include "DefaultAIStrategy.h"

class HappyBirthdayStrategy : public QObject, public DefaultAIStrategy
{
	Q_OBJECT

public:
	HappyBirthdayStrategy(const QDir &strategyDirectory, Tools::AbstractLogger *logger);

	virtual void defaultStrategyParameters(StrategyParameters& parameters) const;

	virtual void readParametersFromFile(Tools::NSettings &settings);
	virtual void writeDefaultParametersToFile(Tools::NSettings& settings);

	void initGameState(GameState& state) const;
	void createActions();

	void defaultObstaclePositions(QList<Tools::DataObject *> &objects) const;

	void microswitchChanged(const QList<int> &raisingEdge, const QList<int> &faillingEdge);

private:
	bool checkGrid(const Tools::NGrid *grid) const;
	AbstractAction* _takeLeftAction;
	AbstractAction* _takeRightAction;

	//parameters
	int _plateRadius; //mm

	int _ownGlassesAreaPenalty;
	int _opponentGlassesAreaPenalty;

	int _estimatedPointsPerGlass;
	int _opponentEstimatedPointsPerGlass;

	int _containerLeftMax;
	int _containerRightMax;
	int _maxNumberOfGlassesPerContainerToMove;

	double _timeToTakeAGlass; //s
	double _timeToOpenAPresent; //s
	double _timeToBlowACandle; //s

private slots:
	void takeLeftFinished();
	void takeRightFinished();
};

#endif // HAPPYBIRTHDAYSTRATEGY_H
