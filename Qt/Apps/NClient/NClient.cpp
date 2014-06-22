#include "NClient.h"
#include "ui_NClient.h"
#include "ConnectionView.h"
#include "NetworkConnection.h"
#include "RemoteControlView.h"
#include "OdometryView.h"
#include "SensorsView.h"
#include "StrategiesView.h"
#include "LogView.h"
#include "ParametersView.h"
#include "Ax12View.h"
#include "GraphsView.h"
#include "ScriptView.h"

#include <QDockWidget>


NClient::NClient(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::NClient), _activeView(0)
{
	qApp->setOrganizationName("Neobot");
	qApp->setApplicationName("Desktop Client");

	ui->setupUi(this);
	setWindowIcon(QIcon(":/app/AppIcon"));
	setWindowState(Qt::WindowMaximized);

	_stack = new QStackedWidget(this);
	setCentralWidget(_stack);

	_connection = new NetworkConnection;

	initTooBar();
	changeView();

	_settings = new QSettings(QSettings::UserScope, "Neobot", "DesktopClient", this);
	loadSettings();
}

NClient::~NClient()
{
	delete _connection;
	delete ui;
}

void NClient::initTooBar()
{
	_mainBarActionGroup = new QActionGroup(this);
	_mainBarActionGroup->setExclusive(true);
	QAction* action = 0;

	action = ui->mainToolBar->addAction(QIcon(":/toolbar/control"), "Remote control", this, SLOT(changeView()));
	addView(action, new RemoteControlView(_connection, this));
	action->setChecked(false);
	action->setShortcut(QKeySequence(Qt::Key_F1));

	action = ui->mainToolBar->addAction(QIcon(":/toolbar/positions"), "Odometry", this, SLOT(changeView()));
	addView(action, new OdometryView(_connection, this));
	action->setChecked(false);
	action->setShortcut(QKeySequence(Qt::Key_F2));

	action = ui->mainToolBar->addAction(QIcon(":/toolbar/graph"), "Graphs", this, SLOT(changeView()));
	addView(action, new GraphsView(_connection, this));
	action->setChecked(false);
	action->setShortcut(QKeySequence(Qt::Key_F3));

	action = ui->mainToolBar->addAction(QIcon(":/toolbar/sharp"), "Sensors", this, SLOT(changeView()));
	addView(action, new SensorsView(_connection, this));
	action->setChecked(false);
	action->setShortcut(QKeySequence(Qt::Key_F4));

	action = ui->mainToolBar->addAction(QIcon(":/toolbar/ax12"), "AX-12", this, SLOT(changeView()));
	addView(action, new Ax12View(_connection, this));
	action->setChecked(false);
	action->setShortcut(QKeySequence(Qt::Key_F5));

	action = ui->mainToolBar->addAction(QIcon(":/toolbar/strategies"), "Strategies", this, SLOT(changeView()));
	addView(action, new StrategiesView(_connection, this));
	action->setChecked(false);
	action->setShortcut(QKeySequence(Qt::Key_F6));

	action = ui->mainToolBar->addAction(QIcon(":/toolbar/upload"), "Parameters", this, SLOT(changeView()));
	addView(action, new ParametersView(_connection, this));
	action->setChecked(false);
	action->setShortcut(QKeySequence(Qt::Key_F7));

	action = ui->mainToolBar->addAction(QIcon(":/toolbar/script"), "Script", this, SLOT(changeView()));
	addView(action, new ScriptView(_connection, this));
	action->setChecked(false);
	action->setShortcut(QKeySequence(Qt::Key_F8));

	action = ui->mainToolBar->addAction(QIcon(":/toolbar/connection"), "Connection", this, SLOT(changeView()));
	_activeView = new ConnectionView(_connection, this);
	addView(action, _activeView);
	action->setChecked(true);
	action->setShortcut(QKeySequence(Qt::Key_F9));

	ui->mainToolBar->addSeparator();

	_logView = new LogView(_connection, this);
	QDockWidget* logDock = new QDockWidget("Log", this);
	logDock->setWidget(_logView);
	addDockWidget(Qt::BottomDockWidgetArea, logDock);
	_logAction = ui->mainToolBar->addAction(QIcon(":/toolbar/log"), "Log");
	_logAction->setCheckable(true);
	_logAction->setChecked(true);
	_logAction->setShortcut(QKeySequence(Qt::Key_F8));
	connect(_logAction, SIGNAL(toggled(bool)), logDock, SLOT(setVisible(bool)));
}

void NClient::addView(QAction *action, AbstractView *widget)
{
	action->setCheckable(true);
	_viewWidgets[action] = widget;
	_stack->addWidget(widget);
	_mainBarActionGroup->addAction(action);
}

void NClient::changeView()
{
	AbstractView* viewWidget = _viewWidgets.value(_mainBarActionGroup->checkedAction());
	if (viewWidget)
	{
		_stack->setCurrentWidget(viewWidget);
		if (_activeView)
			_activeView->setActiveView(false);
		_activeView = viewWidget;
		_activeView->setActiveView(true);
	}
}


void NClient::closeEvent(QCloseEvent *e)
{
	saveSettings();

	QMainWindow::closeEvent(e);
}

void NClient::saveSettings()
{
	_settings->setValue("MainWindowGeometry", saveGeometry());
	_settings->setValue("MainWindowState", saveState());

	_settings->setValue("LogOpened", _logAction->isChecked());

	foreach(AbstractView* view, _viewWidgets)
		view->saveSettings(_settings);

	_logView->saveSettings(_settings);
}

void NClient::loadSettings()
{
	restoreGeometry(_settings->value("MainWindowGeometry").toByteArray());
	restoreState(_settings->value("MainWindowState").toByteArray());

	_logAction->setChecked(_settings->value("LogOpened", true).toBool());

	foreach(AbstractView* view, _viewWidgets)
		view->loadSettings(_settings);

	_logView->loadSettings(_settings);
}
