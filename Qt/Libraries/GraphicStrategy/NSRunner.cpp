#include "NSRunner.h"
#include "StrategyManager.h"
#include "StrategyMap.h"
#include "NSParser.h"

NSRunner::NSRunner(Comm::RobotCommInterface *robot, Pather *pather, Tools::Ax12MovementManager* movementManager, Tools::AbstractLogger *logger)
	: Tools::LoggerInterface(logger)
{
	_manager = new StrategyManager(robot, pather);
	_manager->_map = new StrategyMap(QSizeF(2000,3000), 400, 400);
	_manager->_actionFactory = _manager->createActionFactory();
	_manager->_actionFactory->setPatherEnabled(false);
	_manager->setAx12MovementManager(movementManager);
	connect(_manager, SIGNAL(strategyFinished()), this, SIGNAL(scriptFinished()));

}

NSRunner::~NSRunner()
{
	delete _manager;
}

bool NSRunner::startScript(const QString& scriptCode)
{
	bool ok = false;

	if (!_manager->isRunning())
	{
		QList<AbstractAction*> actions;

		NSParser parser(_manager->_actionFactory);
		if (parser.parse(scriptCode, actions))
		{
			ok = true;
			_manager->_actions = actions;
			_manager->next();
		}
	}

	return ok;
}
