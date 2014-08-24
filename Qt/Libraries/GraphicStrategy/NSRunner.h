#ifndef NSRUNNER_H
#define NSRUNNER_H

#include "AbstractLogger.h"

class StrategyManager;
class Pather;

namespace Comm{class RobotCommInterface;}
namespace Tools{class Ax12MovementManager;}

class NSRunner : public QObject, public Tools::LoggerInterface
{
	Q_OBJECT

public:
	NSRunner(Comm::RobotCommInterface* robot, Pather* pather, Tools::Ax12MovementManager* movementManager, Tools::AbstractLogger* logger = 0);
	~NSRunner();

	bool startScript(const QString &scriptCode);

private:
	StrategyManager* _manager;

signals:
	void scriptFinished();

private slots:
	void strategyFinished();
};

#endif // NSRUNNER_H
