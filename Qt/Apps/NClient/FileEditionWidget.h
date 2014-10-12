#ifndef FILEEDITIONWIDGET_H
#define FILEEDITIONWIDGET_H

#include <QWidget>
#include <QSignalMapper>
#include <QPlainTextEdit>

namespace Ui {
	class FileEditionWidget;
}

class GridEditor;
namespace Tools {class NSettingsPropertyBrowser;}

class ButtonWidget : public QWidget
{
	Q_OBJECT

public:
	ButtonWidget(QWidget *parent = 0);

signals:
	void exportFile();
	void importFile();
	void removeFile();
	void editFile();
};

class FileEditionWidget : public QWidget
{
	Q_OBJECT

public:
	explicit FileEditionWidget(QWidget *parent = 0);
	~FileEditionWidget();

	void clear();
	void setFiles(const QStringList& filenames);
	QString getFileAtRow(int row) const;

	void editData(const QString& filename, const QByteArray& data);

	bool editionInProgress() const;

private:
	Ui::FileEditionWidget *ui;

	QSignalMapper* _editMapper;
	QSignalMapper* _exportMapper;
	QSignalMapper* _importMapper;
	QSignalMapper* _resetMapper;

	enum TableRows
	{
		FilenamePos = 1,
		ButtonsPos = 0
	};

	struct EditionData
	{
		QString localFile;
		QString filename;
	};
	EditionData _currentEditionData;

	GridEditor* _gridEditor;
	Tools::NSettingsPropertyBrowser* _propertiesBrowser;
	QDialog* _propertyDialog;
	QPlainTextEdit* _plainTextEditor;
	QDialog* _textDialog;

	QDialog* createEditionDialog(QWidget* mainWidget);

signals:
	void editFile(const QString& file);
	void exportFile(const QString& file);
	void importFile(const QString& file);
	void resetFile(const QString& file);
	void addFile(const QString& file);
	void newFile(const QString& name);

	void editionDone(const QString& filename, const QByteArray& data);

private slots:
	void importFromDisk();
	void newFile();

	void editionFinished();
	void editionCanceled();
	void parametersEditionFinished();

	void rowDoubleClicked(int row);
};

#endif // FILEEDITIONWIDGET_H
