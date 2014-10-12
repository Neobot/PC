#ifndef STRATEGIESVIEW_H
#define STRATEGIESVIEW_H

#include "AbstractView.h"
#include <QSignalMapper>
#include <QPlainTextEdit>

namespace Ui {
	class StrategiesView;
}

namespace Tools
{
	class NSettingsPropertyBrowser;
}

class GridEditor;

class StrategiesView : public AbstractView, public NetworkClientCommListener
{
	Q_OBJECT
	
public:
	explicit StrategiesView(NetworkConnection *connection, QWidget *parent = 0);
	~StrategiesView();
	

public slots:
	virtual void connectionStatusChanged(NetworkConnection::ConnectionStatus status);

private:
	Ui::StrategiesView *ui;
	QSignalMapper* _editMapper;
	QSignalMapper* _exportMapper;
	QSignalMapper* _importMapper;
	QSignalMapper* _resetMapper;
	GridEditor* _gridEditor;

	Tools::NSettingsPropertyBrowser* _propertiesBrowser;
	QDialog* _propertyDialog;

	QPlainTextEdit* _editor;
	QDialog* _textDialog;

	struct EditionData
	{
		QString localFile;
		QString filename;
		int num;
	};
	EditionData _currentEditionData;

	enum AskStrategyFileContext
	{
		Edition,
		Export
	};
	AskStrategyFileContext _askFileContext;

	void setStrategies(const QStringList &strategies);
	void configurationFiles(int category, const QStringList &filenames);
	void configurationFileData(int category, const QString &filename, const QByteArray &data);

	bool editionInProgress() const;
	QDialog* createEditionDialog(QWidget* mainWidget);

	void loadSettings(QSettings* settings);
	void saveSettings(QSettings* settings);

private slots:
	void askStrategyFiles(int num);
	void editFile(const QString& file);
	void rowDoubleClicked(int row);
	void editionFinished();
	void editionCanceled();
	void parametersEditionFinished();

	void exportFile(const QString& file);
	void importFile(const QString& file);
	void resetFile(const QString& file);
};

#endif // STRATEGIESVIEW_H
