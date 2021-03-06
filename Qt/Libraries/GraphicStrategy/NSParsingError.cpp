#include "NSParsingError.h"
#include "ErrorTable.h"
#include "NonTerminal.h"
#include "Terminal.h"

NSParsingError::NSParsingError(NSParsingError::ErrorType errorType, const QString &errorFilename, Symbol *symbol, const QString &errorMessage)
	: NSParsingError(errorType, errorFilename, 0, 0, 0, errorMessage)
{
	setSymbol(symbol);
}

NSParsingError NSParsingError::warning(const QString& warningMessage, Symbol *symbol)
{
	return NSParsingError(Warning, QString(), symbol, warningMessage);
}

NSParsingError NSParsingError::error(const QString& errorMessage, Symbol *symbol)
{
	return NSParsingError(Error, QString(), symbol, errorMessage);
}

NSParsingError NSParsingError::info(const QString& infoMessage, Symbol *symbol)
{
	return NSParsingError(Info, QString(), symbol, infoMessage);
}

NSParsingError NSParsingError::fromGPError(GPError* e)
{
	QString message;
	switch(e->value)
	{
		case UNKNOWN_TOKEN: message += "Unknown token"; break;
		case UNEXPECTED_TOKEN: message += "Unexpected token"; break;
		case END_COMMENT_NOT_FOUND: message += "End of comment not found"; break;
		default: message += "Unknown error"; break;
	}

	message += " '";
	message += QString::fromStdWString(e->lastTerminal.image);
	message += "'";

	return NSParsingError(Error, QString(), e->line, e->col - 1, findSymbolLength(&e->lastTerminal), message);
}

NSParsingError NSParsingError::undeclaredVariableError(const QString &variableName, Symbol *symbol)
{
	return error(QString("The variable '").append(variableName).append("' is not declared."), symbol);
}

NSParsingError NSParsingError::invalidVariableTypeError(const QString &variableName, const QString& expectedType, Symbol *symbol)
{
	return error(QString("The variable '").append(variableName).append("' is not a ").append(expectedType).append("."), symbol);
}

NSParsingError NSParsingError::invalidSensorValueError(const QString &value, Symbol *symbol)
{
	return error(QString("'").append(value).append("'' is not a valid value for this type of sensor."), symbol);
}

NSParsingError NSParsingError::invalidNumberOfArguments(const QString &functionName, int nbExpectedArgs, int nbFoundArgs, Symbol *symbol)
{
	return error(QString("Function '").append(functionName).append("' wrong number of arguments: expected '")
				 .append(QString::number(nbExpectedArgs)).append(", found ").append(QString::number(nbFoundArgs)), symbol);
}

NSParsingError NSParsingError::undeclaredFunctionError(const QString &functionName, Symbol *symbol)
{
    return error(QString("The function '").append(functionName).append("' is not declared."), symbol);
}

NSParsingError NSParsingError::loadingFileError(const QString &fileName, Symbol *symbol)
{
     return error(QString("An error occured while loading '").append(fileName).append("'."), symbol);
}

NSParsingError NSParsingError::invalidSensorId(Symbol *symbol)
{
	return error(QString("Sensor id should start at 1"), symbol);
}

NSParsingError NSParsingError::functionAlreadyDefinedError(const QString &functionName, Symbol *symbol)
{
	return error(QString("The function '").append(functionName).append("'' is already defined."), symbol);
}

QString NSParsingError::print() const
{
	QString text;
	switch(_type)
	{
		case Info: text += "Info: "; break;
		case Warning: text += "Warning: "; break;
		case Error: text += "Error: "; break;
	}

	if (!_filename.isEmpty())
	{
		text += _filename;
		text += ": ";
	}

	if (_line >= 0)
	{
		text += "Line ";
		text += QString::number(_line);

		if (_column >= 0)
		{
			text += ", Column ";
			text += QString::number(_column);
		}
		text += ": ";
	}

	text += _message;

	return text;
}

NSParsingError::ErrorType NSParsingError::getType() const
{
	return _type;
}

const QString& NSParsingError::getFilename() const
{
	return _filename;
}

int NSParsingError::getLine() const
{
	return _line;
}

int NSParsingError::getColumn() const
{
	return _column;
}

int NSParsingError::getLength() const
{
	return _length;
}

const QString &NSParsingError::getMessage() const
{
	return _message;
}

void NSParsingError::setSymbol(Symbol *symbol)
{
	_line = symbol->line;
	_column = symbol->col - 1;
	_length = findSymbolLength(symbol);
}

void NSParsingError::setFilename(const QString &filename)
{
    _filename = filename;
}

int NSParsingError::findSymbolLength(Symbol *symbol)
{
    int length = -1;
    if (symbol->type == TERMINAL)
    {
        Terminal* t = static_cast<Terminal*>(symbol);
        length = t->image.length();
    }
    else
    {
        int originalCol = symbol->col;
        int originalLine = symbol->line;
        while (true)
        {
            //search the last child-terminal of the symbol
            NonTerminal* nt = static_cast<NonTerminal*>(symbol);
            if (!nt->children.empty())
            {
				symbol = *(nt->children.end() - 1);
                if (symbol->type == TERMINAL)
                {
                    //last terminal found
                    Terminal* t = static_cast<Terminal*>(symbol);
                    if (t->line == originalLine) //if the last terminal is on the same line
                        length = t->col + t->image.length() - originalCol;

                    break;
                }
            }
            else
                break;

        }
    }

    return length;
}
