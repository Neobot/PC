#include "AreaItem.h"
#include "SceneTools.h"

#include <QPainter>
#include <QCursor>
#include <QGraphicsSceneHoverEvent>
#include <QPalette>
#include <QApplication>
#include <QGraphicsScene>

AreaItem::AreaItem(const QString &name, const Tools::NGridArea &area, QGraphicsItem *parent)
    : QGraphicsObject(parent), _name(name)
{
    refreshFromArea(area);
    initAreaItem();
    setMode(SceneTools::SelectionMode);
}

const QString &AreaItem::getName() const
{
    return _name;
}

void AreaItem::setName(const QString &name)
{
    _name = name;
}

void AreaItem::setSize(const QSizeF &size)
{
    _rect.setSize(size);
    computeResizingGrips();
}

void AreaItem::refreshFromArea(const Tools::NGridArea& area)
{
    _rect = QRectF(QPointF(0, 0), SceneTools::mapWithStandardReference(area.getRect().size()));
    setPos(SceneTools::mapWithStandardReference(area.getRect().topLeft()));
}

void AreaItem::updateGridArea(Tools::NGridArea &area)
{
    QRectF r(SceneTools::mapWithStandardReference(pos()), SceneTools::mapWithStandardReference(_rect.size()));
    area.setRect(r);
}

void AreaItem::setMode(SceneTools::Mode mode)
{
    _mode = mode;
    if (_mode == SceneTools::AreaMode)
    {
        setFlag(QGraphicsItem::ItemIsMovable, true);
        setFlag(QGraphicsItem::ItemIsSelectable, true);
        setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);

        setCursor(Qt::SizeAllCursor);
        setZValue(1000);
    }
    else
    {
        setFlag(QGraphicsItem::ItemIsMovable, false);
        setFlag(QGraphicsItem::ItemIsSelectable, false);
        setFlag(QGraphicsItem::ItemSendsScenePositionChanges, false);

        setCursor(_mode == SceneTools::SelectionMode ? Qt::ArrowCursor : Qt::CrossCursor);
        setZValue(-500);
    }
}


void AreaItem::initAreaItem()
{
    for (int i = 0; i < 8; ++i)
        _resizingGrips << new ResizingGrip((ResizingGrip::POSITION)i, SIZE, this);

    computeResizingGrips();

    setResizingGripsVisible(false);
}

QVariant AreaItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged)
    {
        setResizingGripsVisible(value.toBool());
    }
    else if (change == ItemPositionChange && scene())
    {
        prepareGeometryChange();
        computeResizingGrips();

        // value is the new position.
        QPointF newPos = value.toPointF();
        QPointF oldPos = scenePos();

        //_rect.setTopLeft(newPos);

        emit areaChanged(this, oldPos, _rect.size(), newPos, _rect.size());

    }
    return QGraphicsItem::itemChange(change, value);
}

bool AreaItem::isInResizeZone(QPointF point)
{	
    return buildResizingArea().containsPoint(mapFromScene(point), Qt::OddEvenFill);
}

void AreaItem::setResizingGripsVisible(bool visible)
{
    for (int i = 0; i < 8; ++i)
        _resizingGrips[i]->setVisible(visible);
}

void AreaItem::computeResizingGrips()
{
    _resizingGrips[ResizingGrip::TOPLEFT	 ]->setPos(_rect.topLeft      ());
    _resizingGrips[ResizingGrip::TOPRIGHT	 ]->setPos(_rect.topRight     ());
    _resizingGrips[ResizingGrip::BOTTOMLEFT	 ]->setPos(_rect.bottomLeft   ());
    _resizingGrips[ResizingGrip::BOTTOMRIGHT     ]->setPos(_rect.bottomRight  ());
    _resizingGrips[ResizingGrip::TOPCENTER	 ]->setPos(QPointF(_rect.left ()+(_rect.width() / 2.0), _rect.top   ()));
    _resizingGrips[ResizingGrip::BOTTOMCENTER    ]->setPos(QPointF(_rect.left ()+(_rect.width() / 2.0), _rect.bottom()));
    _resizingGrips[ResizingGrip::LEFTCENTER	 ]->setPos(QPointF(_rect.left (), _rect.top()+(_rect.height() / 2.0)));
    _resizingGrips[ResizingGrip::RIGHTCENTER     ]->setPos(QPointF(_rect.right(), _rect.top()+(_rect.height() / 2.0)));
}

void AreaItem::updateMainDiagram(const QRectF& rect)
{
    QPointF oldPos = pos();
    QSizeF oldSize = _rect.size();

    prepareGeometryChange();
    _rect = QRectF(QPointF(0, 0), rect.size());
    setPos(rect.topLeft());
    computeResizingGrips();
    update();

    emit areaChanged(this, oldPos, oldSize, pos(), _rect.size());
}

