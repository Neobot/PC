#include "NSParsingError.h"
#include "ErrorTable.h"

NSParsingError NSParsingError::warning(const QString& warningMessage, Symbol *symbol)
{
	return NSParsingError(Warning, QString(),  symbol->line, symbol->col, warningMessage);
}

NSParsingError NSParsingError::error(const QString& errorMessage, Symbol *symbol)
{
	return NSParsingError(Error, QString(),  symbol->line, symbol->col, errorMessage);
}

NSParsingError NSParsingError::info(const QString& infoMessage, Symbol *symbol)
{
	return NSParsingError(Info, QString(), symbol->line, symbol->col, infoMessage);
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

	return NSParsingError(Error, QString(), e->line, e->col, message);
}

NSParsingError NSParsingError::undeclaredVariableError(const QString &variableName, Symbol *symbol)
{
	return error(QString("The variable '").append(variableName).append("' is not declared."), symbol);
}

NSParsingError NSParsingError::invalidVariableTypeError(const QString &variableName, const QString& expectedType, Symbol *symbol)
{
	return error(QString("The variable '").append(variableName).append("' is not a ").append(expectedType).append("."), symbol);
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

void NSParsingError::setFilename(const QString &filename)
{
	_filename = filename;
}
