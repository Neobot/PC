#include "NSParser.h"
#include "ActionFactory.h"
#include "ns.h"
#include "ToolsLib.h"
#include "StrategyManager.h"
#include "StrategyMap.h"

#include <QFile>

#include <sstream>

NSParser::NSParser(ActionFactory* factory) : _invalidGrammar(false), _tree(nullptr), _factory(factory)
{
	//Open and read the grammar
	QFile grammarFile(":/ns/nsgrammar");
	if (grammarFile.open(QIODevice::ReadOnly))
	{
		QByteArray grammarData = grammarFile.readAll();

		std::string grammarStr(grammarData.data(), grammarData.size());
		std::stringstream stream(grammarStr, ios_base::in | ios_base::binary);

		// Load the grammar

		if (!_cgtFile.load(&stream))
		{
			_invalidGrammar = true;
		}
	}
	else
	{
		_invalidGrammar = true;
	}
}

NSParser::~NSParser()
{
    clearParsingData();
}

void NSParser::setActionFactory(ActionFactory* factory)
{
    _factory = factory;
}

void NSParser::addVariable(const QString& name, const NSParser::DeclaredVariable &variable)
{
	_variables[name] = variable;
}

void NSParser::clearVariables()
{
    _variables.clear();
}

void NSParser::addError(const NSParsingError &error)
{
	NSParsingError errorCopy(error);
	if (hasCurrentFile())
		errorCopy.setFilename(getCurrentFile().fileName());

	if (_rootSymbol)
		errorCopy.setSymbol(_rootSymbol);

	_errors << errorCopy;
}

bool NSParser::isMirrored() const
{
	if (_factory)
		return _factory->getManager()->isMirrored();
	else
		return false;
}

int NSParser::getTableWidth() const
{
	if (_factory)
		return _factory->getManager()->getMap()->getTableRect().height();
	else
		return 3000;
}

bool NSParser::hasCurrentFile() const
{
	return !_fileStack.isEmpty();
}

QFileInfo NSParser::getCurrentFile() const
{
	return _fileStack.top();
}

QString NSParser::getFileInKnownDirectories(QString filePath) const
{
	QList<QDir> searchDirs(_searchDirs);
	if (hasCurrentFile())
		searchDirs.prepend(getCurrentFile().absoluteDir());

	for(const QDir& d : searchDirs)
	{
		if (d.exists(filePath))
			return d.absoluteFilePath(filePath);
	}

	return QString();
}

const QList<NSParsingError> &NSParser::getErrors() const
{
	return _errors;
}

bool NSParser::parse(const QString &scriptCode, QList<AbstractAction *> &actions)
{
	initParsing();
    return parseMainFile(scriptCode, actions, QString());
}

bool NSParser::parseFile(const QString &filepath, QList<AbstractAction *> &actions)
{
	initParsing();

	QFile file(filepath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		_errors << NSParsingError(NSParsingError::Error, filepath, -1, -1, -1, "The file cannot be opened");
		return false;
	}

	return	parseMainFile(filepath, actions, filepath);
}

bool NSParser::verify(const QString &scriptCode)
{
	QList<AbstractAction *> actions;
	return parse(scriptCode, actions);
}

bool NSParser::verifyFile(const QString &filepath)
{
	QList<AbstractAction *> actions;
	return parseFile(filepath, actions);
}

void NSParser::print(QTextStream& out)
{
	if (_tree)
        printTree(out, _tree, 0);
}

void NSParser::setSearchDirectories(const QList<QDir> &dirs)
{
    _searchDirs = dirs;
}

void NSParser::addSearchDirectory(const QDir &dir)
{
    _searchDirs << dir;
}

void NSParser::addSearchDirectories(const QList<QDir> &dirs)
{
    _searchDirs << dirs;
}

void NSParser::removeSearchDirectory(const QDir &dir)
{
	_searchDirs.removeAll(dir);
}

void NSParser::initParsing()
{
	_errors.clear();
    _rootSymbol = nullptr;
}

void NSParser::clearParsingData()
{
    if (_tree)
        delete _tree;

    qDeleteAll(_subtrees);
    _subtrees.clear();

    _rootSymbol = nullptr;
}

