#ifndef NSPARSINGERROR_H
#define NSPARSINGERROR_H

#include <QString>

class GPError;
class Symbol;

class NSParsingError
{
public:

	enum ErrorType
	{
		Info,
		Warning,
		Error
	};

	NSParsingError(ErrorType errorType, const QString& errorFilename, int errorLine, int errorColumn, const QString& errorMessage)
		: _type(errorType), _filename(errorFilename), _line(errorLine), _column(errorColumn), _message(errorMessage) {}

	static NSParsingError warning(const QString& warningMessage, Symbol* symbol);
	static NSParsingError error(const QString& errorMessage, Symbol* symbol);
	static NSParsingError info(const QString& infoMessage, Symbol* symbol);
	static NSParsingError fromGPError(GPError* e);

	static NSParsingError undeclaredVariableError(const QString& variableName, Symbol* symbol);
	static NSParsingError invalidVariableTypeError(const QString& variableName, const QString &expectedType, Symbol* symbol);

	QString print() const;

	ErrorType getType() const;
	const QString& getFilename() const;
	int getLine() const;
	int getColumn() const;
	const QString& getMessage() const;

	void setFilename(const QString& filename);

private:
	ErrorType _type;
	QString _filename;
	int _line;
	int _column;
	QString _message;
};

#endif // NSPARSINGERROR_H
