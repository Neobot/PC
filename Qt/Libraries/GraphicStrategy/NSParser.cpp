#include "NSParser.h"

#include <QFile>

#include <sstream>

NSParser::NSParser() : _invalidGrammar(false)
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


bool NSParser::parse(const QString &scriptCode, QStringList &messages, QList<AbstractAction *> &actions)
{
	Symbol* s = getParsedTree(scriptCode, messages);
	bool ok = s != nullptr;

	delete s;

	return ok;
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


QString NSParser::composeErrorMsg (GPError *err) const
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
		}
	}
//	// Default error
//	header += atoi ();
//	header += ":";
//	header += atoi (err->lastTerminal.col);
//	msg = " unexpected token ";
//	//   msg += err.lastTerminal.image;

	//return header + msg;

	return message;
}


