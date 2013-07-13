#include "GridScene.h"
#include "LinkItem.h"
#include "NodeItem.h"
#include "AreaItem.h"

#include <QtDebug>
#include <QCursor>

using namespace Tools;

const int POINT_RADIUS = 4;

uint qHash(const QPointF& key)
{
    return qHash((int)(key.x() * 1000.0)) ^ qHash((int)(key.y() * 1000.0));
}


GridScene::GridScene(Tools::NGrid* grid, QObject *parent) :
    QGraphicsScene(parent), _background(0)
{
    connect(this, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
    setGrid(grid);

    _constructionInProgress = false;
    _constructionStartNode = new NGridNode(QPointF());
    _constructionEndNode = new NGridNode(QPointF());
    _constructionLinkItem = new LinkItem(_constructionStartNode, _constructionEndNode, SceneTools::Both);
    QColor constructionColor = Qt::darkYellow;
    constructionColor.setAlpha(150);
    _constructionLinkItem->setColor(constructionColor);

    _previouslyConstructedNodeItem = 0;

    _areaCreatorItem = new QGraphicsRectItem;
    //_areaCreatorItem->setPen(Qt::black);
    _areaCreatorItem->setBrush(QBrush(Qt::BDiagPattern));
}

GridScene::~GridScene()
{
    cancelConstruction();

    delete _constructionStartNode;
    delete _constructionEndNode;
    delete _constructionLinkItem;
}

void GridScene::setGrid(Tools::NGrid *grid)
{
    _grid = grid;
    reloadGrid();
}

void GridScene::reloadGrid()
{
    if (!_grid)
        return;

    clearSelection();
    _selectedLinkLines.clear();

    if (_background)
        removeItem(_background);

    clear();

    if (_background)
        addItem(_background);

    foreach(NGridNode* node, _grid->getNodes())
        addNodeItem(node);

    _areaItemList.clear();
    for(QHash<QString, NGridArea*>::const_iterator it = _grid->getAreas().constBegin(); it != _grid->getAreas().constEnd(); ++it)
    {
        const NGridArea* area = *it;
        AreaItem* item = constructAreaItem(it.key(), area);
        addArea(item);
    }
}

void GridScene::addNodeItem(Tools::NGridNode* node)
{
    addItem(constructNodeItem(node, POINT_RADIUS));
}

void GridScene::removeArea(AreaItem *area)
{
    _areaItemList.removeAll(area);
    removeItem(area);
}

void GridScene::addArea(AreaItem *area)
{
    _areaItemList << area;
    addItem(area);
}

void GridScene::updateAreas()
{
    foreach(AreaItem* a, _areaItemList)
        a->updateGridArea(*_grid->getArea(a->getName()));
}

void GridScene::startAreaCreation(const QPointF &point)
{
    _areaCreatorItem->setPos(point);
    _areaCreatorItem->setRect(0, 0, 0, 0);
    addItem(_areaCreatorItem);
    _areaCreatorOrigin = point;
}

void GridScene::continueAreaCreation(const QPointF &point)
{
    QPointF pos = _areaCreatorOrigin;

    QPointF topLeft(point.x() - pos.x() >= 0 ? pos.x() : point.x(), point.y() - pos.y() >= 0 ? pos.y() : point.y());
    QSizeF size(qAbs(point.x() - pos.x()), qAbs(point.y() - pos.y()));

    _areaCreatorItem->setPos(topLeft);
    _areaCreatorItem->setRect(QRectF(QPointF(), size));
}

void GridScene::stopAreaCreation(const QPointF &point)
{
    QPointF pos = _areaCreatorOrigin;

    QPointF topLeft(point.x() - pos.x() >= 0 ? pos.x() : point.x(), point.y() - pos.y() >= 0 ? pos.y() : point.y());
    QSizeF size(qAbs(point.x() - pos.x()), qAbs(point.y() - pos.y()));

    //ensure area name is unique
    QString baseName = "Area";
    int index = 1;
    QString name = QString(baseName).append(QString::number(index));
    while (_grid->getAreas().contains(name))
        name = QString(baseName).append(QString::number(++index));

    _grid->addArea(SceneTools::mapWithStandardReference(QRectF(topLeft, size)), name);
    removeItem(_areaCreatorItem);

    AreaItem* item = constructAreaItem(name, _grid->getArea(name));
    item->setMode(_mode);
    addItem(item);
    _areaItemList << item;

    clearSelection();
    item->setSelected(true);

    emit areaCreated(item, topLeft, size);
}

NodeItem* GridScene::constructNodeItem(Tools::NGridNode* node, int radius)
{
    NodeItem* i = new NodeItem(node, radius);
    QString alias = _grid->getAlias(node);
    if (!alias.isEmpty())
        i->setAlias(alias);

    connect(i, SIGNAL(positionChanged(NodeItem*,QPointF,QPointF)), this, SLOT(nodeMoved(NodeItem*,QPointF,QPointF)));

    return i;
}

AreaItem *GridScene::constructAreaItem(const QString &name, const NGridArea *area)
{
    AreaItem* item = new AreaItem(name, *area);
    connect(item, SIGNAL(areaChanged(AreaItem*,QPointF,QSizeF,QPointF,QSizeF)), this, SIGNAL(areaChanged(AreaItem*,QPointF,QSizeF,QPointF,QSizeF)));

    return item;
}

void GridScene::setBackground(const QPixmap& pixmap)
{   
    if (!_background)
    {
        _background = new QGraphicsPixmapItem(pixmap);
        _background->setZValue(-1000.0);
        addItem(_background);
    }
    else
        _background->setPixmap(pixmap);

    double tableScaleFactor =  3000.0 / _background->boundingRect().width();
    _background->setScale(tableScaleFactor);
}

QPixmap GridScene::getBackground() const
{
    if (_background)
        return _background->pixmap();

    return QPixmap();
}

void GridScene::selectionChanged()
{
    QList<NodeItem *> nodeItems = getSelectedNodes();
    QList<Tools::NGridNode*> nodes;
    foreach(NodeItem* i, nodeItems)
        nodes << i->getNode();

    clearTemporaryLinks();

    QList<QColor> selectionColorLevels;
    selectionColorLevels << Qt::blue << Qt::darkBlue;

    showNodeLinks(nodes, 2, selectionColorLevels);
}

void GridScene::clearTemporaryLinks()
{
    for(QHash<QPointF, QHash<QPointF, LinkItem*> >::iterator it = _selectedLinkLines.begin();
        it != _selectedLinkLines.end(); ++it)
    {
        for(QHash<QPointF, LinkItem*>::iterator it2 = it.value().begin();
                it2 != it.value().end(); ++it2)
        {
            LinkItem* l = it2.value();
            if (l && l->scene() == this)
            {
                removeItem(l);
                delete l;
                l = 0;

                if (_selectedLinkLines.contains(it2.key()) && _selectedLinkLines[it2.key()].contains(it.key()))
                    _selectedLinkLines[it2.key()][it.key()] = 0;
            }
        }
        it.value().clear();
    }
    _selectedLinkLines.clear();
}

void GridScene::ensureLinkRemoved(Tools::NGridNode* n1, Tools::NGridNode* n2)
{
    QPointF p1 = SceneTools::getNodeScenePosition(n1);
    QPointF p2 = SceneTools::getNodeScenePosition(n2);

    if (_selectedLinkLines.contains(p1) && _selectedLinkLines[p1].contains(p2))
    {
        LinkItem* l = _selectedLinkLines[p1][p2];
        if (!l)
            return;

        switch(l->getConnectionType())
        {
            case SceneTools::Normal:
                if (l->getN1()->getPosition() == n1->getPosition())
                {
                    _selectedLinkLines[p2][p1] = 0;
                    _selectedLinkLines[p1][p2] = 0;
                    removeItem(l);
                    delete l;
                }
                else
                {
                    Q_ASSERT(false);
                }
                break;
            case SceneTools::Both:
                if (l->getN1()->getPosition() == n1->getPosition())
                    l->setConnectionType(SceneTools::Reverted);
                else if (l->getN1()->getPosition() == n2->getPosition())
                    l->setConnectionType(SceneTools::Normal);
                else
                    Q_ASSERT(false);
                break;
            case SceneTools::Reverted:
                if (l->getN1()->getPosition() == n2->getPosition())
                {
                    _selectedLinkLines[p2][p1] = 0;
                    _selectedLinkLines[p1][p2] = 0;
                    removeItem(l);
                    delete l;
                }
                else
                {
                    Q_ASSERT(false);
                }
                break;
        }
    }
}

void GridScene::setMode(SceneTools::Mode mode)
{
    _mode = mode;
     QList<QGraphicsItem*> allItems = items();
     foreach(QGraphicsItem* i, allItems)
     {
         NodeItem* node = qgraphicsitem_cast<NodeItem*>(i);
         if (node)
             node->setMode(mode);
     }

     foreach(AreaItem* i, _areaItemList)
         i->setMode(mode);

     if (_background)
     {
         if (mode == SceneTools::ConnectionMode)
             _background->setCursor(QCursor(Qt::CrossCursor));
         else
             _background->unsetCursor();
     }

     if (mode != SceneTools::ConnectionMode)
         cancelConstruction();
}

void GridScene::showNodeLinks(const QList<Tools::NGridNode *> nodes, int level, QList<QColor>& selectionColors)
{
    QList<NGridNode*> secondaryNodes;
    QColor c = selectionColors.isEmpty() ? Qt::black : selectionColors.takeFirst();

    foreach(NGridNode* node, nodes)
    {
        for(QHash<NGridNode*, int>::const_iterator it = node->getReachableNodes().constBegin();
            it != node->getReachableNodes().constEnd(); ++it)
        {
            showNodeLink(node, it.key(), c);

            if (!nodes.contains(it.key()) && level > 1)
                secondaryNodes << it.key();
        }

        for(QList<NGridNode*>::const_iterator it = node->getRevertedConnections().constBegin();
            it != node->getRevertedConnections().constEnd(); ++it)
        {
            showNodeLink(*it, node, c);

            if (!nodes.contains(*it) && !secondaryNodes.contains(*it) && level > 1)
                secondaryNodes << *it;
        }
    }

    if (level > 1)
    {
        --level;
        showNodeLinks(secondaryNodes, level, selectionColors);
    }
}

void GridScene::showNodeLink(Tools::NGridNode *n1, Tools::NGridNode *n2, const QColor &color)
{
    QPointF p = SceneTools::getNodeScenePosition(n1);
    QPointF p2 = SceneTools::getNodeScenePosition(n2);
    if (_selectedLinkLines.contains(p) && _selectedLinkLines[p].contains(p2))
    {
        //nothing to do, the link already exist
    }
    else if (_selectedLinkLines.contains(p2) && _selectedLinkLines[p2].contains(p))
    {
        _selectedLinkLines[p2][p]->setConnectionType(SceneTools::Both);
        _selectedLinkLines[p][p2] = _selectedLinkLines[p2][p];
    }
    else
    {
        LinkItem* linkItem = new LinkItem(n1, n2, SceneTools::Normal);
        linkItem->setColor(color);
        addItem(linkItem);
        _selectedLinkLines[p][p2] = linkItem;
    }
}

Tools::NGrid * GridScene::getGrid() const
{
    return _grid;
}

void GridScene::nodeMoved(NodeItem* node, const QPointF &oldPosition, const QPointF &newPosition)
{
    if (node == focusItem())
        emit nodeMoved(selectedItems(), node, oldPosition, newPosition);
}

void GridScene::reselect()
{
    selectionChanged();
}

QList<NodeItem*> GridScene::getSelectedNodes() const
{
    QList<QGraphicsItem*> items = selectedItems();
    QList<NodeItem*> nodes;
    foreach(QGraphicsItem* i, items)
    {
        NodeItem* n = qgraphicsitem_cast<NodeItem*>(i);
        if (n)
            nodes << n;
    }

    return nodes;
}

NodeItem* GridScene::getFirstNodeAt(const QPointF &position)
{
    QList<QGraphicsItem*> visibleItems = items(position, Qt::ContainsItemBoundingRect, Qt::DescendingOrder);
    foreach(QGraphicsItem* visibleItem, visibleItems)
    {
        NodeItem* node = qgraphicsitem_cast<NodeItem*>(visibleItem);
        if (node)
            return node;
    }

    return 0;
}

void GridScene::selectAll()
{
    QPainterPath p;
    p.addRect(sceneRect());
    setSelectionArea(p);
}


void GridScene::constructLink(const QPointF &position)
{
    NodeItem* nodeAtPosition = getFirstNodeAt(position);
    if (!nodeAtPosition)
    {
        Tools::NGridNode* newNode = new NGridNode(SceneTools::mapWithStandardReference(position));
        nodeAtPosition = constructNodeItem(newNode);
        nodeAtPosition->setMode(SceneTools::ConnectionMode);
    }

    nodeAtPosition->setSelected(true);

    emit nodeCreated(nodeAtPosition, _constructionInProgress ? _previouslyConstructedNodeItem : 0);
    _previouslyConstructedNodeItem = nodeAtPosition;

    _constructionStartNode->setPosition(SceneTools::mapWithStandardReference(position));
    if (!_constructionInProgress)
    {
        _constructionInProgress = true;
        updateConstructionLinkPosition(position);
        addItem(_constructionLinkItem);
        _constructionLinkItem->updatePosition();
    }

    update();
}

void GridScene::updateConstructionLinkPosition(const QPointF &position)
{
    _constructionEndNode->setPosition(SceneTools::mapWithStandardReference(position));
    update();
}

void GridScene::cancelConstruction()
{
    _constructionInProgress = false;

    if (_constructionLinkItem->scene() == this)
        removeItem(_constructionLinkItem);

    clearSelection();
    update();
}

bool GridScene::constructionInProgress() const
{
    return _constructionInProgress;
}



