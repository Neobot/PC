#include "ScriptView.h"
#include "ui_ScriptView.h"
#include "NetworkClientCommInterface.h"
#include "FileEnvReplicator.h"

ScriptView::ScriptView(NetworkConnection *connection, QWidget *parent) :
    AbstractView(connection, QImage(), parent), ui(new Ui::ScriptView)
{
	ui->setupUi(this);
    _connection->registerNetworkResponder(this);
     ui->filesView->setNetworkConnection(_connection);
	 ui->filesView->addAllowedExtension("ns");
}

ScriptView::~ScriptView()
{
	delete ui;
}

void ScriptView::connectionStatusChanged(NetworkConnection::ConnectionStatus status)
{
    switch(status)
    {
        case NetworkConnection::Lookup:
        case NetworkConnection::Disconnected:
            break;
        case NetworkConnection::Connected:
            ui->filesView->setFileCategory(Comm::GlobalScripts);
			ui->filesView->refresh();
            break;
        case NetworkConnection::Controled:
            break;
    }
}
