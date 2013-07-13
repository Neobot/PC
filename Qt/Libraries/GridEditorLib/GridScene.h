#ifndef GRIDSCENE_H
#define GRIDSCENE_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <NGrid.h>
#include "SceneTools.h"

class LinkItem;
class NodeItem;
class AreaItem;

uint qHash(const QPointF& key);

class GridScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GridScene(Tools::NGrid* grid, QObject *parent = 0);
    ~GridScene();

    Tools::NGrid* getGrid() const;
    void setGrid(Tools::NGrid* grid);
    void reloadGrid();

    void setBackground(const QPixmap& pixmap);
    QPixmap getBackground() const;

    void selectAll();
    void reselect();
    QList<NodeItem*> getSelectedNodes() const;
    NodeItem* getFirstNodeAt(const QPointF& position);

    void ensureLinkRemoved(Tools::NGridNode* n1, Tools::NGridNode* n2);

    void setMode(SceneTools::Mode mode);

    void constructLink(const QPointF& position);
    void updateConstructionLinkPosition(const QPointF& position);
    void cancelConstruction();
    bool constructionInProgress() const;

    void addNodeItem(Tools::NGridNode* node);

    void removeArea(AreaItem* area);
    void addArea(AreaItem* area);
    void updateAreas();
    void startAreaCreation(const QPointF& point);
    void continueAreaCreation(const QPointF& point);
    void stopAreaCreation(const QPointF& point);

    AreaItem* constructAreaItem(const QString& name, const Tools::NGridArea* area);

private:
    Tools::NGrid* _grid;
    QGraphicsPixmapItem* _background;
    QHash<QPointF, QHash<QPointF, LinkItem*> > _selectedLinkLines;
    QList<AreaItem*> _areaItemList;
    SceneTools::Mode _mode;

    LinkItem* _constructionLinkItem;
    Tools::NGridNode* _constructionStartNode;
    bool _constructionInProgress;
    Tools::NGridNode* _constructionEndNode;

    NodeItem* _previouslyConstructedNodeItem;
    QGraphicsRectItem* _areaCreatorItem;
    QPointF            _areaCreatorOrigin;


    NodeItem* constructNodeItem(Tools::NGridNode* node, int radius = 4);

    void showNodeLinks(const QList<Tools::NGridNode*> nodes, int level, QList<QColor>& selectionColors);
    void showNodeLink(Tools::NGridNode* n1, Tools::NGridNode* n2, const QColor& color);
    void clearTemporaryLinks();

private slots:
     void selectionChanged();
     void nodeMoved(NodeItem* node, const QPointF& oldPosition, const QPointF& newPosition);

signals:
     void nodeMoved(QList<QGraphicsItem*> selectedNodes, NodeItem* focusedNode, const QPointF& oldPosition, const QPointF& newPosition);
     void nodeCreated(NodeItem* createdNode, NodeItem* previousNode);
     void areaChanged(AreaItem* item, const QPointF& oldPoint, const QSizeF& oldSize, const QPointF& newPoint, const QSizeF& newSize);
     void areaCreated(AreaItem* item, const QPointF& topLeft, const QSizeF& size);
};

#endif // GRIDSCENE_H
