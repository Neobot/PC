#ifndef NSPARSER_H
#define NSPARSER_H

#include <QStringList>

#include "CGTFile.h"

class Symbol;
class GPError;

class AbstractAction;

class NSParser
{
public:
	NSParser();

	bool parse(const QString& scriptCode, QStringList& messages, QList<AbstractAction*>& actions);
	bool parseFile(const QString& filepath, QStringList& messages, QList<AbstractAction*>& actions);

	bool verify(const QString& scriptCode, QStringList& messages);
	bool verifyFile(const QString& filepath, QStringList& messages);

protected:
	Symbol* getParsedTree(const QString& scriptCode, QStringList& messages);
	QString composeErrorMsg(GPError* err) const;

private:
	CGTFile _cgtFile;
	bool _invalidGrammar;
};

#endif // NSPARSER_H
