#include "NSParser.h"
#include "ActionFactory.h"
#include "ns.h"
#include "ToolsLib.h"



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
	QString value = "";
	if (symbol->type == TERMINAL && symbol->symbolIndex == SYM_STRING)
	{
		value = QString::fromStdWString(static_cast<Terminal*>(symbol)->image);
		if (value.startsWith("\""))
			value.remove(0, 1);
		if (value.endsWith("\""))
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

bool NSParser::readSensorOrVar(Symbol *symbol, NSParser::VariableList &variables, int &sensorType, int &id)
{
	bool result = false;
	if (symbol->type == NON_TERMINAL)
	{
		result = true;
		if (symbol->symbolIndex == SYM_SENSOR_IDENTIFIER)
			readSensorIdentifier(symbol, sensorType, id);
		else if (symbol->symbolIndex == SYM_VAR)
			result = readSensorVar(symbol, variables, sensorType, id);
		else //SYM_PARAMETER_OR_VAR
		{
			NonTerminal* nt = static_cast<NonTerminal*>(symbol);
			switch(nt->ruleIndex)
			{
				case PROD_PARAMETER_OR_VAR:
					readSensorIdentifier(symbol, sensorType, id);
					break;
				case PROD_PARAMETER_OR_VAR2:
					result = readSensorVar(symbol, variables, sensorType, id);
					break;
				default:
					result = false;
					break;
			}
		}
	}

	return result;
}

bool NSParser::readSensorVar(Symbol *symbol, NSParser::VariableList &variables, int &sensorType, int &id)
{
	bool result = false;
	QString varName = readVar(symbol);
	if (variables.contains(varName))
	{
		DelclaredVariable& var = variables[varName];
		if (var.isSensor())
		{
			var.toSensor(id, sensorType);
			result = true;
		}
		else
			addError(NSParsingError::invalidVariableTypeError(varName, "sensor", symbol));
	}
	else
		addError(NSParsingError::undeclaredVariableError(varName, symbol));

	return result;
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

bool NSParser::readParameterOrVar(Symbol *symbol, NSParser::VariableList &variables, int& paramId)
{
	bool result = false;
	if (symbol->type == NON_TERMINAL)
	{
		result = true;
		if (symbol->symbolIndex == SYM_PARAMETER_IDENTIFIER)
			paramId = readSubId(symbol);
		else if (symbol->symbolIndex == SYM_VAR)
			result = readParameterVar(symbol, variables, paramId);
		else //SYM_PARAMETER_OR_VAR
		{
			NonTerminal* nt = static_cast<NonTerminal*>(symbol);
			switch(nt->ruleIndex)
			{
				case PROD_PARAMETER_OR_VAR:
					paramId = readSubId(symbol);
					break;
				case PROD_PARAMETER_OR_VAR2:
					result = readParameterVar(symbol, variables, paramId);
					break;
				default:
					result = false;
					break;
			}
		}
	}

	return result;
}

bool NSParser::readParameterVar(Symbol *symbol, NSParser::VariableList &variables, int &paramId)
{
	bool result = false;
	QString varName = readVar(symbol);
	if (variables.contains(varName))
	{
		DelclaredVariable& var = variables[varName];
		if (var.isParameter())
		{
			paramId = var.toParameter();
			result = true;
		}
		else
			addError(NSParsingError::invalidVariableTypeError(varName, "parameter", symbol));
	}
	else
		addError(NSParsingError::undeclaredVariableError(varName, symbol));

	return result;
}

bool NSParser::readAx12OrVar(Symbol *symbol, NSParser::VariableList &variables, int& ax12Id)
{
	bool result = false;
	if (symbol->type == NON_TERMINAL)
	{
		result = true;
		if (symbol->symbolIndex == SYM_AX12_IDENTIFIER)
			ax12Id = readSubId(symbol);
		else if (symbol->symbolIndex == SYM_VAR)
			result = readAx12Var(symbol, variables, ax12Id);
		else //SYM_AX12_OR_VAR
		{
			NonTerminal* nt = static_cast<NonTerminal*>(symbol);
			switch(nt->ruleIndex)
			{
				case PROD_AX12_OR_VAR:
					ax12Id = readSubId(symbol);
					break;
				case PROD_AX12_OR_VAR2:
					result = readAx12Var(symbol, variables, ax12Id);
					break;
				default:
					result = false;
					break;
			}
		}
	}

	return result;
}

bool NSParser::readAx12Var(Symbol *symbol, NSParser::VariableList &variables, int &ax12Id)
{
	bool result = false;
	QString varName = readVar(symbol);
	if (variables.contains(varName))
	{
		DelclaredVariable& var = variables[varName];
		if (var.isAx12())
		{
			ax12Id = var.toAx12();
			result = true;
		}
		else
			addError(NSParsingError::invalidVariableTypeError(varName, "ax12", symbol));
	}
	else
		addError(NSParsingError::undeclaredVariableError(varName, symbol));

	return result;
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

bool NSParser::readActionOrVar(Symbol *symbol, NSParser::VariableList &variables, int &actionId, int &param, int &time)
{
	bool result = false;
	if (symbol->type == NON_TERMINAL)
	{
		result = true;
		if (symbol->symbolIndex == SYM_ACTION2)
			readAction(symbol, actionId, param, time);
		else if (symbol->symbolIndex == SYM_VAR)
			result = readActionVar(symbol, variables, actionId, param, time);
		else //SYM_ACTION_OR_VAR
		{
			NonTerminal* nt = static_cast<NonTerminal*>(symbol);
			switch(nt->ruleIndex)
			{
				case PROD_ACTION_OR_VAR:
					readAction(symbol, actionId, param, time);
					break;
				case PROD_ACTION_OR_VAR2:
					result = readActionVar(symbol, variables, actionId, param, time);
					break;
				default:
					result = false;
					break;
			}
		}
	}

	return result;
}

bool NSParser::readActionVar(Symbol *symbol, NSParser::VariableList &variables, int &actionId, int &param, int &time)
{
	bool result = false;
	QString varName = readVar(symbol);
	if (variables.contains(varName))
	{
		DelclaredVariable& var = variables[varName];
		if (var.isAction())
		{
			var.toAction(actionId, param, time);
			result = true;
		}
		else
			addError(NSParsingError::invalidVariableTypeError(varName, "action", symbol));
	}
	else
		addError(NSParsingError::undeclaredVariableError(varName, symbol));

	return result;
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
			result = readPointVar(symbol, variables, point);
		else //SYM_POINT_OR_VAR
		{
			NonTerminal* nt = static_cast<NonTerminal*>(symbol);
			switch(nt->ruleIndex)
			{
				case PROD_POINT_OR_VAR:
					point = readPoint(searchChild(symbol, SYM_POINT));
					break;
				case PROD_POINT_OR_VAR2:
					result = readPointVar(symbol, variables, point);
					break;
				default:
					result = false;
					break;
			}
		}
	}

	return result;
}

bool NSParser::readPointVar(Symbol* symbol, VariableList& variables, Tools::RPoint &point)
{
	bool result = false;
	QString varName = readVar(symbol);
	if (variables.contains(varName))
	{
		DelclaredVariable& var = variables[varName];
		if (var.isPoint())
		{
			point = var.toPoint();
			result = true;
		}
		else
			addError(NSParsingError::invalidVariableTypeError(varName, "point", symbol));
	}
	else
		addError(NSParsingError::undeclaredVariableError(varName, symbol));

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
			result = readRectVar(symbol, variables, r);
		else //SYM_RECT_OR_VAR
		{
			NonTerminal* nt = static_cast<NonTerminal*>(symbol);
			switch(nt->ruleIndex)
			{
				case PROD_RECT_OR_VAR:
					r = readRect(searchChild(symbol, SYM_RECT));
					break;
				case PROD_RECT_OR_VAR2:
					result = readRectVar(symbol, variables, r);
					break;
				default:
					result = false;
					break;
			}
		}
	}

	return result;
}

bool NSParser::readRectVar(Symbol* symbol, VariableList &variables, QRectF &r)
{
	bool result = false;
	QString varName = readVar(symbol);
	if (variables.contains(varName))
	{
		DelclaredVariable& var = variables[varName];
		if (var.isRect())
		{
			r = var.toRect();
			result = true;
		}
		else
			addError(NSParsingError::invalidVariableTypeError(varName, "rect", symbol));
	}
	else
		addError(NSParsingError::undeclaredVariableError(varName, symbol));

	return result;
}

bool NSParser::readStringOrVar(Symbol *symbol, NSParser::VariableList &variables, QString &str)
{
	bool result = true;
	if (symbol->symbolIndex == SYM_STRING)
		str = readString(symbol);
	else if (symbol->symbolIndex == SYM_VAR)
		result = readStringVar(symbol, variables, str);
	else //SYM_STRING_OR_VAR
	{
		if (symbol->type == NON_TERMINAL)
		{
			NonTerminal* nt = static_cast<NonTerminal*>(symbol);
			switch(nt->ruleIndex)
			{
				case PROD_STRING_OR_VAR_STRING:
					str = readString(searchChild(symbol, SYM_STRING));
					break;
				case PROD_STRING_OR_VAR:
					result = readStringVar(symbol, variables, str);
					break;
				default:
					result = false;
					break;
			}
		}
		else
			result = false;
	}

	return result;
}

bool NSParser::readStringVar(Symbol *symbol, NSParser::VariableList &variables, QString &str)
{
	bool result = false;
	QString varName = readVar(symbol);
	if (variables.contains(varName))
	{
		DelclaredVariable& var = variables[varName];
		if (var.isString())
		{
			str = var.toString();
			result = true;
		}
		else
			addError(NSParsingError::invalidVariableTypeError(varName, "string", symbol));
	}
	else
		addError(NSParsingError::undeclaredVariableError(varName, symbol));

	return result;
}
