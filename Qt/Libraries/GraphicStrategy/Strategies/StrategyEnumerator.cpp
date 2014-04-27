#include "StrategyEnumerator.h"
#include "Strategies/TestStrategy.h"
#include "Strategies/PrehistobotStrategy.h"
#include "Strategies/Ax12TestStrategy.h"
#include "ToolsLib.h"

#include <QDesktopServices>

const QString NEOBOT_STRATEGIES_DIR_NAME = "NeobotStrategies";

StrategyEnumerator::StrategyEnumerator()
{
	_strategiesDirectory = Tools::getDataDirectory();
	_strategiesDirectory.mkdir(NEOBOT_STRATEGIES_DIR_NAME);
	_strategiesDirectory.cd(NEOBOT_STRATEGIES_DIR_NAME);

	for(int i = 0; i < StratCount; ++i)
	{
		StrategyInterface* strat = start((Strategy)i, 0);
		if (strat)
		{
			strat->init();
			delete strat;
		}
	}
}

StrategyInterface * StrategyEnumerator::start(StrategyEnumerator::Strategy strategy, Tools::AbstractLogger* logger)
{
	QDir d;
	if (!getStrategyDirectory(strategy, d))
		return 0;

	switch(strategy)
	{
		case StratTestStrategy:
			return new TestStrategy(d, logger);
		case StratPrehistobot:
			return new PrehistobotStrategy(d, logger);
		case StratTestAx12:
			return new Ax12TestStrategy(d, logger);
		default:
			return 0;
	}
}

QString StrategyEnumerator::getStrategyName(StrategyEnumerator::Strategy strategy)
{
	switch(strategy)
	{
		case StratTestStrategy:
			return "Test";
		case StratPrehistobot:
			return "Prehistobot AI";
		case StratTestAx12:
			return "AX-12 Movements Test";
		default:
			return QString();
	}
}

QStringList StrategyEnumerator::getAvailableStrategies()
{
	QStringList result;
	for(int i = 0; i < StratCount; ++i)
		result << getStrategyName((Strategy)i);

	return result;
}

QStringList StrategyEnumerator::askStrategyFiles(Strategy strategy) const
{
	QDir d;
	if (!getStrategyDirectory(strategy, d))
		return QStringList();

	return d.entryList(QDir::Files | QDir::Readable | QDir::Writable, QDir::Name);
}

QByteArray StrategyEnumerator::askStrategyFileData(Strategy strategy, const QString &filename) const
{
	QDir d;
	if (getStrategyDirectory(strategy, d))
	{
		QStringList filter(filename + "*");
		QStringList matchingFiles = d.entryList(filter, QDir::Files | QDir::Readable | QDir::Writable, QDir::Name);

		if (!matchingFiles.isEmpty())
		{
			QString realFileName = d.absoluteFilePath(matchingFiles.first());
			QFile file(realFileName);
			if (file.open(QIODevice::ReadOnly))
				return file.readAll();
		}
	}

	return QByteArray();
}

void StrategyEnumerator::setStrategyFileData(Strategy strategy, const QString &filename, const QByteArray &data) const
{
	QDir d;
	if (!getStrategyDirectory(strategy, d))
		return;

	QStringList filter(filename + "*");
	QStringList matchingFiles = d.entryList(filter, QDir::Files | QDir::Readable | QDir::Writable, QDir::Name);

	QString realFileName = d.absoluteFilePath(matchingFiles.value(0, filename));

	QFile file(realFileName);
	if (file.open(QIODevice::WriteOnly))
		file.write(data);
}

void StrategyEnumerator::resetStrategyFile(StrategyEnumerator::Strategy strategy, const QString &filename)
{
	QDir d;
	if (getStrategyDirectory(strategy, d))
	{
		QStringList filter(filename + "*");
		QStringList matchingFiles = d.entryList(filter, QDir::Files | QDir::Readable | QDir::Writable, QDir::Name);

		if (!matchingFiles.isEmpty())
		{
			QString realFileName = d.absoluteFilePath(matchingFiles.first());
			QFile::remove(realFileName);

			StrategyInterface* strat = start(strategy, 0);
			if (strat)
			{
				strat->init();
				delete strat;
			}
		}
	}
}

QString StrategyEnumerator::getDataDirectory() const
{
	return _strategiesDirectory.absolutePath();
}

bool StrategyEnumerator::getStrategyDirectory(StrategyEnumerator::Strategy strategy, QDir& strategyDirectory) const
{
	QString name =  getStrategyName(strategy);
	if (name.isEmpty())
		return false;

	_strategiesDirectory.mkdir(name);
	strategyDirectory = QDir(_strategiesDirectory.absoluteFilePath(name));

	return true;
}
