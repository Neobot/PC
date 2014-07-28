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

bool NSParser::parse(const QString &scriptCode, QStringList &messages, QList<AbstractAction *> &actions)
{
	if (_tree)
		delete _tree;
		
	_tree = getParsedTree(scriptCode, messages);

	if (_tree)
	{
		VariableList variables;
		buildActions(_tree, actions, variables);
	}
	
	return _tree;
}

bool NSParser::parseFile(const QString &filepath, QStringList &messages, QList<AbstractAction *> &actions)
{
	QFile file(filepath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		messages << QString("The file \"").append(filepath).append("\" cannot be opened.");
		return false;
	}

	return	parse(filepath, messages, actions);
}

bool NSParser::verify(const QString &scriptCode, QStringList &messages)
{
	QList<AbstractAction *> actions;
	return parse(scriptCode, messages, actions);
}

bool NSParser::verifyFile(const QString &filepath, QStringList &messages)
{
	QList<AbstractAction *> actions;
	return parseFile(filepath, messages, actions);
}

void NSParser::print(QTextStream& out)
{
	if (_tree)
		printTree(out, _tree, 0);
}

Symbol *NSParser::getParsedTree(const QString &scriptCode, QStringList &messages)
{
	if (_invalidGrammar)
	{
		messages << "Critical: An error occured while loading the grammar!";
		return nullptr;
	}

	// Get DFA (Deterministic Finite Automata) from the cgt file
	DFA* dfa = _cgtFile.getScanner();

	// Scan the source in search of tokens
	dfa->scan(scriptCode.toLatin1().data());

	//Report the dfa errors
	for (GPError* e: dfa->getErrors()->errors)
	{
		messages << composeErrorMsg(e);
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
		messages << composeErrorMsg(e);
	}

	return rdc;
}

QString NSParser::composeErrorMsg(GPError *err) const
{
	QString message("Error: Line ");
	message += QString::number(err->lastTerminal.line);
	message += ", Col ";
	message += QString::number(err->lastTerminal.col);

	if (err->expected.size() > 0)
	{
		message += ". Expected one of the following tokens: ";

		for(const std::wstring& exp : err->expected)
		{
			message += QString::fromStdWString(exp);
			message += " ";
		}
	}

	return message;
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

		//if (!ok)
			//error, invalid point var

		if (_factory)
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

		//if (!ok)
			//error, invalid point var

		bool forceForward = dir == Tools::Forward;
		bool forceBackward = dir == Tools::Backward;

		if (_factory)
			return _factory->moveAction(point.toQPointF(), speed, forceForward, forceBackward);
	}

	return nullptr;
}

AbstractAction *NSParser::buildSetParameterAction(Symbol *symbol, NSParser::VariableList &variables)
{
	if (symbol->type == NON_TERMINAL)
	{
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
					paramId = readParameterOrVar(child, variables);
					break;
				case SYM_NUM:
					paramValue = readNum(child);
					break;
			}
		}

		//if (paramId < 0)
			//error

		if (_factory)
			return _factory->setParameterAction(paramId, paramValue);
	}

	return nullptr;
}

AbstractAction *NSParser::buildEnableSensorAction(Symbol *symbol, NSParser::VariableList &variables)
{
	AbstractAction* action = nullptr;
	if (symbol->type == NON_TERMINAL)
	{
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
					readSensorOrVar(child, variables, sensorType, sensorId);
					break;
				case SYM_SENSOR2:
					sensorType = readSensorType(child);
					break;
			}
		}

		//if (sensorType < 0)
			//error

		//if (!all && sensorId <= 0)
			//error

		if (all)
			sensorId = 0;

		if (_factory)
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
					readSensorOrVar(child, variables, sensorType, sensorId);
					break;
				case SYM_SENSOR2:
					sensorType = readSensorType(child);
					break;
			}
		}

		//if (sensorType < 0)
			//error

		//if (!all && sensorId <= 0)
			//error

		if (all)
			sensorId = 0;

		if (_factory)
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

