#include "FileEditionWidget.h"
#include "ui_FileEditionWidget.h"

#include "GridEditor.h"
#include "NSettingsPropertyBrowser.h"
#include "NSEditor.h"
#include "NetworkConnection.h"
#include "NetworkClientCommInterface.h"
#include "FileEnvReplicator.h"

#include <QHeaderView>
#include <QFileDialog>
#include <QInputDialog>
#include <QToolButton>
#include <QTemporaryFile>
#include <QMessageBox>

FileEditionWidget::FileEditionWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FileEditionWidget),  _askFileContext(None), _gridEditor(nullptr), _propertiesBrowser(nullptr),
    _propertyDialog(nullptr), _plainTextEditor(nullptr), _textDialog(nullptr),
	_nsEditor(nullptr), _nsDialog(nullptr)
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

    connect(ui->btnImportFile, SIGNAL(clicked()), this, SLOT(importFromDisk()));
    connect(ui->btnNewFile, SIGNAL(clicked()), this, SLOT(newFile()));
}

FileEditionWidget::~FileEditionWidget()
{
	delete ui;
}

void FileEditionWidget::setInteractionEnabled(bool value)
{
	ui->btnImportFile->setEnabled(value);
	ui->btnNewFile->setEnabled(value);
	ui->tableWidget->setEnabled(value);
}

void FileEditionWidget::setNetworkConnection(NetworkConnection *connection)
{
    _connection = connection;
    _connection->registerNetworkResponder(this);
}

void FileEditionWidget::setFileCategory(int category)
{
	_category = category;
}

void FileEditionWidget::addAllowedExtension(const QString &extension)
{
	_allowedExtensions << extension;
}

void FileEditionWidget::refresh()
{
	_connection->getComm()->askFiles(_category);
}

void FileEditionWidget::clear()
{
	ui->tableWidget->clear();
}

void FileEditionWidget::setFiles(const QStringList &filenames)
{
	ui->tableWidget->clear();
	ui->tableWidget->setColumnCount(2);
	ui->tableWidget->setRowCount(filenames.count());
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(FilenamePos, QHeaderView::Stretch);

	int row = 0;
	foreach(const QString& file, filenames)
	{
		QTableWidgetItem* item = new QTableWidgetItem(file);
		ui->tableWidget->setItem(row, FilenamePos, item);

		ButtonWidget* btns = new ButtonWidget(this);
		ui->tableWidget->setCellWidget(row, ButtonsPos, btns);

		_editMapper->setMapping(btns, file);
		_exportMapper->setMapping(btns, file);
		_importMapper->setMapping(btns, file);
		_resetMapper->setMapping(btns, file);

		connect(btns, SIGNAL(editFile()), _editMapper, SLOT(map()));
		connect(btns, SIGNAL(exportFile()), _exportMapper, SLOT(map()));
		connect(btns, SIGNAL(importFile()), _importMapper, SLOT(map()));
		connect(btns, SIGNAL(removeFile()), _resetMapper, SLOT(map()));

		++row;
	}
}

QString FileEditionWidget::getFileAtRow(int row) const
{
	return ui->tableWidget->item(row, FilenamePos)->text();
}

