#ifndef NSEDITOR_H
#define NSEDITOR_H

#include "NCodeEdit.h"
#include "SignalDelayer.h"

#include <QWidget>

class NSEditor : public QWidget
{
	Q_OBJECT

public:
	explicit NSEditor(QWidget *parent = 0);
	~NSEditor();

	Tools::NCodeEdit* codeEdit() const;
	QString getScript() const;

private:
	Tools::NCodeEdit* _codeEdit;
	Tools::SignalDelayer* _codeChangedSignalDelayer;

	QString _lastDir;
	QString _currentFileName;

	bool _hasModifications;

	void setupUi();

	bool checkOnClose();

private slots:
	void check();
	void newFile();
	void open();
	bool save();
	void saveAs();
};

#endif // NSEDITOR_H
