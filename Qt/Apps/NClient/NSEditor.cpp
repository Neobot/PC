#include "NSEditor.h"
#include "SyntaxRuleHighlighting.h"

#include <QBoxLayout>
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>

#include "NSParser.h"
#include "NSHelpDialog.h"

NSEditor::NSEditor(QWidget *parent) :
	QWidget(parent), _hasModifications(false)
{
	setupUi();
	newFile();

	_codeChangedSignalDelayer = new Tools::SignalDelayer(this);
	_codeChangedSignalDelayer->setDelayedSignal(_codeEdit, SIGNAL(codeChanged()), 1000);
	connect(_codeChangedSignalDelayer, SIGNAL(raised()), this, SLOT(check()));
}

NSEditor::~NSEditor()
{

}

Tools::NCodeEdit *NSEditor::codeEdit() const
{
	return _codeEdit;
}

QString NSEditor::getScript() const
{
    return _codeEdit->toPlainText();
}

void NSEditor::setScript(const QString &script)
{
	_codeEdit->setPlainText(script);
}

void NSEditor::addSearchDirectory(const QDir &dir)
{
	_searchDirectories << dir;
}

void NSEditor::setFileManagementEnabled(bool value)
{
	for(QAction* action : _fileManegementActions)
		action->setEnabled(value);
}

void NSEditor::setupUi()
{
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setContentsMargins(0,0,0,0);
	layout->setSpacing(0);

	QToolBar* toolBar = new QToolBar("NSEditor", this);
	_fileManegementActions << toolBar->addAction("New", this, SLOT(newFile()));
	toolBar->addSeparator();
	_fileManegementActions << toolBar->addAction("Open", this, SLOT(open()));
	_fileManegementActions << toolBar->addAction("Save", this, SLOT(save()));
	_fileManegementActions << toolBar->addAction("Save As", this, SLOT(saveAs()));
	toolBar->addSeparator();
	toolBar->addAction("Reference", this, SLOT(openReference()));

	_codeEdit = new Tools::NCodeEdit(this);
	_codeEdit->setSyntaxHighlighting(Tools::SyntaxRuleHighlighting::nsSyntaxRules());

	layout->addWidget(toolBar);
	layout->addWidget(_codeEdit);
}

void NSEditor::check()
{
	_hasModifications = true;

	QString code = _codeEdit->toPlainText();
	if (code.isEmpty())
		return;

	_codeEdit->clearErrors();
	NSParser parser;
    parser.setSearchDirectories(_searchDirectories);

	bool ok = parser.verify(code);

	if (!ok)
	{
		QString text;

		for(const NSParsingError& e : parser.getErrors())
		{
			_codeEdit->addError(e.getLine(), e.getColumn(), e.getLength(), e.getMessage());

			text += " - ";
			text += e.print();
			text += '\n';
		}
	}
}

void NSEditor::newFile()
{
	if (checkOnClose())
	{
		_currentFileName.clear();
		_codeEdit->blockSignals(true); //block the checking
		_codeEdit->clear();
		_codeEdit->blockSignals(false);
		_hasModifications = false;
	}
}

void NSEditor::open()
{
	if (checkOnClose())
	{
		QString filename = QFileDialog::getOpenFileName(this, "Select a Neobot Script file", _lastDir, "Neobot Script (*.ns)");
		if (!filename.isEmpty())
		{
			_currentFileName = filename;
			_lastDir = QFileInfo(filename).absolutePath();
			QFile f(filename);
			if (f.open(QIODevice::ReadOnly | QIODevice::Text))
			{
				QString content = f.readAll();
				_codeEdit->blockSignals(true); //block the checking to perform it manually
				_codeEdit->setPlainText(content);
				check();
				_codeEdit->blockSignals(false);
				_hasModifications = false;
			}
			else
			{
				QMessageBox::critical(this, "File not found", QString("Cannot open the file \"").append(filename).append("\""));
			}
		}
	}
}

bool NSEditor::save()
{
	if (!_currentFileName.isEmpty())
	{
		QFile f(_currentFileName);
		if (f.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QTextStream out(&f);
			out << _codeEdit->toPlainText();
			_hasModifications = false;
		}
		else
		{
			QMessageBox::critical(this, "Error", QString("Cannot save the file \"").append(_currentFileName).append("\""));
			return false;
		}
	}
	else
	{
		saveAs();
	}

	return true;
}

void NSEditor::saveAs()
{
	QString filename = QFileDialog::getSaveFileName(this, "Select a Neobot Script file name", _lastDir, "Neobot Script (*.ns)");
	if (!filename.isEmpty())
	{
		QString oldFilename = _currentFileName;
		_currentFileName = filename;
		_lastDir = QFileInfo(filename).absolutePath();

		if (!save())
		{
			_currentFileName = oldFilename;
		}
	}
}

void NSEditor::openReference()
{
	NSHelpDialog dialog(this);
	dialog.exec();
}

bool NSEditor::checkOnClose()
{
	bool canClose = true;
	if (_hasModifications)
	{
		int result = QMessageBox::question(this, "The script has unsaved changes", "Do you want to save the modifications",
							  QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
		switch(result)
		{
			case QMessageBox::Save:
				save();
				break;
			case QMessageBox::Cancel:
				canClose = false;
				break;
			case QMessageBox::Discard:
				break;
		}
	}

	return canClose;
}
