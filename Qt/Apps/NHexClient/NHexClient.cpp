#include "NHexClient.h"
#include "ui_NHexClient.h"
#include "ConnectionView.h"

#include <QDockWidget>


NClient::NClient(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::NClient), _activeView(0)
{
	qApp->setOrganizationName("Neobot");
	qApp->setApplicationName("Hexapod Client");

	ui->setupUi(this);
	setWindowIcon(QIcon(":/app/AppIcon"));
	setWindowState(Qt::WindowMaximized);

	_stack = new QStackedWidget(this);
	setCentralWidget(_stack);

	_connection = new CommInterface;

	initTooBar();
	changeView();

	_settings = new QSettings(QSettings::UserScope, "Neobot", "HexClient", this);
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

	action = ui->mainToolBar->addAction(QIcon(":/toolbar/connection"), "Connection", this, SLOT(changeView()));
	_activeView = new ConnectionView(_connection, this);
	addView(action, _activeView);
	action->setChecked(true);
	action->setShortcut(QKeySequence(Qt::Key_F9));
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

	//_settings->setValue("LogOpened", _logAction->isChecked());

	foreach(AbstractView* view, _viewWidgets)
		view->saveSettings(_settings);

	//_logView->saveSettings(_settings);
}

void NClient::loadSettings()
{
	restoreGeometry(_settings->value("MainWindowGeometry").toByteArray());
	restoreState(_settings->value("MainWindowState").toByteArray());

	//_logAction->setChecked(_settings->value("LogOpened", true).toBool());

	foreach(AbstractView* view, _viewWidgets)
		view->loadSettings(_settings);

	//_logView->loadSettings(_settings);
}
