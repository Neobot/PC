#include "LinkItem.h"

#include <cmath>

#include <QPainter>
#include <QVector2D>

#include <QtDebug>


const qreal pi = 3.14;

LinkItem::LinkItem(Tools::NGridNode* node1, Tools::NGridNode* node2, SceneTools::ConnectionType connectionType, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), _n1(node1), _n2(node2), _connectionType(connectionType), _color(Qt::black)
{
    updatePosition();
}

void LinkItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

    painter->save();
    //if (myStartItem->collidesWithItem(myEndItem))
    //         return;

    QPen myPen = pen();
    myPen.setColor(_color);
    qreal arrowSize = 8;
    painter->setPen(myPen);
    painter->setBrush(_color);


    updatePosition();

    QLineF centerLine = line();
    QLineF l1, l2;

    if (_connectionType == SceneTools::Both)
    {
        QLineF nlv = centerLine.normalVector().unitVector();
        QPointF nv = nlv.p1() - nlv.p2();
        l1 = centerLine.translated(nv*1.5);
        l2 = centerLine.translated(nv*-1.5);

        painter->drawLine(l1);
        painter->drawLine(l2);
    }
    else
        painter->drawLine(centerLine);

    QLineF uv = centerLine.unitVector();
    QPointF u = uv.p1() - uv.p2();

    if (_connectionType == SceneTools::Normal || _connectionType == SceneTools::Both)
    {
        QPointF a1 = centerLine.p1() - (centerLine.length()/3 + arrowSize/4) * u;
        QPolygonF arrowNormal = buildArrow(centerLine, a1, arrowSize, true);
        painter->drawPolygon(arrowNormal);
    }

    if (_connectionType == SceneTools::Reverted || _connectionType == SceneTools::Both)
    {
        QPointF a2 = centerLine.p2() + (centerLine.length()/3 + arrowSize/4) * u;
        QPolygonF arrowReversed = buildArrow(_connectionType == SceneTools::Both ? l2 : centerLine, a2, arrowSize, false);
        painter->drawPolygon(arrowReversed);
    }

//    if (isSelected()) {
//        painter->setPen(QPen(myColor, 1, Qt::DashLine));
//    QLineF myLine = line();
//    myLine.translate(0, 4.0);
//    painter->drawLine(myLine);
//    myLine.translate(0,-8.0);
//    painter->drawLine(myLine);
//    }

    painter->restore();
}

QPolygonF LinkItem::buildArrow(const QLineF& line, const QPointF& p, qreal arrowSize, bool fromP1toP2)
{

    QPointF arrowP1(-arrowSize, arrowSize/2);
    QPointF arrowP2(-arrowSize, -arrowSize/2);

    QPolygonF arrowHead;
    arrowHead << QPointF(0, 0) << arrowP1 << arrowP2;

    QTransform t;
    t.translate(p.x(), p.y());
    t.rotate(-line.angle());
    if (!fromP1toP2)
        t.rotate(180.0);

    return t.map(arrowHead);
}

QRectF LinkItem::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                               line().p2().y() - line().p1().y()))
            .normalized()
            .adjusted(-extra, -extra, extra, extra);
}

void LinkItem::setConnectionType(SceneTools::ConnectionType connectionType)
{
    _connectionType = connectionType;
}

Tools::NGridNode * LinkItem::getN1() const
{
    return _n1;
}

Tools::NGridNode * LinkItem::getN2() const
{
    return _n2;
}

void LinkItem::setColor(const QColor &color)
{
    _color = color;
}

SceneTools::ConnectionType LinkItem::getConnectionType() const
{
    return _connectionType;
}

void LinkItem::updatePosition()
{
    QLineF centerLine(SceneTools::getNodeScenePosition(_n1),
                      SceneTools::getNodeScenePosition(_n2));

    setLine(centerLine);
}
