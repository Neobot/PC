#include "ScriptView.h"
#include "ui_ScriptView.h"

#include "CGTFile.h"

#include <QFileInfo>

ScriptView::ScriptView(NetworkConnection *connection, QWidget *parent) :
	AbstractView(connection, QImage(), parent),
	ui(new Ui::ScriptView)
{
	ui->setupUi(this);
	connect(ui->btnCheck, SIGNAL(clicked()), this, SLOT(check()));
}

ScriptView::~ScriptView()
{
	delete ui;
}

void ScriptView::connectionStatusChanged(NetworkConnection::ConnectionStatus status)
{

}

void ScriptView::check()
{
	CGTFile    cgtFile;
	Symbol     *rdc;
	DFA        *dfa;
	LALR       *lalr;

	ErrorTable       *myErrors;

	QFileInfo f("/Users/trabillard/Documents/Programmation/Project/NeoGit/PC/Qt/Libraries/WidgetToolsLib/Resources/nra.cgt");
	QString aa = f.absoluteFilePath();

	// Load grammar file
	if (cgtFile.load(aa.toLatin1().data()))
	{
		wprintf (L"%s\n", "Grammar loaded succesfully");
		cgtFile.printInfo();
	}
	else
	{
		wprintf (L"%s\n", "error loading file");
	}

	// Get DFA (Deterministic Finite Automata) from the cgt file
	dfa = cgtFile.getScanner();

	// Scan the source in search of tokens
	dfa->scan(ui->plainTextEdit->toPlainText().toLatin1().data());

	// Get the error table
	myErrors = dfa->getErrors();

	// If there are errors report them
	if (myErrors->errors.size() > 0)
	{
		for (unsigned int i=0; i < myErrors->errors.size(); i++)
		{
			qDebug("Error 1");
			//cout << myReporter.composeErrorMsg (*myErrors->errors[i]) << endl;
		}
	}

	// Get the tokens to feed the LALR machine with them
	vector <Token*> tokens = dfa->getTokens();
	//printTokens(tokens);

	// Get the LALR (Look Ahead, Left-to-right parse, Rightmost-derivation)
	lalr = cgtFile.getParser();

	// Parse the tokens
	rdc = lalr->parse (tokens, true, true);

	myErrors = lalr->getErrors();
	if (myErrors->errors.size() != 0) {
		for (unsigned int i=0; i < myErrors->errors.size(); i++)
		{
			qDebug("Error 2");
			//cout << filename << ":";
			//cout << myReporter.composeErrorMsg (*myErrors->errors[i]) << endl;
		}
	}

	lalr->printReductionTree(rdc, 0);

	qDebug("Done");

	delete rdc;
}
