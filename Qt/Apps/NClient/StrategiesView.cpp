#include "StrategiesView.h"
#include "ui_StrategiesView.h"

#include "NetworkClientCommInterface.h"
#include "GridEditor.h"
#include "NSettingsPropertyBrowser.h"

#include <QHeaderView>
#include <QToolButton>
#include <QTemporaryFile>
#include <QFileInfo>
#include <QProcess>
#include <QMessageBox>
#include <QFileDialog>

#include <QPushButton>

StrategiesView::StrategiesView(NetworkConnection *connection, QWidget *parent) :
	AbstractView(connection, QImage(), parent),
	ui(new Ui::StrategiesView)
{
	ui->setupUi(this);

	_connection->registerNetworkResponder(this);

	connectionStatusChanged(_connection->getConnectionStatus());

	connect(ui->cbStrategies, SIGNAL(currentIndexChanged(int)), this, SLOT(askStrategyFiles(int)));
     ui->fileEditionWidget->setNetworkConnection(_connection);
	 ui->fileEditionWidget->addAllowedExtension("ns");
	 ui->fileEditionWidget->addAllowedExtension("ngrid");
	 ui->fileEditionWidget->addAllowedExtension("nsettings");
	 ui->fileEditionWidget->addAllowedExtension("");
}

StrategiesView::~StrategiesView()
{
	delete ui;
}

void StrategiesView::connectionStatusChanged(NetworkConnection::ConnectionStatus status)
{
	switch(status)
	{
		case NetworkConnection::Lookup:
		case NetworkConnection::Disconnected:
			ui->cbStrategies->blockSignals(true);
			ui->cbStrategies->clear();
			ui->cbStrategies->blockSignals(false);
			ui->fileEditionWidget->clear();
			ui->cbStrategies->setEnabled(false);
			break;
		case NetworkConnection::Connected:
			_connection->getComm()->askStrategies();
			ui->cbStrategies->setEnabled(true);
			break;
		case NetworkConnection::Controled:
			_connection->getComm()->askStrategyStatus();
			ui->cbStrategies->setEnabled(true);
			break;
	}
}

void StrategiesView::setStrategies(const QStringList &strategies)
{
	ui->cbStrategies->blockSignals(true);
	ui->cbStrategies->clear();
	ui->cbStrategies->blockSignals(false);
	ui->cbStrategies->addItems(strategies);
}

void StrategiesView::loadSettings(QSettings *settings)
{
	settings->beginGroup("Strategies");
	settings->endGroup();
}

void StrategiesView::saveSettings(QSettings *settings)
{
	settings->beginGroup("Strategies");
	settings->endGroup();
}

void StrategiesView::askStrategyFiles(int num)
{
	ui->fileEditionWidget->clear();
	if (num >= 0)
    {
        ui->fileEditionWidget->setFileCategory(num);
		ui->fileEditionWidget->refresh();
    }
}
