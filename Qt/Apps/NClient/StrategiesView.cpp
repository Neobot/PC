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

	_editMapper = new QSignalMapper(this);
	connect(_editMapper, SIGNAL(mapped(QString)), this, SLOT(editFile(QString)));
	connect(ui->tableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(rowDoubleClicked(int)));

	_exportMapper = new QSignalMapper(this);
	connect(_exportMapper, SIGNAL(mapped(QString)), this, SLOT(exportFile(QString)));

	_importMapper = new QSignalMapper(this);
	connect(_importMapper, SIGNAL(mapped(QString)), this, SLOT(importFile(QString)));

	_resetMapper = new QSignalMapper(this);
	connect(_resetMapper, SIGNAL(mapped(QString)), this, SLOT(resetFile(QString)));

    _gridEditor = new GridEditor(this, true, true);
    _gridEditor->set2014Table();
	_gridEditor->setWindowModality(Qt::WindowModal);
	connect(_gridEditor, SIGNAL(accepted()), this, SLOT(editionFinished()));
	connect(_gridEditor, SIGNAL(rejected()), this, SLOT(editionCanceled()));

	_propertiesBrowser = new Tools::NSettingsPropertyBrowser(this);
	_propertyDialog = createEditionDialog(_propertiesBrowser);
	_propertyDialog->setWindowModality(Qt::WindowModal);
	connect(_propertyDialog, SIGNAL(accepted()), this, SLOT(parametersEditionFinished()));
	connect(_propertyDialog, SIGNAL(rejected()), this, SLOT(editionCanceled()));

	_editor = new QPlainTextEdit(this);
	_textDialog = createEditionDialog(_editor);
	_textDialog->setWindowModality(Qt::WindowModal);
	connect(_textDialog, SIGNAL(accepted()), this, SLOT(editionFinished()));
	connect(_textDialog, SIGNAL(rejected()), this, SLOT(editionCanceled()));

	_connection->registerNetworkResponder(this);

	connectionStatusChanged(_connection->getConnectionStatus());

	connect(ui->cbStrategies, SIGNAL(currentIndexChanged(int)), this, SLOT(askStrategyFiles(int)));
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
			ui->tableWidget->clear();
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

void StrategiesView::setStrategyFiles(int strategyNum, const QStringList &filenames)
{
	ui->tableWidget->clear();
	if (strategyNum == ui->cbStrategies->currentIndex())
	{
		ui->tableWidget->setColumnCount(5);
		ui->tableWidget->setRowCount(filenames.count());
		ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
		ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

		int row = 0;
		foreach(const QString& file, filenames)
		{
			QTableWidgetItem* item = new QTableWidgetItem(file);
			ui->tableWidget->setItem(row, 4, item);

			QToolButton* editBtn = new QToolButton(ui->tableWidget);
			editBtn->setText("Edit");
			editBtn->setToolTip("Edit the file");
			editBtn->setIcon(QIcon(":/buttons/edit"));
			editBtn->setAutoRaise(true);
			editBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
			ui->tableWidget->setCellWidget(row, 0, editBtn);
			_editMapper->setMapping(editBtn, file);
			connect(editBtn, SIGNAL(clicked()), _editMapper, SLOT(map()));

			QToolButton* exportBtn = new QToolButton(ui->tableWidget);
			exportBtn->setText("Export");
			exportBtn->setToolTip("Export the file");
			exportBtn->setIcon(QIcon(":/buttons/export"));
			exportBtn->setAutoRaise(true);
			exportBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
			ui->tableWidget->setCellWidget(row, 1, exportBtn);
			_exportMapper->setMapping(exportBtn, file);
			connect(exportBtn, SIGNAL(clicked()), _exportMapper, SLOT(map()));

			QToolButton* importBtn = new QToolButton(ui->tableWidget);
			importBtn->setText("Import");
			importBtn->setToolTip("Import the file");
			importBtn->setIcon(QIcon(":/buttons/import"));
			importBtn->setAutoRaise(true);
			importBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
			ui->tableWidget->setCellWidget(row, 2, importBtn);
			_importMapper->setMapping(importBtn, file);
			connect(importBtn, SIGNAL(clicked()), _importMapper, SLOT(map()));

			QToolButton* resetBtn = new QToolButton(ui->tableWidget);
			resetBtn->setText("Reset");
			resetBtn->setToolTip("Reset/Delete the file");
			resetBtn->setIcon(QIcon(":/buttons/reset"));
			resetBtn->setAutoRaise(true);
			resetBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
			ui->tableWidget->setCellWidget(row, 3, resetBtn);
			_resetMapper->setMapping(resetBtn, file);
			connect(resetBtn, SIGNAL(clicked()), _resetMapper, SLOT(map()));

			++row;
		}
	}
}