Symbol *NSParser::getParsedTree(const QString &scriptCode)
{
	if (_invalidGrammar)
	{
		addError(NSParsingError(NSParsingError::Error, "", -1, -1, -1, "Critical: An error occured while loading the grammar!"));
		return nullptr;
	}

	// Get DFA (Deterministic Finite Automata) from the cgt file
	DFA* dfa = _cgtFile.getScanner();

	// Scan the source in search of tokens
	dfa->scan(scriptCode.toLatin1().data());

	//Report the dfa errors
	for (GPError* e: dfa->getErrors()->errors)
	{
		addError(NSParsingError::fromGPError(e));
	}

	// Get the tokens to feed the LALR machine with them
	vector <Token*> tokens = dfa->getTokens();

	// Get the LALR (Look Ahead, Left-to-right parse, Rightmost-derivation)
	LALR* lalr = _cgtFile.getParser();

	// Parse the tokens
	Symbol* rdc = lalr->parse(tokens, true, true);

	//Report the lalr errors
	for (GPError* e: lalr->getErrors()->errors)
	{
		addError(NSParsingError::fromGPError(e));
	}

	return rdc;
}

void NSParser::printTree(QTextStream& out, Symbol *s, int level)
{
	if (!s)
		return;

	for (int i = 0; i < level; i++)
		out << " ";
		
	QString symStr = QString::fromStdWString(s->symbol);
	if (s->type == NON_TERMINAL)
	{
		symStr.append(">").prepend("<");
	}

	out << symStr
		<< " [" << s->symbolIndex << "]";

	if (s->type == NON_TERMINAL)
	{
		NonTerminal* nt = static_cast<NonTerminal*>(s);
		out << " - Rule " << nt->ruleIndex;
	}

	out	<< " - pos " << s->line << ":" << s->col;

	if (s->trimmed)
		out << "Trimmed";
		
	if (s->type == TERMINAL)
	{
		
		Terminal* terminal = static_cast<Terminal*>(s);
		out << " : " << QString::fromStdWString(terminal->image);
	}
	
	out << "\n";
		
	if (s->type == NON_TERMINAL)
	{
		NonTerminal* nt = static_cast<NonTerminal*>(s);
		for(Symbol* child: nt->children)
		{
			printTree(out, child, level + 1);
		}
	}
}

bool NSParser::parseMainFile(const QString &scriptCode, QList<AbstractAction *> &actions, const QString &originalFilename)
{
   clearParsingData();

   FunctionList functions;
   VariableList variables(_variables);
    _tree = parseTree(scriptCode, actions, originalFilename, variables, functions);

    return _errors.isEmpty();
}

void NSParser::parseSubFile(Symbol *importStatementSymbol, const QString &filepath, QList<AbstractAction *> &actions, VariableList& variables, FunctionList& functions)
{
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
		_errors << NSParsingError::error("The file cannot be opened", importStatementSymbol);
		return;
    }

    bool isRoot = !_rootSymbol;

    if (isRoot)
        _rootSymbol = importStatementSymbol;

	Symbol* subfileTree = parseTree(file.readAll(), actions, filepath, variables, functions);
    _subtrees << subfileTree;

    if (isRoot)
        _rootSymbol = nullptr;
}

Symbol* NSParser::parseTree(const QString& scriptCode, QList<AbstractAction*>& actions, const QString& originalFilename, VariableList& variables, FunctionList& functions)
{
    Symbol* resultTree = nullptr;
	_fileStack.push(QFileInfo(originalFilename));

    resultTree = getParsedTree(QString(scriptCode).append("\n")); //Add a cariage return which is mandatory at the end

    if (resultTree)
	{
        buildActions(resultTree, actions, variables, functions);
	}

	_fileStack.pop();

    return resultTree;
}

