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
	ui(new Ui::StrategiesView), _askFileContext(Edition)
{
	ui->setupUi(this);

	_connection->registerNetworkResponder(this);

	connectionStatusChanged(_connection->getConnectionStatus());

	connect(ui->cbStrategies, SIGNAL(currentIndexChanged(int)), this, SLOT(askStrategyFiles(int)));
	connect(ui->fileEditionWidget, SIGNAL(editionDone(QString,QByteArray)), this, SLOT(editionDone(QString,QByteArray)));
	connect(ui->fileEditionWidget, SIGNAL(editFile(QString)), this, SLOT(editFile(QString)));
	connect(ui->fileEditionWidget, SIGNAL(exportFile(QString)), this, SLOT(exportFile(QString)));
	connect(ui->fileEditionWidget, SIGNAL(importFile(QString)), this, SLOT(importFile(QString)));
	connect(ui->fileEditionWidget, SIGNAL(resetFile(QString)), this, SLOT(resetFile(QString)));
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

void StrategiesView::configurationFiles(int category, const QStringList &filenames)
{
	if (category < Comm::FirstStrategy || category > Comm::LastStrategy)
		return;

	ui->fileEditionWidget->clear();
	if (category == ui->cbStrategies->currentIndex())
	{
		ui->fileEditionWidget->setFiles(filenames);
	}
}

void StrategiesView::configurationFileData(int category, const QString &filename, const QByteArray &data)
{
	if (category < Comm::FirstStrategy || category > Comm::LastStrategy)
		return;

	if (_askFileContext == Edition)
	{
		ui->fileEditionWidget->editData(filename, data);
	}
	else if (_askFileContext == Export)
	{
		QString filter = QString(".*").append(QFileInfo(filename).suffix());
		QString localFile = QFileDialog::getSaveFileName(this, "Select a location", filename, filter);
		if (!localFile.isEmpty())
		{
			QFile file(localFile);
			if (file.open(QIODevice::WriteOnly))
				file.write(data);
			else
				QMessageBox::critical(this, "Error", QString("Export in ").append(filename).append(" failed!"));
		}
	}
}

QDialog *StrategiesView::createEditionDialog(QWidget *mainWidget)
{
	QDialog* d = new QDialog(this);
	QVBoxLayout* mainLayout = new QVBoxLayout(d);

	QHBoxLayout* buttonsLayout = new QHBoxLayout;

	QPushButton* btnOK = new QPushButton(d);
	btnOK->setText("OK");
	connect(btnOK, SIGNAL(clicked()), d, SLOT(accept()));

	QPushButton* btnCancel = new QPushButton(d);
	btnCancel->setText("Cancel");
	connect(btnCancel, SIGNAL(clicked()), d, SLOT(reject()));

	buttonsLayout->addStretch();
	buttonsLayout->addWidget(btnOK);
	buttonsLayout->addWidget(btnCancel);

	mainLayout->addWidget(mainWidget);
	mainLayout->addLayout(buttonsLayout);
	d->resize(1000, 500);

	return d;
}

void StrategiesView::loadSettings(QSettings *settings)
{
	settings->beginGroup("Strategies");
//	_gridEditor->resize(settings->value("GridEditorSize", _gridEditor->size()).toSize());
//	_gridEditor->setZoom(settings->value("GridEditorZoom", _gridEditor->getZoom()).toDouble());
//	_propertyDialog->resize(settings->value("PropertiesEditorSize", _propertyDialog->size()).toSize());
//	_textDialog->resize(settings->value("TextEditorSize", _textDialog->size()).toSize());
	settings->endGroup();
}

void StrategiesView::saveSettings(QSettings *settings)
{
	settings->beginGroup("Strategies");
//	settings->setValue("GridEditorSize", _gridEditor->size());
//	settings->setValue("GridEditorZoom", _gridEditor->getZoom());
//	settings->setValue("PropertiesEditorSize", _propertyDialog->size());
//	settings->setValue("TextEditorSize", _textDialog->size());
	settings->endGroup();
}

void StrategiesView::askStrategyFiles(int num)
{
	ui->fileEditionWidget->clear();
	if (num >= 0)
		_connection->getComm()->askFiles(num);
}

void StrategiesView::exportFile(const QString &file)
{
	_askFileContext = Export;
	int num = ui->cbStrategies->currentIndex();
	_connection->getComm()->askFileData(num, file);
}

void StrategiesView::importFile(const QString &file)
{
	QString filter = QString("*.").append(QFileInfo(file).suffix());
	QString localFile = QFileDialog::getOpenFileName(this, "Select a file", QString(), filter);
	if (!localFile.isEmpty())
	{
		int num = ui->cbStrategies->currentIndex();
        QFile f(localFile);
		if (f.open(QIODevice::ReadOnly))
		{
			QByteArray data = f.readAll();
			_connection->getComm()->sendFileData(num, file, data);
			f.close();
		}
	}
}

void StrategiesView::resetFile(const QString &file)
{
	if (QMessageBox::question(this, "Reset/Remove a file", QString("Are you sure, you want to remove the file ").append(file).append("?"),
						  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
	{
		int num = ui->cbStrategies->currentIndex();
		_connection->getComm()->resetFileData(num, file);
	}
}

void StrategiesView::editionDone(const QString &filename, const QByteArray &data)
{
	_connection->getComm()->sendFileData(ui->cbStrategies->currentIndex(), filename, data);
}

void StrategiesView::editFile(const QString &file)
{
	if (!ui->fileEditionWidget->editionInProgress())
	{
		_askFileContext = Edition;
		int num = ui->cbStrategies->currentIndex();
		_connection->getComm()->askFileData(num, file);
	}
}
