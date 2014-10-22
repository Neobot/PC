#ifndef NSEDITOR_H
#define NSEDITOR_H

#include "NCodeEdit.h"
#include "SignalDelayer.h"

#include <QWidget>
#include <QDir>

class NSEditor : public QWidget
{
	Q_OBJECT

public:
	explicit NSEditor(QWidget *parent = 0);
	~NSEditor();

	Tools::NCodeEdit* codeEdit() const;
	QString getScript() const;
    void setScript(const QString& script);

    void addSearchDirectory(const QDir& dir);

	void setFileManagementEnabled(bool value);

private:
	Tools::NCodeEdit* _codeEdit;
	Tools::SignalDelayer* _codeChangedSignalDelayer;

	QString _lastDir;
	QString _currentFileName;

	bool _hasModifications;

    QList<QDir> _searchDirectories;
	QList<QAction*> _fileManegementActions;

	void setupUi();

	bool checkOnClose();

private slots:
	void check();
	void newFile();
	void open();
	bool save();
	void saveAs();
	void openReference();
};

#endif // NSEDITOR_H
