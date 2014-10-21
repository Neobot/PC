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

	void setStrategies(const QStringList &strategies);

	void loadSettings(QSettings* settings);
	void saveSettings(QSettings* settings);

private slots:
	void askStrategyFiles(int num);
};

#endif // STRATEGIESVIEW_H