void NSParser::buildActions(Symbol* symbol, QList<AbstractAction*>& actions, VariableList& variables, FunctionList& functions)
{
	AbstractAction* action = nullptr;
	switch(symbol->symbolIndex)
	{
		case SYM_WAITSTATEMENT: //<WaitStatement>
			action = buildWaitAction(symbol);
			break;
		case SYM_DECLARESTATEMENT:
			readVariable(symbol, variables);
			break;
		case SYM_TELEPORTSTATEMENT:
			action = buildTeleportAction(symbol, variables);
			break;
		case SYM_MOVETOSTATEMENT:
			action = buildGoToAction(symbol, variables);
			break;
		case SYM_SETPARAMSTATEMENT:
			action = buildSetParameterAction(symbol, variables);
			break;
		case SYM_ENABLESENSORSTATEMENT:
			action = buildEnableSensorAction(symbol, variables);
			break;
		case SYM_DISABLESENSORSTATEMENT:
			action = buildDisableSensorAction(symbol, variables);
			break;
		case SYM_ACTUATORSTATEMENT:
			action = buildActuatorAction(symbol, variables);
			break;
		case SYM_MOVEAX12STATEMENT:
			action = buildMoveAx12Action(symbol, variables);
			break;
		case SYM_AX12MOVEMENTSTATEMENT:
			action = buildAx12MovementAction(symbol, variables, false);
			break;
		case SYM_AX12ASYNCMOVEMENTSTATEMENT:
			action = buildAx12MovementAction(symbol, variables, true);
			break;
		case SYM_LISTSTATEMENT:
			action = buildListAction(symbol, variables, functions);
			break;
		case SYM_CONCURRENTSTATEMENT:
			action = buildConcurrentListAction(symbol, variables, functions);
			break;
		case SYM_CONDITIONSTATEMENT:
			action = buildIfAction(symbol, variables, functions);
			break;
		case SYM_WHILESTATEMENT:
			action = buildWhileAction(symbol, variables, functions);
			break;
		case SYM_FUNCTIONSTATEMENT:
			readFunction(symbol, variables, functions);
			break;
		case SYM_CALLSTATEMENT:
			action = buildCalledFunctionActions(symbol, variables, functions);
			break;
		case SYM_ROTATETOSTATEMENT:
			action = buildRotateToAction(symbol);
			break;
		case SYM_TIMEOUTSTATEMENT:
			action = buildTimeoutAction(symbol, variables, functions);
			break;
		case SYM_IMPORTSTATEMENT:
			action = buildImportActions(symbol, variables, functions);
			break;
		default:
		{
			if (symbol->type == NON_TERMINAL)
			{
				//Build sub-actions
				NonTerminal* nt = static_cast<NonTerminal*>(symbol);
				for(Symbol* child: nt->children)
					buildActions(child, actions, variables, functions);
			}
			//else : no actions to build
		}
	}

	if (action)
		actions << action;
}

void NSParser::readVariable(Symbol* symbol, VariableList& variables)
{
	if (symbol->type == NON_TERMINAL)
	{
		QString name;
		DeclaredVariable var;
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
		{
			switch(child->symbolIndex)	
			{
				case SYM_POINT: 
				case SYM_FIXED_POINT:
				{
					Tools::RPoint p = readPoint(child);
					var = DeclaredVariable::fromPoint(p);
					break;
				}
				case SYM_RECT2:
				case SYM_FIXED_RECT:
				{
					QRectF r = readRect(child, variables);
					var = DeclaredVariable::fromRect(r);
					break;
				}
				case SYM_SENSOR_IDENTIFIER:
				{
					int type = -1, id = -1;
					readSensorIdentifier(child, type, id);
					if (id <= 0)
					{
						addError(NSParsingError::invalidSensorId(child));
						return;
					}
					var = DeclaredVariable::fromSensor(id, type);
					break;
				}
				case SYM_PARAMETER_IDENTIFIER:
				{
					int paramId = readSubId(child);
					var = DeclaredVariable::fromParameter(paramId);
					break;
				}
				case SYM_AX12_IDENTIFIER:
				{
					int ax12Id = readSubId(child);
					var = DeclaredVariable::fromAx12(ax12Id);
					break;
				}
				case SYM_ACTION2:
				{
					int actionId, param, time;
					readAction(child, actionId, param, time);
					var = DeclaredVariable::fromAction(actionId, param, time);
				}
				case SYM_STRING:
				{
					QString str = readString(child);
					var = DeclaredVariable::fromString(str);
				}
				case SYM_VAR:
					name = readVar(child);
					break;
			}
		}
		
		if (!name.isEmpty())
		{
			//if (variables.contains(name))
				//error already defined
			variables[name] = var;
		}
	}
}

AbstractAction* NSParser::buildWaitAction(Symbol* symbol)
{
	if (symbol->type == NON_TERMINAL)
	{
		int time = 0; //inMs
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
		{
			switch(child->symbolIndex)	
			{
				case SYM_TIME: 
					time = readTimeInMs(child);
					break;
			}
		}
		
		if (_factory)
			return _factory->waitAction(time);
	}
	
	return nullptr;
}

AbstractAction *NSParser::buildTimeoutAction(Symbol *symbol, VariableList variables, FunctionList functions)
{
	if (symbol->type == NON_TERMINAL)
	{
		int time = 0; //inMs
		AbstractAction* action = nullptr;
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
		{
			switch(child->symbolIndex)
			{
				case SYM_TIME:
					time = readTimeInMs(child);
					break;
				default:
				{
					QList<AbstractAction*> list;
					buildActions(child, list, variables, functions);
					if (!list.isEmpty())
						action = list.first();
				}
			}
		}

		if (_factory)
			return _factory->timeoutAction(action, time);
	}

	return nullptr;
}