double NSParser::readFloat(Symbol* symbol)
{
	double value = 0.0;
	if (symbol->type == TERMINAL && symbol->symbolIndex == SYM_FLOAT)
		value = QString::fromStdWString(static_cast<Terminal*>(symbol)->image).toDouble();
	
	return value;
}

QString NSParser::readIdentifier(Symbol* symbol)
{
	QString value = 0;
	if (symbol->type == TERMINAL && symbol->symbolIndex == SYM_IDENTIFIER)
		value = QString::fromStdWString(static_cast<Terminal*>(symbol)->image);
	
	return value;
}

QString NSParser::readString(Symbol* symbol)
{
	QString value = 0;
	if (symbol->type == TERMINAL && symbol->symbolIndex == SYM_STRING)
	{
		value = QString::fromStdWString(static_cast<Terminal*>(symbol)->image);
		if (!value.startsWith("\""))
			value.remove(0, 1);
		if (!value.endsWith("\""))
			value.remove(value.count() - 1, 1);
	}
	
	return value;
}

int NSParser::readInteger(Symbol* symbol)
{
	int value = 0;
	if (symbol->type == TERMINAL && symbol->symbolIndex == SYM_INTEGER)
		value = QString::fromStdWString(static_cast<Terminal*>(symbol)->image).toInt();
	
	return value;
}

double NSParser::readNum(Symbol* symbol)
{
	double num = 0.0;
	if (symbol->type == NON_TERMINAL)
	{	
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
		{
			switch(child->symbolIndex)	
			{
				case SYM_INTEGER:
					num = readInteger(child);
					break;
				case SYM_FLOAT:
					num = readFloat(child);
					break;
			}
		}
	}
	
	return num;
}

void NSParser::readSensorIdentifier(Symbol *symbol, int& sensorType, int &id)
{
	if (symbol->type == NON_TERMINAL)
	{
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
		{
			switch(child->symbolIndex)
			{
				case SYM_INTEGER:
				case SYM_ID:
				{
					Symbol* intSymbol = searchChild(child, SYM_INTEGER);
					id = readInteger(intSymbol);
					break;
				}
				case SYM_SENSOR2:
				{
					sensorType = readSensorType(child);
					break;
				}
			}
		}
	}
}

void NSParser::readSensorOrVar(Symbol *symbol, NSParser::VariableList &variables, int &sensorType, int &id)
{
	if (symbol->type == NON_TERMINAL)
	{
		if (symbol->symbolIndex == SYM_SENSOR_IDENTIFIER)
			readSensorIdentifier(symbol, sensorType, id);
		else if (symbol->symbolIndex == SYM_VAR)
		{
			variables[readVar(symbol)].toSensor(id, sensorType);
		}
		else //SYM_PARAMETER_OR_VAR
		{
			NonTerminal* nt = static_cast<NonTerminal*>(symbol);
			switch(nt->ruleIndex)
			{
				case PROD_PARAMETER_OR_VAR:
					readSensorIdentifier(symbol, sensorType, id);
					break;
				case PROD_PARAMETER_OR_VAR2:
				{
					QString varName = readVar(symbol);
					 variables[varName].toSensor(id, sensorType);
					break;
				}
			}
		}
	}
}

int NSParser::readSensorType(Symbol *symbol)
{
	int sensorType = -1;
	NonTerminal* nt = static_cast<NonTerminal*>(symbol);
	if (nt->ruleIndex == PROD_SENSOR_COLOR_SENSOR)
		sensorType = Comm::ColorSensor;
	else if (nt->ruleIndex == PROD_SENSOR_MICROSWITCH)
		sensorType = Comm::MicroswitchSensor;
	else if (nt->ruleIndex == PROD_SENSOR_SHARP)
		sensorType = Comm::SharpSensor;
	else
		sensorType = Comm::UnknownedSensor;

	return sensorType;
}

