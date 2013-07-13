#ifndef NODEITEM_H
#define NODEITEM_H

#include <QGraphicsObject>
#include <QRectF>
#include "NGrid.h"
#include "SceneTools.h"

class LinkItem;

class NodeItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit NodeItem(Tools::NGridNode* node, int radius, QGraphicsItem *parent = 0);

    enum { Type = UserType + 2 };
    int type() const { return Type; }

    QRectF boundingRect() const;
    Tools::NGridNode* getNode() const;

    void setMode(SceneTools::Mode mode);
    void updatePositionFromNode();

    void setAlias(const QString& alias);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    Tools::NGridNode* _node;
    QRectF _rect;
    QString _alias;
    int _radius;
    SceneTools::Mode _mode;

    void computeRectSize(int radius);

signals:
    void positionChanged(NodeItem* node, const QPointF& oldPosition, const QPointF& newPosition);
};

#endif // NODEITEM_H
