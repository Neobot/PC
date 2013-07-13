#include "ViewWidget.h"
#include "ui_ViewWidget.h"

#include "FullScreenWidget.h"

#include <QMainWindow>

ViewWidget::ViewWidget(QWidget *parent, QWidget *view, QWidget* settings) :
	QWidget(parent),
	ui(new Ui::ViewWidget), _isFullScreen(false)
{
    ui->setupUi(this);

	_currentView = ui->pageView;
	if (view)
		setView(view);

	_currentSettings = ui->pageSetting;
	ui->btnSetting->setEnabled(settings);
	if (settings)
		setSettings(settings);

    connect(ui->btnFullscreen, SIGNAL(clicked()), this, SLOT(fullscreen()));
	connect(ui->btnSetting, SIGNAL(clicked()), this, SLOT(switchSettings()));
}

ViewWidget::~ViewWidget()
{
    delete ui;
}

void ViewWidget::setView(QWidget *widget)
{
	int index = ui->stackedWidget->currentIndex();

	ui->stackedWidget->removeWidget(_currentView);
    delete _currentView;
    _currentView = widget;
	ui->stackedWidget->insertWidget(0, _currentView);

	ui->stackedWidget->setCurrentIndex(index);
}

void ViewWidget::setSettings(QWidget *widget)
{
	int index = ui->stackedWidget->currentIndex();

	ui->stackedWidget->removeWidget(_currentSettings);
	delete _currentSettings;
	_currentSettings = widget;
	if (!_currentSettings)
		_currentSettings = new QWidget(this);
	ui->stackedWidget->insertWidget(1, _currentSettings);
	_currentView->setParent(this);

	ui->btnSetting->setEnabled(_currentSettings);

	ui->stackedWidget->setCurrentIndex(index);
}

QWidget *ViewWidget::getView() const
{
    return _currentView;
}

void ViewWidget::fullscreen()
{
	if (!_isFullScreen)
	{
		Tools::FullScreenWidget::instance()->setFullScreen(ui->baseFrame);
		connect(Tools::FullScreenWidget::instance(), SIGNAL(fullScreenStoped(QWidget*)), this, SLOT(fullscreenStoped(QWidget*)));
		ui->btnFullscreen->setText("Quit fullscreen (Esc)");
	}
	else
	{
		Tools::FullScreenWidget::instance()->stopFullScreen();
	}
}

void ViewWidget::fullscreenStoped(QWidget *w)
{
    _isFullScreen = false;
    ui->mainLayout->insertWidget(0, w);
	disconnect(Tools::FullScreenWidget::instance(), SIGNAL(fullScreenStoped(QWidget*)), this, SLOT(fullscreenStoped(QWidget*)));

    ui->btnFullscreen->setText("FullScreen");
}

void ViewWidget::switchSettings()
{
	if (!ui->stackedWidget->isActive())
		ui->stackedWidget->slideInIndex(ui->stackedWidget->currentIndex() == 0 ? 1 : 0);
	else
	{
		ui->btnSetting->blockSignals(true);
		ui->btnSetting->toggle();
		ui->btnSetting->blockSignals(false);
	}
}


