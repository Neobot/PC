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
	NSParser parser;

	QStringList errors;
	bool ok = parser.verify(ui->plainTextEdit->toPlainText(), errors);

	if (ok)
	{
		QMessageBox::information(this, "No errors", "The parsing has succeded");
	}
	else
	{
		for(QString& e: errors)
			e.prepend(" - ");

		QMessageBox::critical(this, "Verification failed", QString("The following error occured:\n").append(errors.join("\n")));

	}
}