AbstractAction* NSParser::buildTeleportAction(Symbol* symbol, VariableList& variables)
{
	if (symbol->type == NON_TERMINAL)
	{
		bool ok = false;
		Tools::RPoint point;
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
		{
			switch(child->symbolIndex)
			{
				case SYM_POINT_OR_VAR:
				case SYM_POINT:
				case SYM_FIXED_POINT:
				case SYM_VAR:
					ok = readPointOrVar(child, variables, point);
					break;
			}
		}

		if (ok && _factory)
			return _factory->teleportAction(point);
	}

	return nullptr;
}

AbstractAction* NSParser::buildGoToAction(Symbol* symbol, VariableList& variables)
{
	if (symbol->type == NON_TERMINAL)
	{
		bool ok = false;
		Tools::RPoint point;
		Tools::Direction dir = Tools::Unknown;
		int speed = 100;
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
		{
			switch(child->symbolIndex)
			{
				case SYM_POINT_OR_VAR:
				case SYM_POINT:
				case SYM_FIXED_POINT:
				case SYM_VAR:
					ok = readPointOrVar(child, variables, point);
					break;
				case SYM_DIRECTION:
					dir = readDirection(child);
					break;
				case SYM_SPEED2:
					speed = readSpeed(child);
					break;
			}
		}

		bool forceForward = dir == Tools::Forward;
		bool forceBackward = dir == Tools::Backward;

		if (ok && _factory)
			return _factory->moveAction(point.toQPointF(), speed, forceForward, forceBackward);
	}

	return nullptr;
}

AbstractAction *NSParser::buildRotateToAction(Symbol *symbol)
{
	if (symbol->type == NON_TERMINAL)
	{
		double angle = 0.0;
		bool ok = false;
		int speed = 100;
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
		{
			switch(child->symbolIndex)
			{
				case SYM_ANGLE:
				case SYM_FIXED_ANGLE:
				case SYM_NUM:
					angle = readAngleInRadian(child);
					ok = true;
					break;
				case SYM_SPEED2:
					speed = readSpeed(child);
					break;
			}
		}

		if (ok && _factory)
			return _factory->manualAbsoluteTurnMoveAction(angle, speed);
	}

	return nullptr;
}

AbstractAction *NSParser::buildSetParameterAction(Symbol *symbol, NSParser::VariableList &variables)
{
	if (symbol->type == NON_TERMINAL)
	{
		bool ok = false;
		int paramId = -1;
		double paramValue = 0.0;
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
		{
			switch(child->symbolIndex)
			{
				case SYM_PARAMETER_IDENTIFIER:
				case SYM_PARAMETER_OR_VAR:
				case SYM_VAR:
					ok = readParameterOrVar(child, variables, paramId);
					break;
				case SYM_NUM:
					paramValue = readNum(child);
					break;
			}
		}

		if (ok && _factory)
			return _factory->setParameterAction(paramId, paramValue);
	}

	return nullptr;
}

AbstractAction *NSParser::buildEnableSensorAction(Symbol *symbol, NSParser::VariableList &variables)
{
	AbstractAction* action = nullptr;
	if (symbol->type == NON_TERMINAL)
	{
		bool ok = false;
		int sensorId = -1, sensorType = -1;
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		bool all = nt->ruleIndex == PROD_ENABLESENSORSTATEMENT_ENABLE_ALL;
		for(Symbol* child: nt->children)
		{
			switch(child->symbolIndex)
			{
				case SYM_SENSOR_IDENTIFIER:
				case SYM_SENSOR_OR_VAR:
				case SYM_VAR:
					ok = readSensorOrVar(child, variables, sensorType, sensorId);
					break;
				case SYM_SENSOR2:
					sensorType = readSensorType(child);
					break;
			}
		}

		if (all)
		{
			sensorId = 0;
			ok = true;
		}
		else if (sensorId == 0)
		{
			addError(NSParsingError::invalidSensorId(symbol));
			return nullptr;
		}

		if (ok && _factory)
		{
			switch(sensorType)
			{
				case Comm::ColorSensor:
					action = _factory->enableColorSensorAction(sensorId);
					break;
				case Comm::SharpSensor:
					action = _factory->enableSharpAction(sensorId);
					break;
				case Comm::MicroswitchSensor:
					action = _factory->enableMicroswitchAction(sensorId);
					break;
			}
		}
	}

	return action;
}

