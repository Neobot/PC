#include "GridView.h"
#include "GridScene.h"
#include "NodeItem.h"

#include <QtDebug>

GridView::GridView(QWidget *parent) :
    QGraphicsView(parent), _zoomScale(1.0), _mode(SceneTools::SelectionMode), _areaInConstruction(false), _areaConstructionAsked(false)
{
    setBackgroundBrush(Qt::gray);
    setCacheMode(QGraphicsView::CacheBackground);
    setDragMode(QGraphicsView::RubberBandDrag);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

void GridView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::CTRL)
    {
        if ((event->delta() / 240.0) > 0)
            zoomIn();
        else
            zoomOut();
    }
    else
        QGraphicsView::wheelEvent(event);
}

void GridView::zoomIn()
{
    scaleViewBy(ZOOM_STEP);
}

void GridView::zoomOut()
{
    scaleViewBy(1/ZOOM_STEP);
}

void GridView::scaleViewBy(qreal scaleFactor)
{
    qreal nextFactor = calculateScaleFactor(matrix().scale(scaleFactor, scaleFactor));
    if (nextFactor < ZOOM_LOWER_LIMIT || nextFactor > ZOOM_UPPER_LIMIT)
        return;

    scale(scaleFactor, scaleFactor);
    _zoomScale = nextFactor;
}

void GridView::scaleViewTo(qreal absoluteScaleFactor)
{
    qreal scale = absoluteScaleFactor/_zoomScale;
    scaleViewBy(scale);
}

qreal GridView::calculateScaleFactor(const QMatrix& matrix)
{
    qreal factor;
    matrix.map(1.0, 1.0, &factor, &factor);
    return factor;
}

void GridView::mouseMoveEvent(QMouseEvent *event)
{
    QPointF clikedPosInScene = mapToScene(event->pos());
    if (_mode == SceneTools::ConnectionMode)
    {
        qobject_cast<GridScene*>(scene())->updateConstructionLinkPosition(clikedPosInScene);
    }
    else if (_mode == SceneTools::AreaMode)
    {
        if (!_areaInConstruction && _areaConstructionAsked)
        {
            if ((_areaConstructionOrigin - clikedPosInScene).manhattanLength() > 50)
            {
                qobject_cast<GridScene*>(scene())->startAreaCreation(_areaConstructionOrigin);
                _areaInConstruction = true;
            }
        }
        if (_areaInConstruction)
        {
            qobject_cast<GridScene*>(scene())->continueAreaCreation(clikedPosInScene);
        }
    }

    QGraphicsView::mouseMoveEvent(event);
}

void GridView::mousePressEvent(QMouseEvent *event)
{
    QPointF clikedPosInScene = mapToScene(event->pos());
    if (_mode == SceneTools::ConnectionMode)
    {
        qobject_cast<GridScene*>(scene())->constructLink(clikedPosInScene);
    }
    else if (_mode == SceneTools::AreaMode && event->modifiers() == Qt::CTRL)
    {
        _areaConstructionAsked = true;
        _areaConstructionOrigin = clikedPosInScene;
    }
    else
        QGraphicsView::mousePressEvent(event);
}

void GridView::mouseReleaseEvent(QMouseEvent *event)
{
    QPointF clikedPosInScene = mapToScene(event->pos());
    if (_areaInConstruction && _mode == SceneTools::AreaMode)
    {
        qobject_cast<GridScene*>(scene())->stopAreaCreation(clikedPosInScene);
        _areaInConstruction = false;
    }

    _areaConstructionAsked = false;

    QGraphicsView::mouseReleaseEvent(event);
}

void GridView::setMode(SceneTools::Mode mode)
{
    _mode = mode;
    if (mode == SceneTools::ConnectionMode)
        viewport()->setCursor(Qt::CrossCursor);
    else
		viewport()->unsetCursor();
}

qreal GridView::getScaleFactor() const
{
	return _zoomScale;
}

void GridView::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        if (_mode == SceneTools::ConnectionMode)
        {
            GridScene* gridScene = qobject_cast<GridScene*>(scene());
            if (gridScene->constructionInProgress())
                gridScene->cancelConstruction();
            else
                emit cancelMode();
        }
        else if (_mode == SceneTools::AreaMode)
            emit cancelMode();
    }

    QGraphicsView::keyReleaseEvent(event);
}

void GridView::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);

    QPainter painter(viewport());
    QPen pen(Qt::black);
    pen.setWidth(3);
    painter.setPen(pen);

    QPoint origin(QPoint(5, 5));
    QPoint x(origin + QPoint(0, 50));
    QPoint y(origin + QPoint(50, 0));

    QPolygon arrowX;
    arrowX.append(x);
    arrowX.append(x + QPoint(-3, -5));
    arrowX.append(x + QPoint(3, -5));

    QPolygon arrowY;
    arrowY.append(y);
    arrowY.append(y + QPoint(-5, -3));
    arrowY.append(y + QPoint(-5, 3));

    painter.drawLine(origin, x);
    painter.drawLine(origin, y);
    painter.drawConvexPolygon(arrowX);
    painter.drawConvexPolygon(arrowY);

    painter.drawText(QRect(x + QPoint(10, -15), QSize(20, 20)), "x");
    painter.drawText(QRect(y + QPoint(-10, 5), QSize(20, 20)), "y");
}