QPolygonF AreaItem::buildResizingArea() const
{
	//int size = MARGIN;
    return QPolygonF(_rect);//.subtracted(QPolygonF(_rect.adjusted(size, size, -size, -size)));
}

QRectF AreaItem::boundingRect() const
{
    int size = MARGIN*8;
    return _rect.adjusted(-size, -size, size, size);
}

QPainterPath AreaItem::shape() const
{
    QPainterPath path;

    path.addPolygon(buildResizingArea());
    
    return path;
}

void AreaItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
    painter->save();

    QColor brushColor = _mode == SceneTools::AreaMode ? Qt::yellow : QColor(255,253,246);
    QBrush brush(brushColor, Qt::BDiagPattern);
    painter->fillRect(_rect, brush);

    QPen p;
    p.setWidth(2);
    p.setColor(brushColor);
    p.setStyle(Qt::DashLine);
    if (isSelected())
    {
        p.setColor(Qt::darkRed);
    }
    painter->setPen(p);
    painter->drawRect(_rect);

    if (_mode == SceneTools::AreaMode)
    {
        p.setColor(Qt::black);
        painter->setPen(p);

        QFont f = painter->font();
        f.setPointSize(30);
        painter->setFont(f);

        QRectF textRect(_rect.topLeft() + QPointF(5, 1), _rect.size());
        painter->drawText(textRect, _name);
    }

    painter->restore();
}

const QRectF &AreaItem::getRect() const
{
    return _rect;
}

//-------------------------

ResizingGrip::ResizingGrip(POSITION position, int size, AreaItem* areaItem) : QGraphicsItem(areaItem),
    _size(size), _position(position), _areaItem(areaItem)
{	
    setAcceptHoverEvents(true);
    setPositionCursor();
}

void ResizingGrip::setPositionCursor()
{
    switch(_position)
    {
        case TOPLEFT:
            setCursor(QCursor(Qt::SizeFDiagCursor));
            break;
        case TOPCENTER:
            setCursor(QCursor(Qt::SizeVerCursor));
            break;
        case TOPRIGHT:
            setCursor(QCursor(Qt::SizeBDiagCursor));
            break;
        case LEFTCENTER:
            setCursor(QCursor(Qt::SizeHorCursor));
            break;
        case RIGHTCENTER:
            setCursor(QCursor(Qt::SizeHorCursor));
            break;
        case BOTTOMLEFT:
            setCursor(QCursor(Qt::SizeBDiagCursor));
            break;
        case BOTTOMCENTER:
            setCursor(QCursor(Qt::SizeVerCursor));
            break;
        case BOTTOMRIGHT:
            setCursor(QCursor(Qt::SizeFDiagCursor));
            break;
        default:
            Q_ASSERT(false);
    }
}

QRectF ResizingGrip::boundingRect() const
{
    return QRectF (-_size, -_size, _size*2, _size*2);
}

void ResizingGrip::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

    QPen pen(QColor("white"));
    QBrush brush(qApp->palette().color(QPalette::Highlight));
    painter->setPen(pen);
    painter->setBrush(brush);
    QRectF rect(-_size/2.0, -_size/2.0, _size, _size);
    painter->drawRect(rect);
}

void ResizingGrip::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	Q_UNUSED(event);
    //_AreaItem->shrinkToTranslationBoundaryRect();
}

void ResizingGrip::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    // ==========================================================================================
    // VERSION 1: Resize AreaItem and then move it to 0,0
    // ==========================================================================================

    QRectF  resizableZone = _areaItem->getRect().translated(_areaItem->scenePos());
    QPointF scenePos = event->scenePos();

    switch(_position)
    {
        case TOPLEFT:
            resizableZone.setTopLeft(scenePos);
            break;
        case TOPCENTER:
            resizableZone.setTop(scenePos.y());
            break;
        case TOPRIGHT:
            resizableZone.setTopRight(scenePos);
            break;
        case LEFTCENTER:
            resizableZone.setLeft(scenePos.x());
            break;
        case RIGHTCENTER:
            resizableZone.setRight(scenePos.x());
            break;
        case BOTTOMLEFT:
            resizableZone.setBottomLeft(scenePos);
            break;
        case BOTTOMCENTER:
            resizableZone.setBottom(scenePos.y());
            break;
        case BOTTOMRIGHT:
            resizableZone.setBottomRight(scenePos);
            break;
        default:
            Q_ASSERT(false);
    }

    //resizableZone.moveTopLeft(QPointF(0, 0));
    _areaItem->updateMainDiagram(resizableZone);

    event->accept();
}
