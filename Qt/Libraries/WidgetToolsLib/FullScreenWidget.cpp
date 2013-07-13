#include "FullScreenWidget.h"

#include <QKeyEvent>

using namespace Tools;

FullScreenWidget::FullScreenWidget() :
    QWidget(0), _isFullScreen(false), _w(0)
{
    _layout = new QHBoxLayout(this);
    _layout->setMargin(0);
}

void FullScreenWidget::setFullScreen(QWidget *widget)
{
    if (!_isFullScreen)
    {
		_w = widget;
		_layout->addWidget(_w);
		_isFullScreen = true;
		showFullScreen();
    }
}


void FullScreenWidget::stopFullScreen()
{
    _isFullScreen = false;
    _layout->removeWidget(_w);
    emit fullScreenStoped(_w);
    showNormal();
    hide();
    _w = 0;
}

void FullScreenWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape && _isFullScreen)
    {
	stopFullScreen();
    }
}

FullScreenWidget *FullScreenWidget::instance()
{
    static FullScreenWidget instance;

    return &instance;
}
