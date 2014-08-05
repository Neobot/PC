#include "ScriptView.h"
#include "ui_ScriptView.h"

#include "NSParser.h"

#include <QFileInfo>
#include <QMessageBox>


ScriptView::ScriptView(NetworkConnection *connection, QWidget *parent) :
	AbstractView(connection, QImage(), parent),
	ui(new Ui::ScriptView)
{
	ui->setupUi(this);
	connect(ui->editor, SIGNAL(check()), this, SLOT(check()));

	_codeChangedSignalDelayer = new Tools::SignalDelayer(this);
	_codeChangedSignalDelayer->setDelayedSignal(ui->editor->codeEdit(), SIGNAL(codeChanged()), 1000);
	connect(_codeChangedSignalDelayer, SIGNAL(raised()), this, SLOT(check()));
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
	if (ui->editor->codeEdit()->toPlainText().isEmpty())
		return;

	ui->editor->codeEdit()->clearErrors();
	NSParser parser;

	bool ok = parser.verify(ui->editor->codeEdit()->toPlainText());

	if (ok)
	{
		//QMessageBox::information(this, "No errors", "The parsing has succeded");
	}
	else
	{
		QString text;

		for(const NSParsingError& e : parser.getErrors())
		{
			ui->editor->codeEdit()->addError(e.getLine(), e.getColumn(), e.getLength(), e.getMessage());

			text += " - ";
			text += e.print();
			text += '\n';
		}

		//QMessageBox::critical(this, "Verification failed", QString("The following error occured:\n").append(text));

	}
}