void StrategiesView::setStrategyFileData(int strategyNum, const QString &filename, const QByteArray &data)
{
	if (_askFileContext == Edition)
	{
		QTemporaryFile tmp(QString("StratXXXXXX_").append(filename));
		if (tmp.open())
		{
			tmp.write(data);
			tmp.close();

			QString tmpFilePath = QFileInfo(tmp.fileName()).absoluteFilePath();

			bool editionOK = false;

			QFileInfo info(filename);
			if (info.suffix() == "ngrid")
			{
				if (!_gridEditor->isVisible())
				{
					_gridEditor->open(tmpFilePath);
					_gridEditor->show();
					editionOK = true;
				}
			}
			else if (info.suffix() == "nsettings")
			{
				if (!_propertyDialog->isVisible())
				{
					Tools::NSettings settings;
					settings.loadFrom(tmpFilePath);
					_propertiesBrowser->setSettings(settings);
					_propertyDialog->show();
					editionOK = true;
				}
			}
			else
			{
				if (!_textDialog->isVisible())
				{
					_editor->setPlainText(data);
					_textDialog->show();
					editionOK = true;
				}
			}

			if (editionOK)
			{
				_currentEditionData.filename = filename;
				_currentEditionData.localFile = tmpFilePath;
				_currentEditionData.num = strategyNum;
			}
			else
			{
				_currentEditionData.filename.clear();
				_currentEditionData.localFile.clear();
				_currentEditionData.num = -1;
				QMessageBox::critical(this, "Edition failed", QString("Cannot edit ").append(filename));
			}
		}
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

bool StrategiesView::editionInProgress() const
{
	return _gridEditor->isVisible() || _propertyDialog->isVisible() || _textDialog->isVisible();
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
	_gridEditor->resize(settings->value("GridEditorSize", _gridEditor->size()).toSize());
	_gridEditor->setZoom(settings->value("GridEditorZoom", _gridEditor->getZoom()).toDouble());
	_propertyDialog->resize(settings->value("PropertiesEditorSize", _propertyDialog->size()).toSize());
	_textDialog->resize(settings->value("TextEditorSize", _textDialog->size()).toSize());
	settings->endGroup();
}

void StrategiesView::saveSettings(QSettings *settings)
{
	settings->beginGroup("Strategies");
	settings->setValue("GridEditorSize", _gridEditor->size());
	settings->setValue("GridEditorZoom", _gridEditor->getZoom());
	settings->setValue("PropertiesEditorSize", _propertyDialog->size());
	settings->setValue("TextEditorSize", _textDialog->size());
	settings->endGroup();
}

void StrategiesView::askStrategyFiles(int num)
{
	ui->tableWidget->clear();
	if (num >= 0)
		_connection->getComm()->askStrategyFiles(num);
}

void StrategiesView::rowDoubleClicked(int row)
{
	QString file = ui->tableWidget->item(row, 4)->text();
	editFile(file);
}

void StrategiesView::editionFinished()
{
	QFile f(_currentEditionData.localFile);
	if (f.open(QIODevice::ReadOnly))
	{
		QByteArray data = f.readAll();
		_connection->getComm()->sendStrategyFileData(_currentEditionData.num, _currentEditionData.filename, data);
		f.close();
		f.remove();
	}
}

void StrategiesView::editionCanceled()
{
	QFile::remove(_currentEditionData.localFile);
}

void StrategiesView::parametersEditionFinished()
{
	Tools::NSettings settings = _propertiesBrowser->getSettings();
	settings.writeTo(_currentEditionData.localFile);

	editionFinished();
}

void StrategiesView::exportFile(const QString &file)
{
	_askFileContext = Export;
	int num = ui->cbStrategies->currentIndex();
	_connection->getComm()->askStrategyFileData(num, file);
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
			_connection->getComm()->sendStrategyFileData(num, file, data);
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
		_connection->getComm()->resetStrategyFileData(num, file);
	}
}

void StrategiesView::editFile(const QString &file)
{
	if (!editionInProgress())
	{
		_askFileContext = Edition;
		int num = ui->cbStrategies->currentIndex();
		_connection->getComm()->askStrategyFileData(num, file);
	}
}
