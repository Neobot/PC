#include "ScriptView.h"
#include "ui_ScriptView.h"

ScriptView::ScriptView(NetworkConnection *connection, QWidget *parent) :
	AbstractView(connection, QImage(), parent),
	ui(new Ui::ScriptView)
{
	ui->setupUi(this);
}

ScriptView::~ScriptView()
{
	delete ui;
}

void ScriptView::connectionStatusChanged(NetworkConnection::ConnectionStatus status)
{

}