AbstractAction *NSParser::buildDisableSensorAction(Symbol *symbol, NSParser::VariableList &variables)
{
	AbstractAction* action = nullptr;
	if (symbol->type == NON_TERMINAL)
	{
		bool ok = false;
		int sensorId = -1, sensorType = -1;
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		bool all = nt->ruleIndex == PROD_DISABLESENSORSTATEMENT_DISABLE_ALL;
		for(Symbol* child: nt->children)
		{
			switch(child->symbolIndex)
			{
				case SYM_SENSOR_IDENTIFIER:
				case SYM_SENSOR_OR_VAR:
				case SYM_VAR:
					ok = readSensorOrVar(child, variables, sensorType, sensorId);
					break;
				case SYM_SENSOR2:
					sensorType = readSensorType(child);
					break;
			}
		}

		if (all)
		{
			sensorId = 0;
			ok = true;
		}
		else if (sensorId == 0)
		{
			addError(NSParsingError::invalidSensorId(symbol));
			return nullptr;
		}

		if (ok && _factory)
		{
			switch(sensorType)
			{
				case Comm::ColorSensor:
					action = _factory->disableColorSensorAction(sensorId);
					break;
				case Comm::SharpSensor:
					action = _factory->disableSharpAction(sensorId);
					break;
				case Comm::MicroswitchSensor:
					action = _factory->disableMicroswitchAction(sensorId);
					break;
			}
		}
	}

	return action;
}

AbstractAction *NSParser::buildActuatorAction(Symbol *symbol, NSParser::VariableList &variables)
{
	if (symbol->type == NON_TERMINAL)
	{
		bool ok = false;
		int actionId = -1;
		int param = -1;
		int time = 0;
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
		{
			switch(child->symbolIndex)
			{
				case SYM_ACTION2:
				case SYM_ACTION_OR_VAR:
				case SYM_VAR:
					ok = readActionOrVar(child, variables, actionId, param, time);
					break;
			}
		}

		if (ok && _factory)
			return _factory->actuatorAction((Comm::RobotAction)actionId, param, time);
	}

	return nullptr;
}

AbstractAction *NSParser::buildMoveAx12Action(Symbol *symbol, NSParser::VariableList &variables)
{
	if (symbol->type == NON_TERMINAL)
	{
		bool ok = false;
		int ax12id = -1;
		double angle = -1;
		int speed = 100;
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
		{
			switch(child->symbolIndex)
			{
				case SYM_AX12_IDENTIFIER:
				case SYM_AX12_OR_VAR:
				case SYM_VAR:
					ok = readAx12OrVar(child, variables, ax12id);
					break;
				case SYM_SPEED2:
					speed = readSpeed(child);
					break;
				case SYM_ANGLE:
				case SYM_FIXED_ANGLE:
					angle = readAngleInRadian(child);
					break;
			}
		}

		if (ok && _factory)
			return _factory->ax12Action(ax12id, Tools::radianToDegree(angle), speed);
	}

	return nullptr;
}

AbstractAction *NSParser::buildAx12MovementAction(Symbol *symbol, NSParser::VariableList &variables, bool async)
{
	if (symbol->type == NON_TERMINAL)
	{
		bool groupOk = false;
		bool mvtOk = false;
		QString group;
		QString mvt;
		int speed = 100;
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
		{
			switch(child->symbolIndex)
			{
				case SYM_STRING_OR_VAR:
				case SYM_STRING:
				case SYM_VAR:
					if (!groupOk)
						groupOk = readStringOrVar(child, variables, group);
					else
						mvtOk = readStringOrVar(child, variables, mvt);
					break;
				case SYM_SPEED2:
					speed = readSpeed(child);
					break;
			}
		}



		if (groupOk && mvtOk && _factory)
		{
			if (!async)
				return _factory->ax12Movement(group, mvt, speed);
			else
				return _factory->ax12AsynchroneMovement(group, mvt, speed);
		}
	}

	return nullptr;
}

AbstractAction *NSParser::buildListAction(Symbol *symbol, VariableList variables, FunctionList functions)
{
	QList<AbstractAction*> actionList;
	if (symbol->type == NON_TERMINAL)
	{
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
			buildActions(child, actionList, variables, functions);
	}

	if (!actionList.isEmpty() && _factory)
		return _factory->actionList(actionList);

	return nullptr;
}

