#ifndef LINKITEM_H
#define LINKITEM_H

#include <QGraphicsLineItem>
#include "NGrid.h"
#include "SceneTools.h"

class LinkItem : public QGraphicsLineItem
{
public:
    LinkItem(Tools::NGridNode* node1, Tools::NGridNode* node2, SceneTools::ConnectionType connectionType, QGraphicsItem *parent = 0);

    void setConnectionType(SceneTools::ConnectionType connectionType);

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    QRectF boundingRect() const;

    Tools::NGridNode* getN1() const;
    Tools::NGridNode* getN2() const;

    SceneTools::ConnectionType getConnectionType() const;

    void setColor(const QColor& color);
    void updatePosition();

 protected:
     void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
     Tools::NGridNode* _n1;
     Tools::NGridNode* _n2;
     SceneTools::ConnectionType _connectionType;
     QColor _color;

     QPolygonF buildArrow(const QLineF& line, const QPointF& p, qreal arrowSize, bool fromP1toP2);


};

#endif // LINKITEM_H