int NSParser::readSubId(Symbol *symbol)
{
	Symbol* intSymbol = searchChild(symbol, SYM_INTEGER, true);
	if (intSymbol)
		return readInteger(intSymbol);

	return -1;
}

int NSParser::readParameterOrVar(Symbol *symbol, NSParser::VariableList &variables)
{
	int paramId = -1;
	if (symbol->type == NON_TERMINAL)
	{
		if (symbol->symbolIndex == SYM_PARAMETER_IDENTIFIER)
			paramId = readSubId(symbol);
		else if (symbol->symbolIndex == SYM_VAR)
			paramId = variables[readVar(symbol)].toParameter();
		else //SYM_PARAMETER_OR_VAR
		{
			NonTerminal* nt = static_cast<NonTerminal*>(symbol);
			switch(nt->ruleIndex)
			{
				case PROD_PARAMETER_OR_VAR:
					paramId = readSubId(symbol);
					break;
				case PROD_PARAMETER_OR_VAR2:
				{
					QString varName = readVar(symbol);
					paramId = variables[varName].toParameter();
					break;
				}
			}
		}
	}

	return paramId;
}

int NSParser::readAx12OrVar(Symbol *symbol, NSParser::VariableList &variables)
{
	int ax12Id = -1;
	if (symbol->type == NON_TERMINAL)
	{
		if (symbol->symbolIndex == SYM_AX12_IDENTIFIER)
			ax12Id = readSubId(symbol);
		else if (symbol->symbolIndex == SYM_VAR)
			ax12Id = variables[readVar(symbol)].toAx12();
		else //SYM_PARAMETER_OR_VAR
		{
			NonTerminal* nt = static_cast<NonTerminal*>(symbol);
			switch(nt->ruleIndex)
			{
				case PROD_AX12_OR_VAR:
					ax12Id = readSubId(symbol);
					break;
				case PROD_AX12_OR_VAR2:
				{
					QString varName = readVar(symbol);
					ax12Id = variables[varName].toAx12();
					break;
				}
			}
		}
	}

	return ax12Id;
}

void NSParser::readAction(Symbol *symbol, int &actionId, int &param, int &time)
{
	if (symbol->type == NON_TERMINAL)
	{
		bool idOk = false;
		bool paramOk = false;

		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
		{
			switch(child->symbolIndex)
			{
				case SYM_INTEGER:
				{
					int value = readInteger(child);
					if (!idOk)
					{
						actionId = value;
						idOk = true;
					}
					else if (!paramOk)
					{
						param = value;
						paramOk = true;
					}
					break;
				}
				case SYM_TIME:
				{
					time = readTimeInMs(child);
					break;
				}
			}
		}
	}
}

void NSParser::readActionOrVar(Symbol *symbol, NSParser::VariableList &variables, int &actionId, int &param, int &time)
{
	if (symbol->type == NON_TERMINAL)
	{
		if (symbol->symbolIndex == SYM_ACTION2)
			readAction(symbol, actionId, param, time);
		else if (symbol->symbolIndex == SYM_VAR)
		{
			variables[readVar(symbol)].toAction(actionId, param, time);
		}
		else //SYM_ACTION_OR_VAR
		{
			NonTerminal* nt = static_cast<NonTerminal*>(symbol);
			switch(nt->ruleIndex)
			{
				case PROD_ACTION_OR_VAR:
					readAction(symbol, actionId, param, time);
					break;
				case PROD_ACTION_OR_VAR2:
				{
					QString varName = readVar(symbol);
					 variables[varName].toAction(actionId, param, time);
					break;
				}
			}
		}
	}
}


double NSParser::readFixedAngleInRadian(Symbol* symbol)
{
	double value = 0.0;
	if (symbol->type == NON_TERMINAL)
	{
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		Symbol* numSymbol = searchChild(symbol, SYM_NUM);
		if (numSymbol)
			value = readNum(numSymbol);

		switch(nt->ruleIndex)
		{
			case PROD_FIXED_ANGLE_RAD:
				break;
			case PROD_FIXED_ANGLE_DEG:
			case PROD_FIXED_ANGLE:
			default:
				value = Tools::degreeToRadian(value);
				break;
		}
	}

	return value;
}