AbstractAction *NSParser::buildConcurrentListAction(Symbol *symbol, VariableList variables, FunctionList functions)
{
	QList<AbstractAction*> actionList;
	AsynchroneActionGroup::StopCondition stopCondition = AsynchroneActionGroup::AllActionFinished;
	if (symbol->type == NON_TERMINAL)
	{
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
		{
			switch(child->symbolIndex)
			{
				case SYM_CONCURRENCYCONDITION:
				{
					stopCondition = (AsynchroneActionGroup::StopCondition)readConcurrencyStopCondition(child);
					break;
				}
				case SYM_LISTSTATEMENT:
				{
					if (child->type == NON_TERMINAL)
					{
						NonTerminal* ntChild = static_cast<NonTerminal*>(child);
						for(Symbol* listChild: ntChild->children)
							buildActions(listChild, actionList, variables, functions);
					}
				}
			}
		}
	}

	if (!actionList.isEmpty() && _factory)
		return _factory->asynchroneActionList(actionList, stopCondition);

	return nullptr;
}

AbstractAction *NSParser::buildIfAction(Symbol *symbol, VariableList variables, FunctionList functions)
{
	if (symbol->type == NON_TERMINAL)
	{
		ConditionInfo info;
		AbstractAction* thenAction = nullptr;
		AbstractAction* elseAction = nullptr;
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
		{
			switch(child->symbolIndex)
			{
				case SYM_IFCONDITION:
				case SYM_SENSORCONDITION:
				case SYM_POSITIONCONDITION:
				case SYM_ORIENTATIONCONDITION:
				case SYM_OPPONENTCONDITION:
				case SYM_REVERSECONDITION:
					info = readCondition(child, variables);
					break;
				default:
				{
					QList<AbstractAction*> list;
					buildActions(child, list, variables, functions);
					if (!list.isEmpty())
					{
						if (!thenAction)
							thenAction = list.first();
						else if (!elseAction)
							elseAction = list.first();
					}
				}
			}
		}

		if (_factory && info.isValid())
		{
			if (info.neg)
				qSwap(thenAction, elseAction);

			switch(info.type)
			{
				case ConditionInfo::InvalidCondition:
					break;
				case ConditionInfo::AlwaysTrueCondition:
					return thenAction;
				case ConditionInfo::AlwaysFalseCondition:
					return elseAction;
				case ConditionInfo::RobotPosCondition:
					return _factory->ifPositionAction(info.rect, thenAction, elseAction);
				case ConditionInfo::RobotOrientationCondition:
					return _factory->ifOrientationAction(info.angleMin, info.angleMax, thenAction, elseAction);
				case ConditionInfo::OpponentPosCondition:
					return _factory->ifOpponentAction(info.rect, thenAction, elseAction);
				case ConditionInfo::ColorSensorValueCondition:
					return _factory->ifColorSensorAction(info.sensorId, info.sensorValue, thenAction, elseAction);
				case ConditionInfo::SharpValueCondition:
					return _factory->ifSharpAction(info.sensorId, info.sensorValue, thenAction, elseAction);
				case ConditionInfo::MicroswitchValueCondition:
					return _factory->ifMicroswitchAction(info.sensorId, info.sensorValue, thenAction, elseAction);
				case ConditionInfo::ReversedStrategyCondition:
					return _factory->ifStrategyReversedAction(thenAction, elseAction);
			}
		}
	}

	return nullptr;
}

AbstractAction *NSParser::buildWhileAction(Symbol *symbol, VariableList variables, FunctionList functions)
{
	if (symbol->type == NON_TERMINAL)
	{
		ConditionInfo info;
		AbstractAction* loopedAction = nullptr;
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
		{
			switch(child->symbolIndex)
			{
				case SYM_IFCONDITION:
				case SYM_SENSORCONDITION:
				case SYM_POSITIONCONDITION:
				case SYM_ORIENTATIONCONDITION:
				case SYM_OPPONENTCONDITION:
				case SYM_REVERSECONDITION:
					info = readCondition(child, variables);
					break;
				default:
				{
					QList<AbstractAction*> list;
					buildActions(child, list, variables, functions);
					if (!list.isEmpty())
						loopedAction = list.first();
				}
			}
		}

		if (_factory && info.isValid() && loopedAction)
		{
			switch(info.type)
			{
				case ConditionInfo::InvalidCondition:
					break;
				case ConditionInfo::AlwaysTrueCondition:
					return _factory->infiniteLoop(loopedAction);
				case ConditionInfo::AlwaysFalseCondition:
					return nullptr;
				case ConditionInfo::RobotPosCondition:
					return _factory->whilePositionAction(info.rect, info.neg, loopedAction);
				case ConditionInfo::RobotOrientationCondition:
					return _factory->whileOrientationAction(info.angleMin, info.angleMax, info.neg, loopedAction);
				case ConditionInfo::OpponentPosCondition:
					return _factory->whileOpponentAction(info.rect, info.neg, loopedAction);
				case ConditionInfo::ColorSensorValueCondition:
					return _factory->whileColorSensorAction(info.sensorId, info.sensorValue, info.neg, loopedAction);
				case ConditionInfo::SharpValueCondition:
					return _factory->whileSharpAction(info.sensorId, info.sensorValue, info.neg, loopedAction);
				case ConditionInfo::MicroswitchValueCondition:
					return _factory->whileMicroswitchAction(info.sensorId, info.sensorValue, info.neg, loopedAction);
				case ConditionInfo::ReversedStrategyCondition:
					return _factory->whileStrategyReversedAction(info.neg, loopedAction);
			}
		}
	}

	return nullptr;
}

