#ifndef NGRID_H
#define NGRID_H

#include <QList>
#include <QHash>
#include <QPointF>
#include <QSizeF>
#include "AbstractLogger.h"
#include "NGridArea.h"

namespace Tools
{
    class NGridNode
    {
    public:
        NGridNode(const QPointF& position);
        virtual ~NGridNode();

        void addReachableNode(NGridNode* node, int costToGo = 1);
        void removeReachableNode(NGridNode* node);
        void removeAllConnections();

        void setReachableNodeCostToGo(NGridNode* node, int cost);
        int getCostToGo(NGridNode* node) const;

        const QPointF& getPosition() const;
        void setPosition(const QPointF& position);

        const QHash<NGridNode*, int>& getReachableNodes() const;
        const QList<NGridNode*>& getRevertedConnections() const;

    private:
        QPointF _pos;
        QHash<NGridNode*, int> _reachableNodes;
        QList<NGridNode*> _revertedLinks;
    };

    class NGrid
    {
    public:
        enum StandardGridConnectionType
        {
            NoConnection = 0,
            FourConnections = 4,
            HeightConnections = 8
        };

        NGrid();
        virtual ~NGrid();

        void makeStandardGrid(const QPointF& origin, double dx, double dy, const QSizeF& size, StandardGridConnectionType connectionType);
        bool readFromFile(const QString& filename, bool reverse = false);
        bool writeToFile(const QString& filename);

        void addNode(NGridNode* node);
        NGridNode* addNode(const QPointF& position);
        void removeNode(NGridNode* node);

        int indexOf(NGridNode* node) const;
        bool contains(NGridNode* node) const;

        void clear();

        void addBidirectionalLink(NGridNode* n1, NGridNode* n2);
        void removeBidirectionalLink(NGridNode* n1, NGridNode* n2);

        void setGlobalCost(int cost);

        void addAlias(NGridNode* node, const QString& alias);
        void removeAlias(NGridNode* node);
        QString getAlias(NGridNode* node);
        QStringList getAllAliases() const;

		NGridNode* getNode(const QString& alias) const;
        const QList<NGridNode*>& getNodes() const;

        NGridNode* getNearestNode(const QPointF& position) const;
        bool nodesAreAligned(NGridNode *c1, NGridNode *c2, NGridNode *c3) const;

        void addArea(const QRectF& rect, const QString& name);
		NGridArea* getArea(const QString& name) const;
        void updateAreas();
        const QHash<QString, NGridArea*>& getAreas() const;
        NGridArea *removeArea(const QString& name);
        void renameArea(const QString& oldName, const QString& newName);

    private:
        QHash<QString, NGridNode*> _alias;
        QList<NGridNode*> _nodes;
        QHash<QString, NGridArea*> _areas;

        enum NGRID_FILE_FORMAT_NODES
        {
            NODE_INDEX,
            NODE_X,
            NODE_Y,
            NODE_ALIAS,
            NODES_NB_FIELDS
        };

        enum NGRID_FILE_FORMAT_LINKS
        {
            LINK_START_NODE_INDEX,
            LINK_END_NODE_INDEX,
            COST_TO_GO,
            LINKS_NB_FIELDS
        };

        enum NGRID_FILE_FORMAT_AREAS
        {
            AREA_POS_X,
            AREA_POS_Y,
            AREA_WIDTH,
            AREA_HEIGHT,
            AREA_NAME,
            AREA_NB_FIELDS
        };

        static const char* NODE_SECTION_TITLE;
        static const char*  AREAS_SECTION_TITLE;
        static const char* LINNKS_SECTION_TITLE;
    };

    AbstractLogger& operator<<(AbstractLogger& logger, const NGridNode& node);

}

#endif // NGRID_H
