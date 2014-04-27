#ifndef NGRIDAREA_H
#define NGRIDAREA_H

#include <QRectF>
#include <QStringList>
#include <QHash>

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


	class AreaLocker
	{
	public:
		AreaLocker(Tools::NGrid* grid) : _grid(grid) {}
		void lockArea(const QString& area, int costToGoInside, int costToGoOutside, int internalCost);
		void unlockArea(const QString& area);

	private:
		struct AreaLockingValues
		{
			int costToGoInside;
			int costToGoOutside;
			int internalCost;
			int oldCostToGoInside;
			int oldCostToGoOutside;
			int oldInternalCost;
		};

		Tools::NGrid* _grid;
		QHash<QString, AreaLockingValues> _lockedAreas;
		QStringList _areaList;

		void setAreaValues(const QString& area);
	};
}

#endif // NGRIDAREA_H
