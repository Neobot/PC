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

	NSParsingError(ErrorType errorType, const QString& errorFilename, int errorLine, int errorColumn, int errorLength, const QString& errorMessage)
		: _type(errorType), _filename(errorFilename), _line(errorLine), _column(errorColumn), _length(errorLength), _message(errorMessage) {}

	static NSParsingError warning(const QString& warningMessage, Symbol* symbol);
	static NSParsingError error(const QString& errorMessage, Symbol* symbol);
	static NSParsingError info(const QString& infoMessage, Symbol* symbol);
	static NSParsingError fromGPError(GPError* e);

	static NSParsingError undeclaredVariableError(const QString& variableName, Symbol* symbol);
	static NSParsingError invalidVariableTypeError(const QString& variableName, const QString &expectedType, Symbol* symbol);
	static NSParsingError invalidSensorValueError(const QString& value, Symbol* symbol);
	static NSParsingError functionAlreadyDefinedError(const QString& functionName, Symbol* symbol);
	static NSParsingError invalidNumberOfArguments(const QString& functionName, int nbExpectedArgs, int nbFoundArgs, Symbol* symbol);
	static NSParsingError undeclaredFunctionError(const QString& variableName, Symbol* symbol);

	QString print() const;

	ErrorType getType() const;
	const QString& getFilename() const;
	int getLine() const;
	int getColumn() const;
	int getLength() const;
	const QString& getMessage() const;

	void setFilename(const QString& filename);

private:
	ErrorType _type;
	QString _filename;
	int _line;
	int _column;
	int _length;
	QString _message;

    static int findSymbolLength(Symbol* symbol);
};

#endif // NSPARSINGERROR_H