void FileEditionWidget::editData(const QString &filename, const QByteArray &data)
{
	QTemporaryFile tmp(QString("Edition_XXXXXX_").append(filename));
	if (tmp.open())
	{
		tmp.write(data);
		tmp.close();

		QString tmpFilePath = QFileInfo(tmp.fileName()).absoluteFilePath();

		bool editionOK = false;

		QFileInfo info(filename);
		if (info.suffix() == "ngrid")
		{
			if (!_gridEditor)
			{
                _gridEditor = new GridEditor(this, true, true);
				_gridEditor->setLastTable();
				_gridEditor->setWindowModality(Qt::WindowModal);
				_gridEditor->restoreGeometry(_gridEditorGeometry);
				_gridEditor->restoreState(_gridEditorState);
				_gridEditor->setZoom(_gridEditorZoom);
				connect(_gridEditor, SIGNAL(accepted()), this, SLOT(editionFinished()));
				connect(_gridEditor, SIGNAL(rejected()), this, SLOT(editionCanceled()));
			}

			if (!_gridEditor->isVisible())
			{
				_gridEditor->open(tmpFilePath);
				_gridEditor->show();
				editionOK = true;
                _currentEditionData.editor = _gridEditor;
			}
		}
		else if (info.suffix() == "nsettings")
		{
			if (!_propertyDialog)
			{
				_propertiesBrowser = new Tools::NSettingsPropertyBrowser(this);
				_propertyDialog = createEditionDialog(_propertiesBrowser);
				_propertyDialog->setWindowModality(Qt::WindowModal);
				_propertyDialog->restoreGeometry(_propertyDialogGeometry);
                connect(_propertyDialog, SIGNAL(accepted()), this, SLOT(editionFinished()));
				connect(_propertyDialog, SIGNAL(rejected()), this, SLOT(editionCanceled()));
			}

			if (!_propertyDialog->isVisible())
			{
				Tools::NSettings settings;
				settings.loadFrom(tmpFilePath);
				_propertiesBrowser->setSettings(settings);
				_propertyDialog->show();
				editionOK = true;
                _currentEditionData.editor = _propertiesBrowser;
			}
		}
        else if (info.suffix() == "ns")
        {
            if (!_nsDialog)
            {
                _nsEditor = new NSEditor(this);
                _nsEditor->addSearchDirectory(_connection->getGlobalScriptDirectory());
				_nsEditor->setFileManagementEnabled(false);
                _nsDialog = createEditionDialog(_nsEditor);
                _nsDialog->setWindowModality(Qt::WindowModal);
				_nsDialog->restoreGeometry(_nsDialogGeometry);
                connect(_nsDialog, SIGNAL(accepted()), this, SLOT(editionFinished()));
                connect(_nsDialog, SIGNAL(rejected()), this, SLOT(editionCanceled()));
            }

            if (!_nsDialog->isVisible())
            {
                _nsEditor->setScript(data);
                _nsDialog->show();
                editionOK = true;
                _currentEditionData.editor = _nsEditor;
            }
        }
		else
		{
			if (!_textDialog)
			{
				_plainTextEditor = new QPlainTextEdit(this);
				_textDialog = createEditionDialog(_plainTextEditor);
				_textDialog->setWindowModality(Qt::WindowModal);
				_textDialog->restoreGeometry(_textDialogGeometry);
				connect(_textDialog, SIGNAL(accepted()), this, SLOT(editionFinished()));
				connect(_textDialog, SIGNAL(rejected()), this, SLOT(editionCanceled()));
			}

			if (!_textDialog->isVisible())
			{
				_plainTextEditor->setPlainText(data);
				_textDialog->show();
				editionOK = true;
                _currentEditionData.editor = _plainTextEditor;
			}
		}

		if (editionOK)
		{
			_currentEditionData.filename = filename;
			_currentEditionData.localFile = tmpFilePath;
		}
		else
		{
			_currentEditionData.filename.clear();
			_currentEditionData.localFile.clear();
            _currentEditionData.editor = nullptr;
			QMessageBox::critical(this, "Edition failed", QString("Cannot edit ").append(filename));
		}
	}
}

void FileEditionWidget::importFromDisk()
{
	QString file = QFileDialog::getOpenFileName(this, "Select a file to import", QString(), buildFileFilter());
	if (!file.isEmpty())
    {
        QFile f(file);
        if (f.open(QIODevice::ReadOnly))
        {
            QByteArray data = f.readAll();
            _connection->getComm()->sendFileData(_category, QFileInfo(file).fileName(), data);
			QTimer::singleShot(500, this, SLOT(refresh()));
            f.close();
        }
    }
}

void FileEditionWidget::newFile()
{
	QString name = QInputDialog::getText(this, "New file", "Type the name and extension of the file.", QLineEdit::Normal, "New File");
	if (!name.isEmpty())
	{
		QFileInfo info(name);
		if (!_allowedExtensions.isEmpty() && (info.suffix().isEmpty() || (!_allowedExtensions.contains(info.suffix()) && !_allowedExtensions.contains(QString()))))
		{
				QString defaultExt = _allowedExtensions.value(0);
				if (!defaultExt.isEmpty())
				{
					name += ".";
					name += defaultExt;
				}
		}

		_connection->getComm()->sendFileData(_category, name, QByteArray());
		QTimer::singleShot(500, this, SLOT(refresh()));
	}
}

void FileEditionWidget::editionFinished()
{
    bool readFromTmpFile = false;
	_askFileContext = None;

    QByteArray data;
    if (_currentEditionData.editor == _nsEditor)
    {
        data = _nsEditor->getScript().toLatin1();
    }
    else if (_currentEditionData.editor == _propertiesBrowser)
    {
        Tools::NSettings settings = _propertiesBrowser->getSettings();
        settings.writeTo(_currentEditionData.localFile);
        readFromTmpFile = true;
    }
    else if (_currentEditionData.editor == _plainTextEditor)
    {
         data = _plainTextEditor->toPlainText().toLatin1();
    }
    else
    {
        readFromTmpFile = true;
    }

    if (readFromTmpFile)
    {
        QFile f(_currentEditionData.localFile);
        if (f.open(QIODevice::ReadOnly))
        {
           data = f.readAll();
           f.close();
        }
    }

    QFile::remove(_currentEditionData.localFile);
	if (_category == Comm::GlobalScripts)
		_connection->getNsEnvReplicator().refreshWithData(_currentEditionData.filename, data);
	_connection->getComm()->sendFileData(_category, _currentEditionData.filename, data);
}

void FileEditionWidget::editionCanceled()
{
	_askFileContext = None;
	QFile::remove(_currentEditionData.localFile);
}

