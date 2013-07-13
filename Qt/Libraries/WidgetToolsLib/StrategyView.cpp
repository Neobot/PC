#include "StrategyView.h"
#include <QHBoxLayout>
#include "StrategyScene.h"

#include <QtDebug>

using namespace Tools;

StrategyView::StrategyView(QWidget *parent) :
	QGraphicsView(parent), _zoomScale(1.0), _scene(0)
{
	reset();
	setBackgroundBrush(Qt::gray);
	setCacheMode(QGraphicsView::CacheBackground);
	setViewportUpdateMode(QGraphicsView::NoViewportUpdate);

	connect(&_timer, SIGNAL(timeout()), this, SLOT(refresh()));
	_timer.setInterval(500);
}

void StrategyView::wheelEvent(QWheelEvent *event)
{

	if ((event->delta() / 240.0) > 0)
		zoomIn();
	else
		zoomOut();
}

void StrategyView::mousePressEvent(QMouseEvent* event)
{
	Q_UNUSED(event);
	//scene()->removeItem(scene()->itemAt(mapToScene(event->pos())));
}

void StrategyView::zoomIn()
{
	scaleViewBy(ZOOM_STEP);
}

void StrategyView::zoomOut()
{
	scaleViewBy(1/ZOOM_STEP);
}

void StrategyView::scaleViewBy(qreal scaleFactor)
{
	qreal nextFactor = calculateScaleFactor(matrix().scale(scaleFactor, scaleFactor));
	//if (nextFactor < ZOOM_LOWER_LIMIT || nextFactor > ZOOM_UPPER_LIMIT)
	//  return;

	scale(scaleFactor, scaleFactor);
	_zoomScale = nextFactor;
}

void StrategyView::scaleViewTo(qreal absoluteScaleFactor)
{
	qreal scale = absoluteScaleFactor/_zoomScale;
	scaleViewBy(scale);
}

qreal StrategyView::calculateScaleFactor(const QMatrix& matrix)
{
	qreal factor;
	matrix.map(1.0, 1.0, &factor, &factor);
	return factor;
}

void StrategyView::resizeEvent(QResizeEvent *event)
{
	Q_UNUSED(event);
	fitInView(sceneRect(), Qt::KeepAspectRatio);
}

StrategyScene * StrategyView::getScene() const
{
	return _scene;
}

void StrategyView::reset()
{
	delete _scene;
	_scene = new StrategyScene(this);
	setScene(_scene);
}

void StrategyView::refresh()
{
	_scene->refresh();
	viewport()->update();
}

void StrategyView::setRefreshInterval(int value)
{
	_timer.setInterval(value);
}

void StrategyView::setRefreshEnabled(bool value)
{
	if (value)
	{
		refresh();
		_timer.start();
	}
	else
		_timer.stop();
}
