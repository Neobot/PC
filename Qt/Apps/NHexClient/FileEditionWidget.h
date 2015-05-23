#ifndef FILEEDITIONWIDGET_H
#define FILEEDITIONWIDGET_H

#include <QWidget>
#include <QSignalMapper>
#include <QPlainTextEdit>
#include <QSettings>

namespace Ui {
	class FileEditionWidget;
}

class GridEditor;
namespace Tools {class NSettingsPropertyBrowser;}
class NSEditor;
class CommInterface;

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

	void setInteractionEnabled(bool value);

	void setNetworkConnection(CommInterface* connection);
    void setFileCategory(int category);
	void addAllowedExtension(const QString& extension);

	void clear();
	void setFiles(const QStringList& filenames);
	QString getFileAtRow(int row) const;

	void editData(const QString& filename, const QByteArray& data);

    bool editionInProgress() const;

	void loadSettings(QSettings* settings);
	void saveSettings(QSettings* settings);

public slots:
	void refresh();

private:
	Ui::FileEditionWidget *ui;

	CommInterface* _connection;
    int _category;
	QStringList _allowedExtensions;

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
        QWidget* editor = nullptr;
	};
	EditionData _currentEditionData;

    enum AskStrategyFileContext
    {
		None,
        Edition,
        Export
    };
    AskStrategyFileContext _askFileContext;

	GridEditor* _gridEditor;
	QByteArray _gridEditorGeometry;
	QByteArray _gridEditorState;
	double _gridEditorZoom = 1.0;

    Tools::NSettingsPropertyBrowser* _propertiesBrowser;
    QDialog* _propertyDialog;
	QByteArray _propertyDialogGeometry;

    QPlainTextEdit* _plainTextEditor;
    QDialog* _textDialog;
	QByteArray _textDialogGeometry;

    NSEditor* _nsEditor;
    QDialog* _nsDialog;
	QByteArray _nsDialogGeometry;

	QDialog* createEditionDialog(QWidget* mainWidget);
	QString buildFileFilter();

    void configurationFiles(int category, const QStringList &filenames);
    void configurationFileData(int category, const QString &filename, const QByteArray &data);

private slots:
	void importFromDisk();
	void newFile();

	void editionFinished();
	void editionCanceled();

	void rowDoubleClicked(int row);

    void editFile(const QString& file);
    void exportFile(const QString& file);
    void importFile(const QString& file);
    void resetFile(const QString& file);
};

#endif // FILEEDITIONWIDGET_H
