#ifndef STRATEGYENUMERATOR_H
#define STRATEGYENUMERATOR_H

#include "StrategyInterface.h"

class StrategyEnumerator
{
public:
	enum Strategy
	{
		StratPrehistobot,
		StratTestStrategy,
		StratTestAx12,
		StratCount
	};

	StrategyEnumerator();

	StrategyInterface* start(Strategy strategy, Tools::AbstractLogger *logger = 0);

	static QString getStrategyName(Strategy strategy);
	static QStringList getAvailableStrategies();

	QStringList askStrategyFiles(Strategy strategy) const;
	QByteArray askStrategyFileData(Strategy strategy, const QString &filename) const;
	void setStrategyFileData(Strategy strategy, const QString &filename, const QByteArray &data) const;
	void resetStrategyFile(Strategy strategy, const QString &filename);

	QString getDataDirectory() const;

private:
	QDir _strategiesDirectory;

	bool getStrategyDirectory(Strategy strategy, QDir &strategyDirectory) const;
};

#endif // STRATEGYENUMERATOR_H
