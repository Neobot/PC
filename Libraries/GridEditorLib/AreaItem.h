#ifndef AREAITEM_H
#define AREAITEM_H

#include <QGraphicsItem>
#include <QList>
#include <NGridArea.h>
#include "SceneTools.h"

/**
* \brief Ddefines a Block that can be resized.
*
* Height ResizingGrip allow resizing of the Block. They are only visible when the block is selected.
*/
class AreaItem : public QGraphicsObject
{
    Q_OBJECT

    friend class ResizingGrip;

    static const int MARGIN = 10;
    static const int SIZE   = 10;

    typedef QList<QGraphicsItem*> ResizingGrips;

public:
    AreaItem(const QString& name, const Tools::NGridArea& area, QGraphicsItem* parent = 0);

    enum { Type = UserType + 3 };
    int type() const { return Type; }

    const QString& getName() const;
    void setName(const QString& name);

    void setSize(const QSizeF& size);

    void refreshFromArea(const Tools::NGridArea& area);
    void updateGridArea(Tools::NGridArea& area);

    /**
        * Updates the position of each ResizingGrips arround the main rectangle.
        */
    void computeResizingGrips();

    QRectF boundingRect() const;

    /**
        * Returns a shape containing the main rectangle and its ResizingGrips.
        */
    virtual QPainterPath shape() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    const QRectF& getRect() const;

    void setMode(SceneTools::Mode mode);

signals:
    void areaChanged(AreaItem* item, const QPointF& oldPoint, const QSizeF& oldSize, const QPointF& newPoint, const QSizeF& newSize);

private:
    /**
        * Shows/hides ResizingGrips.
        */
    void setResizingGripsVisible(bool visible);

    /**
        * Returns whether the given point is contained in resizing area
        */
    bool isInResizeZone(QPointF point);

    /**
        * Set the given rect as the new main rectangle and places all ResizingGrips arround it.
        */
    void updateMainDiagram(const QRectF& rect);

    /**
        * Returns the resizing area of this item.
        */
    QPolygonF buildResizingArea() const;

    void	  initAreaItem();
    QVariant  itemChange(GraphicsItemChange change, const QVariant &value);

    QString _name;
    QRectF _rect;
    ResizingGrips _resizingGrips;
    SceneTools::Mode _mode;
};

/**
* \brief Defines a graphical item used to resize a AreaItem.
*
* A ResizingGrip can be placed on height fixed position arround the AreaItem. Moving a ResizingGrip
* with the mouse will upscale or downsclae the AreaItem.
*/
class ResizingGrip : public QGraphicsItem
{
public:
    enum POSITION {TOPLEFT, TOPCENTER, TOPRIGHT, RIGHTCENTER, BOTTOMRIGHT, BOTTOMCENTER, BOTTOMLEFT, LEFTCENTER};

    ResizingGrip(POSITION position, int size, AreaItem* areaItem);

    enum { Type = UserType + 4 };
    int type() const { return Type; }

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
    virtual QRectF boundingRect() const;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

private:
    int			_size;
    POSITION		_position;
    AreaItem*           _areaItem;

    void setPositionCursor();
};

#endif // AREAITEM_H
