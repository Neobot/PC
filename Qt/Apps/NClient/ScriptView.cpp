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
	 ui->filesView->setInteractionEnabled(false);
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
			ui->filesView->setInteractionEnabled(false);
            break;
        case NetworkConnection::Connected:
            ui->filesView->setFileCategory(Comm::GlobalScripts);
			ui->filesView->refresh();
			ui->filesView->setInteractionEnabled(true);
            break;
        case NetworkConnection::Controled:
			ui->filesView->setInteractionEnabled(true);
            break;
    }
}

void ScriptView::loadSettings(QSettings *settings)
{
	settings->beginGroup("GlobalScripts");
	settings->beginGroup("FileEditor");
	ui->filesView->loadSettings(settings);
	settings->endGroup();
	settings->endGroup();
}

void ScriptView::saveSettings(QSettings *settings)
{
	settings->beginGroup("GlobalScripts");
	settings->beginGroup("FileEditor");
	ui->filesView->saveSettings(settings);
	settings->endGroup();
	settings->endGroup();
}