void NSParser::readFunction(Symbol* symbol, VariableList variables, FunctionList& functions)
{
	if (symbol->type == NON_TERMINAL)
	{
		QString functionName;
		FunctionInfo info;
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
		{
			switch(child->symbolIndex)
			{
				case SYM_VAR:
					if (functionName.isEmpty())
						functionName = readVar(child);
					else
						info.parameterNames << readVar(child);
					break;
				case SYM_VARLIST:
					readVarList(child, info.parameterNames);
					break;
				case SYM_LISTSTATEMENT:
					info.actionListSymbol = child;
					break;
			}
		}

		if (!functionName.isEmpty() && info.actionListSymbol != nullptr)
		{
			if (!functions.contains(functionName))
			{
				info.currentVariables = variables;
				info.currentFunctions = functions;
				functions[functionName] = info;
			}
			else
				addError(NSParsingError::functionAlreadyDefinedError(functionName, symbol));
		}
    }
}

void NSParser::readGrid(Symbol *symbol, const QString &gridFile, NSParser::VariableList &variables)
{
    Tools::NGrid grid;
    if (grid.readFromFile(gridFile))
    {
        for(const QString& alias : grid.getAllAliases())
        {
            Tools::NGridNode* node = grid.getNode(alias);
            node->getPosition();
            DeclaredVariable varPoint = DeclaredVariable::fromPoint(Tools::RPoint(node->getPosition()));
            variables[alias] = varPoint;
        }

		for(QHash<QString, Tools::NGridArea*>::const_iterator it = grid.getAreas().constBegin(); it != grid.getAreas().constEnd(); ++it)
        {
            QString areaName = it.key();
			const Tools::NGridArea* area = *it;

            DeclaredVariable varRect = DeclaredVariable::fromRect(area->getRect());
            variables[areaName] = varRect;
        }
    }
    else
    {
        addError(NSParsingError::loadingFileError(gridFile, symbol));
    }
}

AbstractAction* NSParser::buildCalledFunctionActions(Symbol *symbol, VariableList variables, FunctionList functions)
{
	if (symbol->type == NON_TERMINAL)
	{
		QString functionName;
		QList<DeclaredVariable> varList;
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
		{
			switch(child->symbolIndex)
			{
				case SYM_VAR:
					if (functionName.isEmpty())
						functionName = readVar(child);
					else
					{
						DeclaredVariable var;
						readCallArg(child, variables, var);
						varList << var;
					}
					break;
				case SYM_CALLARGLIST:
					readCallArgList(child, variables, varList);
					break;
				case SYM_CALLARG:
				default:
				{
					DeclaredVariable var;
					readCallArg(child, variables, var);
					if (var.isValid())
						varList << var;
					break;
				}
			}
		}

		if (!functionName.isEmpty())
		{
			if (functions.contains(functionName))
			{
				FunctionInfo& info = functions[functionName];

				if (info.parameterNames.count() != varList.count())
					addError(NSParsingError::invalidNumberOfArguments(functionName, info.parameterNames.count(), varList.count(), symbol));
				else
				{
					VariableList functionVariables(info.currentVariables);
					for(int i = 0; i < info.parameterNames.count(); ++i)
					{
						const QString& param = info.parameterNames[i];
						DeclaredVariable& var = varList[i];
						functionVariables[param] = var;
					}
					QList<AbstractAction*> actions;
					buildActions(info.actionListSymbol, actions, functionVariables, info.currentFunctions);

					if (_factory)
						return _factory->actionList(actions);
				}
			}
			else
			{
				addError(NSParsingError::undeclaredFunctionError(functionName, symbol));
			}
		}
	}

	return nullptr;
}

