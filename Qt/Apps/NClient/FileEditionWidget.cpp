#include "FileEditionWidget.h"
#include "ui_FileEditionWidget.h"

#include "GridEditor.h"
#include "NSettingsPropertyBrowser.h"

#include <QHeaderView>
#include <QFileDialog>
#include <QInputDialog>
#include <QToolButton>
#include <QTemporaryFile>
#include <QMessageBox>

FileEditionWidget::FileEditionWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FileEditionWidget), _gridEditor(nullptr), _propertiesBrowser(nullptr), _propertyDialog(nullptr), _plainTextEditor(nullptr), _textDialog(nullptr)
{
	ui->setupUi(this);

	_editMapper = new QSignalMapper(this);
	connect(_editMapper, SIGNAL(mapped(QString)), this, SIGNAL(editFile(QString)));
	connect(ui->tableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(rowDoubleClicked(int)));

	_exportMapper = new QSignalMapper(this);
	connect(_exportMapper, SIGNAL(mapped(QString)), this, SIGNAL(exportFile(QString)));

	_importMapper = new QSignalMapper(this);
	connect(_importMapper, SIGNAL(mapped(QString)), this, SIGNAL(importFile(QString)));

	_resetMapper = new QSignalMapper(this);
	connect(_resetMapper, SIGNAL(mapped(QString)), this, SIGNAL(resetFile(QString)));

	connect(ui->btnImportFile, SIGNAL(clicked()), this, SIGNAL(importFromDisk()));
}

FileEditionWidget::~FileEditionWidget()
{
	delete ui;
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
				connect(_gridEditor, SIGNAL(accepted()), this, SLOT(editionFinished()));
				connect(_gridEditor, SIGNAL(rejected()), this, SLOT(editionCanceled()));
			}

			if (!_gridEditor->isVisible())
			{
				_gridEditor->open(tmpFilePath);
				_gridEditor->show();
				editionOK = true;
			}
		}
		else if (info.suffix() == "nsettings")
		{
			if (!_propertyDialog)
			{
				_propertiesBrowser = new Tools::NSettingsPropertyBrowser(this);
				_propertyDialog = createEditionDialog(_propertiesBrowser);
				_propertyDialog->setWindowModality(Qt::WindowModal);
				connect(_propertyDialog, SIGNAL(accepted()), this, SLOT(parametersEditionFinished()));
				connect(_propertyDialog, SIGNAL(rejected()), this, SLOT(editionCanceled()));
			}

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
			if (!_textDialog)
			{
				_plainTextEditor = new QPlainTextEdit(this);
				_textDialog = createEditionDialog(_plainTextEditor);
				_textDialog->setWindowModality(Qt::WindowModal);
				connect(_textDialog, SIGNAL(accepted()), this, SLOT(editionFinished()));
				connect(_textDialog, SIGNAL(rejected()), this, SLOT(editionCanceled()));
			}

			if (!_textDialog->isVisible())
			{
				_plainTextEditor->setPlainText(data);
				_textDialog->show();
				editionOK = true;
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
			QMessageBox::critical(this, "Edition failed", QString("Cannot edit ").append(filename));
		}
	}
}

void FileEditionWidget::importFromDisk()
{
	QString file = QFileDialog::getOpenFileName(this, "Select a file to import");
	if (!file.isEmpty())
		emit addFile(file);
}

void FileEditionWidget::newFile()
{
	QString name = QInputDialog::getText(this, "New file", "Type the name and extension of the file.", QLineEdit::Normal, "New File");
	if (!name.isEmpty())
		emit newFile(name);
}

void FileEditionWidget::editionFinished()
{
	QFile f(_currentEditionData.localFile);
	if (f.open(QIODevice::ReadOnly))
	{
		QByteArray data = f.readAll();
		emit editionDone(_currentEditionData.filename, data);
		f.close();
		f.remove();
	}
}

void FileEditionWidget::editionCanceled()
{
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

void FileEditionWidget::parametersEditionFinished()
{
	Tools::NSettings settings = _propertiesBrowser->getSettings();
	settings.writeTo(_currentEditionData.localFile);

	editionFinished();
}

bool FileEditionWidget::editionInProgress() const
{
	return (_gridEditor && _gridEditor->isVisible())
			|| (_propertyDialog && _propertyDialog->isVisible())
			|| (_textDialog && _textDialog->isVisible());
}

void FileEditionWidget::rowDoubleClicked(int row)
{
	QString file = getFileAtRow(row);
	emit editFile(file);
}
