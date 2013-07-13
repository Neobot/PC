#include "NodeItem.h"
#include "LinkItem.h"
#include "SceneTools.h"

#include <QPainter>
#include <QtDebug>
#include <QCursor>

NodeItem::NodeItem(Tools::NGridNode* node, int radius, QGraphicsItem *parent) :
    QGraphicsObject(parent), _node(node), _radius(radius)
{
    computeRectSize(_radius);
    updatePositionFromNode();

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);

    setMode(SceneTools::SelectionMode);
}

void NodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

    painter->save();

    QColor c(Qt::black);

    if (!_alias.isEmpty())
        c = Qt::yellow;

    if (isSelected())
        c = Qt::red;

    painter->fillRect(_rect, c);
    if (!_alias.isEmpty())
    {
        QPen p = painter->pen();
        p.setWidth(_radius/2);
        painter->setPen(p);
        painter->drawRect(_rect);
    }
//    if (isSelected() && !_alias.isEmpty())
//    {
//        QFont f = painter->font();
//        f.setPixelSize(50);
//        painter->setFont(f);
//        QFontMetrics metrics(painter->font());
//        QRectF textRect;
//        textRect.setWidth(metrics.width(_alias));
//        textRect.setHeight(metrics.height());
//        textRect.moveCenter(_rect.center());
//        textRect.translate(0, -textRect.height());

//        painter->drawText(textRect, _alias);
//    }

    painter->restore();
 }

QRectF NodeItem::boundingRect() const
{
    return _rect.adjusted(-5, -5, 5, 5);
}

Tools::NGridNode * NodeItem::getNode() const
{
    return _node;
}

QVariant NodeItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene())
    {
        // value is the new position.
        QPointF newPos = SceneTools::mapWithStandardReference(value.toPointF());
        QPointF oldPos = SceneTools::mapWithStandardReference(scenePos());

        _node->setPosition(newPos);

        emit positionChanged(this, oldPos, newPos);

    }
    return QGraphicsItem::itemChange(change, value);
}

void NodeItem::updatePositionFromNode()
{
    setPos(SceneTools::getNodeScenePosition(_node));
}

void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (_mode == SceneTools::SelectionMode)
        setCursor(Qt::ClosedHandCursor);

    QGraphicsItem::mousePressEvent(event);
}

void NodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (_mode == SceneTools::SelectionMode)
        setCursor(Qt::OpenHandCursor);

    QGraphicsItem::mouseReleaseEvent(event);
}

void NodeItem::setAlias(const QString &alias)
{
    _alias = alias;
    if (!_alias.isEmpty())
        computeRectSize(_radius * 2);
    else
        computeRectSize(_radius);
}

void NodeItem::computeRectSize(int radius)
{
    _rect = QRectF(-radius, -radius, 2*radius, 2*radius);
}

void NodeItem::setMode(SceneTools::Mode mode)
{
    _mode = mode;
    if (_mode == SceneTools::SelectionMode)
    {
        setFlag(QGraphicsItem::ItemIsMovable, true);
        setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
        setFlag(QGraphicsItem::ItemIsSelectable, true);

        setCursor(Qt::OpenHandCursor);
    }
    else
    {
        setFlag(QGraphicsItem::ItemIsMovable, false);
        setFlag(QGraphicsItem::ItemSendsScenePositionChanges, false);
        setFlag(QGraphicsItem::ItemIsSelectable, _mode == SceneTools::ConnectionMode ? true : false);

        setCursor(_mode == SceneTools::ConnectionMode ? Qt::CrossCursor : Qt::ArrowCursor);
    }
}
