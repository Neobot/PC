#ifndef NCLIENT_H
#define NCLIENT_H

#include <QMainWindow>
#include <QActionGroup>
#include <QStackedWidget>
#include <QSettings>
#include "CommInterface.h"

class AbstractView;

namespace Ui {
	class NClient;
}

class NClient : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit NClient(QWidget *parent = 0);
	~NClient();
	
private:
	Ui::NClient *ui;
	QSettings* _settings;
	QStackedWidget* _stack;

	QActionGroup* _mainBarActionGroup;
	QHash<QAction*, AbstractView*> _viewWidgets;
	AbstractView* _activeView;

	CommInterface* _connection;

	void initTooBar();
	void addView(QAction* action, AbstractView *widget);

	void closeEvent(QCloseEvent *);
	void saveSettings();
	void loadSettings();

private slots:
	void changeView();
};

#endif // NCLIENT_H
