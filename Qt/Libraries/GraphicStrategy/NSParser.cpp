#include "NSParser.h"
#include "ActionFactory.h"
#include "ns.h"
#include "ToolsLib.h"

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
	delete _tree;
}

void NSParser::setActionFactory(ActionFactory* factory)
{
	_factory = factory;
}

void NSParser::addError(const NSParsingError &error)
{
	NSParsingError errorCopy(error);
	errorCopy.setFilename(_currentFile);
	_errors << errorCopy;
}

const QList<NSParsingError> &NSParser::getErrors() const
{
	return _errors;
}

bool NSParser::parse(const QString &scriptCode, QList<AbstractAction *> &actions, const QString &originalFilename)
{
	_errors.clear();
	_currentFile = QFileInfo(originalFilename).fileName();

	if (_tree)
		delete _tree;

	_tree = getParsedTree(scriptCode);

	if (_tree)
	{
		VariableList variables;
		buildActions(_tree, actions, variables);
	}

	return _errors.isEmpty();
}

bool NSParser::parse(const QString &scriptCode, QList<AbstractAction *> &actions)
{
	return parse(scriptCode, actions, QString());
}

bool NSParser::parseFile(const QString &filepath, QList<AbstractAction *> &actions)
{
	QFile file(filepath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		_errors << NSParsingError(NSParsingError::Error, filepath, -1, -1, -1, "The file cannot be opened");
		return false;
	}

	return	parse(filepath, actions, filepath);
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

void NSParser::buildActions(Symbol* symbol, QList<AbstractAction*>& actions, VariableList& variables)
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
		default:
		{
			if (symbol->type == NON_TERMINAL)
			{
				//Build sub-actions
				NonTerminal* nt = static_cast<NonTerminal*>(symbol);
				for(Symbol* child: nt->children)
					buildActions(child, actions, variables);
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
		DelclaredVariable var;
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
		{
			switch(child->symbolIndex)	
			{
				case SYM_POINT: 
				case SYM_FIXED_POINT:
				{
					Tools::RPoint p = readPoint(child);
					var = DelclaredVariable::fromPoint(p);
					break;
				}
				case SYM_RECT:
				case SYM_FIXED_RECT:
				{
					QRectF r = readRect(child);
					var = DelclaredVariable::fromRect(r);
					break;
				}
				case SYM_SENSOR_IDENTIFIER:
				{
					int type = -1, id = 0;
					readSensorIdentifier(child, type, id);
					var = DelclaredVariable::fromSensor(id, type);
					break;
				}
				case SYM_PARAMETER_IDENTIFIER:
				{
					int paramId = readSubId(child);
					var = DelclaredVariable::fromParameter(paramId);
					break;
				}
				case SYM_AX12_IDENTIFIER:
				{
					int ax12Id = readSubId(child);
					var = DelclaredVariable::fromAx12(ax12Id);
					break;
				}
				case SYM_ACTION2:
				{
					int actionId, param, time;
					readAction(child, actionId, param, time);
					var = DelclaredVariable::fromAction(actionId, param, time);
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

//-----------------------------------------------------------------

NSParser::DelclaredVariable NSParser::DelclaredVariable::fromPoint(const Tools::RPoint& p)
{
	DelclaredVariable var;
	var.type = Point;
	var.data << QVariant(QPointF(p.getX(), p.getY()))
			 << QVariant(p.getTheta());

	return var;
}

NSParser::DelclaredVariable NSParser::DelclaredVariable::fromRect(const QRectF& rect)
{
	DelclaredVariable var;
	var.type = Rect;
	var.data << QVariant(rect);

	return var;
}

NSParser::DelclaredVariable NSParser::DelclaredVariable::fromAction(int id, int param, int timeMs)
{
	DelclaredVariable var;
	var.type = Action;
	var.data << QVariant(id)
			 << QVariant(param)
			 << QVariant(timeMs);

	return var;
}

NSParser::DelclaredVariable NSParser::DelclaredVariable::fromAx12(int id)
{
	DelclaredVariable var;
	var.type = Ax12;
	var.data << QVariant(id);

	return var;
}

NSParser::DelclaredVariable NSParser::DelclaredVariable::fromParameter(int id)
{
	DelclaredVariable var;
	var.type = Param;
	var.data << QVariant(id);

	return var;
}

NSParser::DelclaredVariable NSParser::DelclaredVariable::fromSensor(int id, int type)
{
	DelclaredVariable var;
	var.type = Sensor;
	var.data << QVariant(id)
			 << QVariant(type);

	return var;
}

Tools::RPoint NSParser::DelclaredVariable::toPoint() const
{
	QPointF p = data.value(0).toPointF();
	double theta = data.value(1).toDouble();
	return Tools::RPoint(p, theta);
}

QRectF NSParser::DelclaredVariable::toRect() const
{
	return data.value(0).toRectF();
}

int NSParser::DelclaredVariable::toAx12() const
{
	return data.value(0, -1).toInt();
}

int NSParser::DelclaredVariable::toParameter() const
{
	return data.value(0, -1).toInt();
}

void NSParser::DelclaredVariable::toSensor(int& id, int& type) const
{
	id = data.value(0, -1).toInt();
	type = data.value(1, -1).toInt();
}

void NSParser::DelclaredVariable::toAction(int& id, int& param, int& timeMs)
{
	id = data.value(0, -1).toInt();
	param = data.value(1, -1).toInt();
	timeMs = data.value(2, -1).toInt();
}