AbstractAction *NSParser::buildImportActions(Symbol *symbol, VariableList& variables, FunctionList& functions)
{
	if (symbol->type == NON_TERMINAL)
	{
		QList<AbstractAction*> actions;
		QString filename;
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
		{
			switch(child->symbolIndex)
			{
				case SYM_STRING:
					filename = readString(child);
					break;
			}
		}

		QString knownFile = getFileInKnownDirectories(filename);
		if (!knownFile.isEmpty())
		{
            QFileInfo info(knownFile);
            if (info.suffix().toLower() == "ngrid")
            {
                readGrid(symbol, knownFile, variables);
            }
            else
            {
                VariableList subVariables;
                FunctionList subFunctions;
                parseSubFile(symbol, knownFile, actions, subVariables, subFunctions);

                for(VariableList::const_iterator it = subVariables.constBegin(); it != subVariables.constEnd(); ++it)
                {
                    variables[it.key()] = *it;
                }

                for(FunctionList::const_iterator it = subFunctions.constBegin(); it != subFunctions.constEnd(); ++it)
                {
                    functions[it.key()] = *it;
                }
            }
		}

		if (_factory && !actions.isEmpty())
			return _factory->actionList(actions);
	}

	return nullptr;
}


Symbol* NSParser::searchChild(Symbol* symbol, unsigned short symbolIndex, bool recursive)
{
	if (symbol->type == NON_TERMINAL)
	{
		if (symbol->symbolIndex == symbolIndex)
			return symbol;

		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
		{
			if (child->symbolIndex == symbolIndex)
				return child;

			if (recursive)
			{
				Symbol* foundChild = searchChild(child, symbolIndex, true);
				if (foundChild)
					return foundChild;
			}
		}
	}

	return nullptr;
}

QString NSParser::readTerminals(Symbol *symbol)
{
	QString text;
	if (symbol->type == TERMINAL)
	{
		Terminal* t = static_cast<Terminal*>(symbol);
		text = QString::fromStdWString(t->image);
	}
	else
	{
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
		{
			text += readTerminals(child);
			text += " ";
		}
	}

	return text;
}

//-----------------------------------------------------------------

NSParser::DeclaredVariable NSParser::DeclaredVariable::fromPoint(const Tools::RPoint& p)
{
	DeclaredVariable var;
	var.type = Point;
	var.data << QVariant(QPointF(p.getX(), p.getY()))
			 << QVariant(p.getTheta());

	return var;
}

NSParser::DeclaredVariable NSParser::DeclaredVariable::fromRect(const QRectF& rect)
{
	DeclaredVariable var;
	var.type = Rect;
	var.data << QVariant(rect);

	return var;
}

NSParser::DeclaredVariable NSParser::DeclaredVariable::fromAction(int id, int param, int timeMs)
{
	DeclaredVariable var;
	var.type = Action;
	var.data << QVariant(id)
			 << QVariant(param)
			 << QVariant(timeMs);

	return var;
}

NSParser::DeclaredVariable NSParser::DeclaredVariable::fromAx12(int id)
{
	DeclaredVariable var;
	var.type = Ax12;
	var.data << QVariant(id);

	return var;
}

NSParser::DeclaredVariable NSParser::DeclaredVariable::fromParameter(int id)
{
	DeclaredVariable var;
	var.type = Param;
	var.data << QVariant(id);

	return var;
}

NSParser::DeclaredVariable NSParser::DeclaredVariable::fromSensor(int id, int type)
{
	DeclaredVariable var;
	var.type = Sensor;
	var.data << QVariant(id)
			 << QVariant(type);

	return var;
}

NSParser::DeclaredVariable NSParser::DeclaredVariable::fromString(const QString &str)
{
	DeclaredVariable var;
	var.type = String;
	var.data << str;

	return var;
}

Tools::RPoint NSParser::DeclaredVariable::toPoint() const
{
	QPointF p = data.value(0).toPointF();
	double theta = data.value(1).toDouble();
	return Tools::RPoint(p, theta);
}

QRectF NSParser::DeclaredVariable::toRect() const
{
	return data.value(0).toRectF();
}

int NSParser::DeclaredVariable::toAx12() const
{
	return data.value(0, -1).toInt();
}

int NSParser::DeclaredVariable::toParameter() const
{
	return data.value(0, -1).toInt();
}

void NSParser::DeclaredVariable::toSensor(int& id, int& type) const
{
	id = data.value(0, -1).toInt();
	type = data.value(1, -1).toInt();
}

void NSParser::DeclaredVariable::toAction(int& id, int& param, int& timeMs) const
{
	id = data.value(0, -1).toInt();
	param = data.value(1, -1).toInt();
	timeMs = data.value(2, -1).toInt();
}

QString NSParser::DeclaredVariable::toString() const
{
	return data.value(0).toString();
}