double NSParser::readAngleInRadian(Symbol* symbol)
{
	double value = 0.0;
	if (symbol->type == NON_TERMINAL)
	{
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		Symbol* fixedSymbol = searchChild(symbol, SYM_FIXED_ANGLE);
		if (fixedSymbol)
		{
			value = readFixedAngleInRadian(fixedSymbol);
			switch(nt->ruleIndex)
			{
				case PROD_ANGLE_AUTO_REVERSED:
					//do auto reverse
					break;
				case PROD_ANGLE_REVERSED:
					//do reverse
					break;
				case PROD_ANGLE:
					break;
			}
		}
	}

	return value;
}

QString NSParser::readVar(Symbol* symbol)
{
	QString varName;
	Symbol* identifier = searchChild(symbol, SYM_IDENTIFIER);
	if (identifier)
		varName = readIdentifier(identifier);

	return varName;
}

int NSParser::readSpeed(Symbol* symbol)
{
	int speed = 100;
	Symbol* intergerSymbol = searchChild(symbol, SYM_INTEGER);
	if (intergerSymbol)
		speed = readInteger(intergerSymbol);

	return speed;
}

Tools::Direction NSParser::readDirection(Symbol* symbol)
{
	Tools::Direction dir  = Tools::Unknown;
	if (symbol->type == NON_TERMINAL)
	{
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		switch(nt->ruleIndex)
		{
			case PROD_DIRECTION_FORWARD:
				dir = Tools::Forward;
				break;
			case PROD_DIRECTION_BACKWARD:
				dir = Tools::Backward;
				break;
			case PROD_DIRECTION_AUTO:
				dir = Tools::Unknown;
				break;
		}
	}

	return dir;
}



int NSParser::readTimeInMs(Symbol* symbol)
{
	int value = 0;
	int unitFactor = 1;
	if (symbol->type == NON_TERMINAL)
	{	
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
		{
			switch(child->symbolIndex)	
			{
				case SYM_INTEGER:
					value = QString::fromStdWString(static_cast<Terminal*>(child)->image).toInt();
					break;
				case SYM_TIME_UNIT:
					if (static_cast<NonTerminal*>(child)->ruleIndex == PROD_TIME_UNIT_S)
						unitFactor = 1000; //seconds
					else //child->ruleIndex == PROD_TIME_UNIT_MS
						unitFactor = 1; // milliseconds
					break;
			}
		}
	}
	
	return value * unitFactor;
}

Tools::RPoint NSParser::readFixedPoint(Symbol* symbol)
{
	Tools::RPoint point;
	bool xOk = false;
	bool yOk = false;
	if (symbol->type == NON_TERMINAL)
	{
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
		{
			switch(child->symbolIndex)
			{
				case SYM_NUM:
				case SYM_FIXED_ANGLE:
				{
					double value = readNum(child);
					if (!xOk)
					{
						point.setX(value);
						xOk = true;
					}
					else if (!yOk)
					{
						point.setY(value);
						yOk = true;
					}
					else
					{
						double angle = readFixedAngleInRadian(child);
						point.setTheta(angle);
					}
					break;
				}
			}
		}
	}
	
	return point;
}

