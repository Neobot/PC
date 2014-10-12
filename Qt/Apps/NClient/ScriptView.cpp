#include "ScriptView.h"
#include "ui_ScriptView.h"

ScriptView::ScriptView(NetworkConnection *connection, QWidget *parent) :
	AbstractView(connection, QImage(), parent), ui(new Ui::ScriptView),
	_replicator(connection, Comm::GlobalScripts, "ns")
{
	ui->setupUi(this);
}

ScriptView::~ScriptView()
{
	delete ui;
}

void ScriptView::connectionStatusChanged(NetworkConnection::ConnectionStatus status)
{
	if (status == NetworkConnection::Connected)
		_replicator.refresh();
}


