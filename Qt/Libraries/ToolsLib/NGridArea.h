#ifndef NGRIDAREA_H
#define NGRIDAREA_H

#include <QRectF>
#include <QList>

namespace Tools
{
    class NGrid;
    class NGridNode;

    class NGridArea
    {
        friend class NGrid;

    public:
        NGridArea();
        NGridArea(const QRectF& rect, NGrid* grid);

        bool isValid() const;

        const QList<NGridNode *> &getNodes() const;
        NGridNode* getNearestNode(const QPointF& position) const;

        void setInternalCost(int cost);
        void setCostToGoInside(int cost);
        void setCostToGoOutside(int cost);

        void setRect(const QRectF& rect);
        const QRectF& getRect() const;

        bool contains(NGridNode* node) const;
        bool contains(const QPointF& position) const;

        NGridNode* getCentralNode() const;

    private:
        QRectF _rect;
        QList<NGridNode*> _nodes;
        NGrid* _parentGrid;

        void update();

    };
}

#endif // NGRIDAREA_H