bool NSParser::readPointOrVar(Symbol* symbol, VariableList& variables, Tools::RPoint &point)
{
	bool result = false;
	if (symbol->type == NON_TERMINAL)
	{
		result = true;
		if (symbol->symbolIndex == SYM_POINT)
			point = readPoint(symbol);
		else if (symbol->symbolIndex == SYM_FIXED_POINT)
			point = readFixedPoint(symbol);
		else if (symbol->symbolIndex == SYM_VAR)
		{
			QString varName = readVar(symbol);
			result = variables.contains(varName);
			if (result)
				point = variables[varName].toPoint();
		}
		else //SYM_POINT_OR_VAR
		{
			NonTerminal* nt = static_cast<NonTerminal*>(symbol);
			switch(nt->ruleIndex)
			{
				case PROD_POINT_OR_VAR:
					point = readPoint(searchChild(symbol, SYM_POINT));
					break;
				case PROD_POINT_OR_VAR2:
				{
					QString varName = readVar(symbol);
					result = variables.contains(varName);
					if (result)
						point = variables[varName].toPoint();
					break;
				}
			}
		}
	}

	return result;
}

Tools::RPoint NSParser::readPoint(Symbol* symbol)
{
	Tools::RPoint point;
	if (symbol->type == NON_TERMINAL)
	{
		if (symbol->symbolIndex == SYM_FIXED_POINT)
			point = readFixedPoint(symbol);
		else
		{
			NonTerminal* nt = static_cast<NonTerminal*>(symbol);
			Symbol* fixedPointSymbol = searchChild(symbol, SYM_FIXED_POINT);
			if (fixedPointSymbol)
			{
				point = readFixedPoint(fixedPointSymbol);
				switch(nt->ruleIndex)
				{
					case PROD_POINT_AUTO_REVERSED:
						//do auto reverse
						break;
					case PROD_POINT_REVERSED:
						//do reverse
						break;
					case PROD_POINT:
						break;
				}
			}
		}
	}

	return point;
}

QRectF NSParser::readRect(Symbol* symbol)
{
	QRectF r;
	if (symbol->type == NON_TERMINAL)
	{
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		Symbol* fixedSymbol = searchChild(symbol, SYM_FIXED_RECT);
		if (fixedSymbol)
		{
			r = readFixedRect(fixedSymbol);
			switch(nt->ruleIndex)
			{
				case PROD_RECT_AUTO_REVERSED:
					//do auto reverse
					break;
				case PROD_RECT_REVERSED:
					//do reverse
					break;
				case PROD_RECT:
					break;
			}
		}
	}

	return r;
}



QRectF NSParser::readFixedRect(Symbol* symbol)
{
	QRectF r(0,0,1,1);
	int nbParamRead = 0;
	if (symbol->type == NON_TERMINAL)
	{
		NonTerminal* nt = static_cast<NonTerminal*>(symbol);
		for(Symbol* child: nt->children)
		{
			switch(child->symbolIndex)
			{
				case SYM_NUM:
				{
					double value = readNum(child);
					if (nbParamRead == 0)
						r.setX(value);
					else if (nbParamRead == 1)
						r.setY(value);
					else if (nbParamRead == 2)
						r.setWidth(value);
					else
						r.setHeight(value);
					++nbParamRead;
					break;
				}
			}
		}
	}
	
	return r;
}

bool NSParser::NSParser::readRectOrVar(Symbol *symbol, NSParser::VariableList &variables, QRectF& r)
{
	bool result = false;
	if (symbol->type == NON_TERMINAL)
	{
		result = true;
		if (symbol->symbolIndex == SYM_RECT)
			r = readRect(symbol);
		else if (symbol->symbolIndex == SYM_FIXED_RECT)
			r = readFixedRect(symbol);
		else if (symbol->symbolIndex == SYM_VAR)
		{
			QString varName = readVar(symbol);
			result = variables.contains(varName);
			if (result)
				r = variables[varName].toRect();
		}
		else //SYM_RECT_OR_VAR
		{
			NonTerminal* nt = static_cast<NonTerminal*>(symbol);
			switch(nt->ruleIndex)
			{
				case PROD_RECT_OR_VAR:
					r = readRect(searchChild(symbol, SYM_RECT));
					break;
				case PROD_RECT_OR_VAR2:
				{
					QString varName = readVar(symbol);
					result = variables.contains(varName);
					if (result)
						r = variables[varName].toRect();
					break;
				}
			}
		}
	}

	return result;
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