ButtonWidget::ButtonWidget(QWidget *parent) : QWidget(parent)
{
	QToolButton* editBtn = new QToolButton(this);
	editBtn->setText("Edit");
	editBtn->setToolTip("Edit the file");
	editBtn->setIcon(QIcon(":/buttons/edit"));
	editBtn->setAutoRaise(true);
	editBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	connect(editBtn, SIGNAL(clicked()), this, SIGNAL(editFile()));

	QToolButton* exportBtn = new QToolButton(this);
	exportBtn->setText("Export");
	exportBtn->setToolTip("Export the file");
	exportBtn->setIcon(QIcon(":/buttons/export"));
	exportBtn->setAutoRaise(true);
	exportBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	connect(exportBtn, SIGNAL(clicked()), this, SIGNAL(exportFile()));

	QToolButton* importBtn = new QToolButton(this);
	importBtn->setText("Import");
	importBtn->setToolTip("Import the file");
	importBtn->setIcon(QIcon(":/buttons/import"));
	importBtn->setAutoRaise(true);
	importBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	connect(importBtn, SIGNAL(clicked()), this, SIGNAL(importFile()));

	QToolButton* resetBtn = new QToolButton(this);
	resetBtn->setText("Reset");
	resetBtn->setToolTip("Reset/Delete the file");
	resetBtn->setIcon(QIcon(":/buttons/reset"));
	resetBtn->setAutoRaise(true);
	resetBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	connect(resetBtn, SIGNAL(clicked()), this, SIGNAL(removeFile()));

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setContentsMargins(0,0,0,0);
	layout->setSpacing(0);

	layout->addSpacing(5);
	layout->addWidget(editBtn);
	layout->addSpacing(4);
	layout->addWidget(exportBtn);
	layout->addWidget(importBtn);
	layout->addSpacing(4);
	layout->addWidget(resetBtn);
	layout->addSpacing(5);
}

QDialog *FileEditionWidget::createEditionDialog(QWidget *mainWidget)
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

QString FileEditionWidget::buildFileFilter()
{
	QString filter;
	foreach(const QString& ext, _allowedExtensions)
	{
		filter += " *.";

		if (ext.isEmpty())
			filter += "*";
		else
			filter += ext;
	}

	return filter;
}

void FileEditionWidget::configurationFiles(int category, const QStringList &filenames)
{
    if (_category == category)
    {
        clear();
        setFiles(filenames);
    }
}

void FileEditionWidget::configurationFileData(int category, const QString &filename, const QByteArray &data)
{
    if (_category == category)
    {
        if (_askFileContext == Edition)
        {
           editData(filename, data);
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
}

bool FileEditionWidget::editionInProgress() const
{
	return (_gridEditor && _gridEditor->isVisible())
			|| (_propertyDialog && _propertyDialog->isVisible())
			|| (_textDialog && _textDialog->isVisible());
}

void FileEditionWidget::loadSettings(QSettings *settings)
{
	_gridEditorGeometry = settings->value("GridEditorGeometry").toByteArray();
	_gridEditorState = settings->value("GridEditorState").toByteArray();
	_gridEditorZoom = settings->value("GridEditorZoom", 0.25).toDouble();
	_propertyDialogGeometry = settings->value("PropertiesEditorGeometry").toByteArray();
	_textDialogGeometry = settings->value("TextEditorGeometry").toByteArray();
	_nsDialogGeometry = settings->value("NSEditorGeometry").toByteArray();
}

void FileEditionWidget::saveSettings(QSettings *settings)
{
	if (_gridEditor)
	{
		settings->setValue("GridEditorGeometry", _gridEditor->saveGeometry());
		settings->setValue("GridEditorState", _gridEditor->saveState());
		settings->setValue("GridEditorZoom", _gridEditor->getZoom());
	}

	if (_propertyDialog)
		settings->setValue("PropertiesEditorGeometry", _propertyDialog->saveGeometry());

	if (_textDialog)
		settings->setValue("TextEditorGeometry", _textDialog->saveGeometry());

	if (_nsDialog)
		settings->setValue("NSEditorGeometry", _nsDialog->saveGeometry());
}

void FileEditionWidget::rowDoubleClicked(int row)
{
	QString file = getFileAtRow(row);
    editFile(file);
}

void FileEditionWidget::exportFile(const QString &file)
{
    _askFileContext = Export;
    _connection->getComm()->askFileData(_category, file);
}

void FileEditionWidget::importFile(const QString &file)
{
    QString filter = QString("*.").append(QFileInfo(file).suffix());
    QString localFile = QFileDialog::getOpenFileName(this, "Select a file", QString(), filter);
    if (!localFile.isEmpty())
    {
        QFile f(localFile);
        if (f.open(QIODevice::ReadOnly))
        {
            QByteArray data = f.readAll();
            _connection->getComm()->sendFileData(_category, file, data);
            f.close();
        }
    }
}

void FileEditionWidget::resetFile(const QString &file)
{
    if (QMessageBox::question(this, "Reset/Remove a file", QString("Are you sure, you want to remove the file ").append(file).append("?"),
                          QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
    {
        _connection->getComm()->resetFileData(_category, file);
		refresh();
    }
}

void FileEditionWidget::editFile(const QString &file)
{
	if (!editionInProgress())
    {
        _askFileContext = Edition;
        _connection->getComm()->askFileData(_category, file);
    }
}
