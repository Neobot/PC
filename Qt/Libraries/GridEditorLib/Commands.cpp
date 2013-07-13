#include "Commands.h"
#include "NodeItem.h"
#include "AreaItem.h"
#include "Inspectors/NodeInspector.h"
#include "Inspectors/AreaInspector.h"

#include <QtDebug>

ChangeBackgroundCommand::ChangeBackgroundCommand(GridScene* scene, const QPixmap& pixmap) :
    _scene(scene), _pixmap(pixmap)
{
    _oldPixmap = _scene->getBackground();
    setText("Change background");
}

void ChangeBackgroundCommand::undo()
{
    _scene->setBackground(_oldPixmap);
}

void ChangeBackgroundCommand::redo()
{
    _scene->setBackground(_pixmap);
}

//-------------

MoveNodeCommand::MoveNodeCommand(GridScene* scene, NodeInspector* inspector, const QList<QGraphicsItem*>& nodes,
                                 NodeItem* focusedNode, const QPointF& oldPosition, const QPointF& newPosition) :
    _scene(scene), _inspector(inspector), _nodes(nodes), _focusedNode(focusedNode), _oldPosition(oldPosition),
    _newPosition(newPosition), _firstTime(false)
{
    setText("Move node");
}

void MoveNodeCommand::undo()
{
    foreach(QGraphicsItem* i, _nodes)
    {
        NodeItem* n = qgraphicsitem_cast<NodeItem*>(i);
        if (n)
        {
            n->blockSignals(true);
            n->getNode()->setPosition(_oldPosition + (n->getNode()->getPosition() - _newPosition));
            n->updatePositionFromNode();
            n->blockSignals(false);
        }
    }

    _scene->update();
    _inspector->update();
}

void MoveNodeCommand::redo()
{
    if (!_firstTime)
    {
        _firstTime = true;
        return;
    }

    foreach(QGraphicsItem* i, _nodes)
    {
        NodeItem* n = qgraphicsitem_cast<NodeItem*>(i);
        if (n)
        {
            n->blockSignals(true);
            if (_firstTime)
            {
                n->getNode()->setPosition(_newPosition + (n->getNode()->getPosition() - _oldPosition));
                n->updatePositionFromNode();
            }
            n->blockSignals(false);
        }
    }

    _scene->update();
    _inspector->update();
}

bool MoveNodeCommand::mergeWith(const QUndoCommand *other)
{
    const MoveNodeCommand* otherMoveCommand =  static_cast<const MoveNodeCommand*>(other);

    if (_focusedNode != otherMoveCommand->_focusedNode)
        return false;

    foreach(QGraphicsItem* i, _nodes)
        if (!otherMoveCommand->_nodes.contains(i))
            return false;

    _newPosition = otherMoveCommand->_newPosition;
    return true;
}

void MoveNodeCommand::enableRedoOnPush()
{
    _firstTime = true;
}

//------------

RemoveNodeCommand::RemoveNodeCommand(Tools::NGrid* grid, GridScene* scene, const QList<QGraphicsItem*>& nodes) :
    _grid(grid), _scene(scene), _nodes(nodes)
{
    setText("Remove node");
}

void RemoveNodeCommand::undo()
{
    for(QList<QGraphicsItem*>::const_iterator itItem = _nodes.constEnd() - 1;
        itItem != _nodes.constBegin() - 1; --itItem)
    {
        NodeItem* n = qgraphicsitem_cast<NodeItem*>(*itItem);
        if (n)
        {
            Tools::NGridNode* node = n->getNode();

            _grid->addNode(node);
            const QHash<Tools::NGridNode*, int>& reachableNodes = _connectedNodesLists[node];
            for(QHash<Tools::NGridNode*, int>::const_iterator it = reachableNodes.constBegin();
                it != reachableNodes.constEnd(); ++it)
            {
                node->addReachableNode(it.key(), it.value());
            }

            const QHash<Tools::NGridNode*, int>& revertedNodes = _connectedNodesLists[node];
            for(QHash<Tools::NGridNode*, int>::const_iterator it = revertedNodes.constBegin();
                it != revertedNodes.constEnd(); ++it)
            {
                it.key()->addReachableNode(node, it.value());
            }

            _scene->addItem(n);
        }
    }
    _scene->reselect();
}

void RemoveNodeCommand::redo()
{
    foreach(QGraphicsItem* i, _nodes)
    {
        NodeItem* n = qgraphicsitem_cast<NodeItem*>(i);
        if (n)
        {
            Tools::NGridNode* node = n->getNode();

            _connectedNodesLists[node] = node->getReachableNodes();
            const QList<Tools::NGridNode*>& revertedConnections = node->getRevertedConnections();
            foreach(Tools::NGridNode* r, revertedConnections)
                _revertedConnectedNodesLists[node][r] = r->getReachableNodes()[node];

            _grid->removeNode(node);

            n->setSelected(false);
            _scene->removeItem(n);
        }
    }

    _scene->reselect();
}

//-------------

AliasChangedCommand::AliasChangedCommand(Tools::NGrid* grid, GridScene* scene, NodeInspector* inspector, NodeItem* node, const QString& newAlias) :
    _grid(grid), _scene(scene), _inspector(inspector), _node(node), _newAlias(newAlias)
{
    _oldAlias = _grid->getAlias(_node->getNode());
    setText("Change alias");
}

void AliasChangedCommand::undo()
{
    _inspector->blockSignals(true);
    _grid->removeAlias(_node->getNode());
    _grid->addAlias(_node->getNode(), _oldAlias);
    _node->setAlias(_oldAlias);
    _scene->update();
    _inspector->update();
    _inspector->blockSignals(false);
}

void AliasChangedCommand::redo()
{
    _inspector->blockSignals(true);
    _grid->removeAlias(_node->getNode());
    _grid->addAlias(_node->getNode(), _newAlias);
    _node->setAlias(_newAlias);
    _scene->update();
    _inspector->update();
    _inspector->blockSignals(false);
}

bool AliasChangedCommand::mergeWith(const QUndoCommand *other)
{
    const AliasChangedCommand* otherMoveCommand =  static_cast<const AliasChangedCommand*>(other);

    if (_node != otherMoveCommand->_node)
        return false;

    _newAlias = otherMoveCommand->_newAlias;
    return true;
}

//------------

LinkAddedOrRemovedCommand::LinkAddedOrRemovedCommand(Tools::NGrid* grid, GridScene* scene, NodeInspector* inspector, Tools::NGridNode* node, Tools::NGridNode* secondNode, int cost, bool add) :
    _grid(grid), _scene(scene), _inspector(inspector), _node(node), _secondNode(secondNode), _add(add), _cost(cost)
{
    setText(_add ? "Add node" : "Remove node");
}

void LinkAddedOrRemovedCommand::undo()
{
    if (_add)
        remove();
    else
        add();
    _scene->update();
}

void LinkAddedOrRemovedCommand::redo()
{
    if (_add)
        add();
    else
        remove();
    _scene->update();
}

void LinkAddedOrRemovedCommand::add()
{
    _node->addReachableNode(_secondNode, _cost);
    _scene->reselect();
}

void LinkAddedOrRemovedCommand::remove()
{
    _node->removeReachableNode(_secondNode);
    _scene->ensureLinkRemoved(_node, _secondNode);
}

//------------

CostChangedCommand::CostChangedCommand(GridScene* scene, NodeInspector* inspector, Tools::NGridNode* node, Tools::NGridNode* secondNode, int cost) :
    _scene(scene), _inspector(inspector), _node(node), _secondNode(secondNode), _cost(cost)
{
    setText("Change a cost");
    _oldCost = _node->getCostToGo(_secondNode);
}

void CostChangedCommand::undo()
{
    _node->setReachableNodeCostToGo(_secondNode, _oldCost);
    _scene->update();
}

void CostChangedCommand::redo()
{
    _node->setReachableNodeCostToGo(_secondNode, _cost);
    _scene->update();
}

bool CostChangedCommand::mergeWith(const QUndoCommand *other)
{
    const CostChangedCommand* otherCommand =  static_cast<const CostChangedCommand*>(other);

    if (_node != otherCommand->_node || _secondNode != otherCommand->_secondNode)
        return false;

    _cost = otherCommand->_cost;
    return true;
}

//------------

RemoveConnectionsCommand::RemoveConnectionsCommand(GridScene* scene, const QList<NodeItem*>& nodes) :
    _scene(scene), _nodes(nodes)
{
    setText("Remove connections");
}

void RemoveConnectionsCommand::undo()
{
    for(QHash<NodeItem*, QHash<NodeItem*, int> >::const_iterator it = _connectedNodesLists.constBegin();
        it != _connectedNodesLists.constEnd(); ++it)
    {
        NodeItem* n1 = it.key();
        for(QHash<NodeItem*, int>::const_iterator it2 = it.value().constBegin();
            it2 != it.value().constEnd(); ++it2)
        {
            NodeItem* n2 = it2.key();
            n1->getNode()->addReachableNode(n2->getNode(), it2.value());
        }
    }

    _scene->reselect();
    _scene->update();
}

void RemoveConnectionsCommand::redo()
{
    foreach(NodeItem* n1, _nodes)
    {
        foreach(NodeItem* n2, _nodes)
        {
            if (n1 != n2)
            {
                if (n1->getNode()->getReachableNodes().contains(n2->getNode()))
                {
                    _connectedNodesLists[n1][n2] = n1->getNode()->getReachableNodes().value(n2->getNode(), -1);
                    n1->getNode()->removeReachableNode(n2->getNode());
                }
            }
        }
    }

    _scene->reselect();
    _scene->update();
}

//------------

AddSingleConnectionCommand::AddSingleConnectionCommand(Tools::NGrid* grid, GridScene* scene, NodeItem* n1, NodeItem* n2) :
    _grid(grid), _scene(scene), _n1(n1), _n2(n2)
{
    setText("Add node");

    _n1ToBeAdded = !_grid->contains(_n1->getNode());

    if (n2)
    {
        _n2ToBeAdded = !_grid->contains(_n2->getNode());
        _connect1to2 = !_n2->getNode()->getRevertedConnections().contains(_n1->getNode());
        _connect2to1 = !_n1->getNode()->getRevertedConnections().contains(_n2->getNode());
    }
}

void AddSingleConnectionCommand::undo()
{
    if (_n1ToBeAdded)
    {
        _grid->removeNode(_n1->getNode());
        _n1->setSelected(false);
        _scene->removeItem(_n1);
    }

    if (_n2)
    {
        if (_n2ToBeAdded)
        {
            _grid->removeNode(_n2->getNode());
            _n2->setSelected(false);
            _scene->removeItem(_n2);
        }

        if (_connect1to2)
            _n1->getNode()->removeReachableNode(_n2->getNode());

        if (_connect2to1)
            _n2->getNode()->removeReachableNode(_n1->getNode());
    }

    _scene->reselect();
    _scene->update();
}

void AddSingleConnectionCommand::redo()
{
    _n1->setSelected(true);
    if (_n1ToBeAdded)
    {
        _grid->addNode(_n1->getNode());
        _scene->addItem(_n1);
    }

    if (_n2)
    {
        if (_n2ToBeAdded)
        {
            _grid->addNode(_n2->getNode());
            _scene->addItem(_n2);
        }

        if (_connect1to2)
            _n1->getNode()->addReachableNode(_n2->getNode());

        if (_connect2to1)
            _n2->getNode()->addReachableNode(_n1->getNode());
    }

    _scene->reselect();
    _scene->update();
}

//------------

AddConnectionsCommand::AddConnectionsCommand(GridScene* scene, const QList<NodeItem*>& nodes) :
    _scene(scene), _nodes(nodes)
{
    setText("Add connections");
}

void AddConnectionsCommand::undo()
{
    for(QHash<NodeItem*, QHash<NodeItem*, int> >::const_iterator it = _connectedNodesLists.constBegin();
        it != _connectedNodesLists.constEnd(); ++it)
    {
        NodeItem* n1 = it.key();
        for(QHash<NodeItem*, int>::const_iterator it2 = it.value().constBegin();
            it2 != it.value().constEnd(); ++it2)
        {
            NodeItem* n2 = it2.key();
            n1->getNode()->removeReachableNode(n2->getNode());
        }
    }

    _scene->reselect();
    _scene->update();
}

void AddConnectionsCommand::redo()
{
    foreach(NodeItem* n1, _nodes)
    {
        foreach(NodeItem* n2, _nodes)
        {
            if (n1 != n2)
            {
                if (!n1->getNode()->getReachableNodes().contains(n2->getNode()))
                {
                    _connectedNodesLists[n1][n2] = n1->getNode()->getReachableNodes().value(n2->getNode(), -1);
                    n1->getNode()->addReachableNode(n2->getNode());
                }
            }
        }
    }

    _scene->reselect();
    _scene->update();
}

//-------------

ChangeAreaCommand::ChangeAreaCommand(GridScene* scene, AreaInspector *inspector, AreaItem *item, const QPointF &oldPosition, const QSizeF &oldSize, const QPointF &newPosition, const QSizeF &newSize) :
    _scene(scene), _item(item), _oldPosition(oldPosition), _oldSize(oldSize), _newPosition(newPosition), _newSize(newSize),
    _inspector(inspector), _firstTime(false)
{
    setText("Change Area");
}

void ChangeAreaCommand::undo()
{
    _inspector->blockSignals(true);
    _item->blockSignals(true);
    _item->setSize(_oldSize);
    _item->setPos(_oldPosition);
    _item->blockSignals(false);

    _scene->update();
    _inspector->update();
    _inspector->blockSignals(false);
}

void ChangeAreaCommand::redo()
{
    if (!_firstTime)
    {
        _firstTime = true;
        _inspector->update();
        return;
    }

    _inspector->blockSignals(true);

    _item->blockSignals(true);
    _item->setSize(_newSize);
    _item->setPos(_newPosition);
    _item->blockSignals(false);

    _scene->update();
    _inspector->update();

    _inspector->blockSignals(false);
}

bool ChangeAreaCommand::mergeWith(const QUndoCommand *other)
{
    const ChangeAreaCommand* otherCommand =  static_cast<const ChangeAreaCommand*>(other);

    if (_item != otherCommand->_item)
        return false;

    _newPosition = otherCommand->_newPosition;
    _newSize = otherCommand->_newSize;
    return true;
}

void ChangeAreaCommand::enableRedoOnPush()
{
    _firstTime = true;
}

//------------

RemoveAreaCommand::RemoveAreaCommand(Tools::NGrid* grid, GridScene* scene, AreaItem* item) :
    _grid(grid), _scene(scene), _item(item)
{
    setText("Remove an area");
    _name = _item->getName();
    _rect = _grid->getArea(_name)->getRect();
    _isSelected = _item->isSelected();
}

void RemoveAreaCommand::undo()
{
    _grid->addArea(_rect, _name);
    _scene->addArea(_item);
    if (_isSelected)
        _item->setSelected(true);

    _scene->update();
}

void RemoveAreaCommand::redo()
{
    _item->setSelected(false);
    _scene->removeArea(_item);
    delete _grid->removeArea(_name);

    _scene->update();
}

//-------------

AreaNameChangedCommand::AreaNameChangedCommand(Tools::NGrid* grid, GridScene* scene, AreaInspector* inspector, AreaItem* area, const QString& newAlias) :
    _grid(grid), _scene(scene), _inspector(inspector), _area(area), _newAlias(newAlias)
{
    _oldAlias = _area->getName();
    setText("Rename area");
}

void AreaNameChangedCommand::undo()
{
    _inspector->blockSignals(true);

    _grid->renameArea(_newAlias, _oldAlias);
    _area->setName(_oldAlias);

    _scene->update();
    //_inspector->update();

    _inspector->blockSignals(false);
}

void AreaNameChangedCommand::redo()
{
    _inspector->blockSignals(true);

    _grid->renameArea(_oldAlias, _newAlias);
    _area->setName(_newAlias);

    _scene->update();
    //_inspector->update();

    _inspector->blockSignals(false);
}

bool AreaNameChangedCommand::mergeWith(const QUndoCommand *other)
{
    const AreaNameChangedCommand* otherMoveCommand =  static_cast<const AreaNameChangedCommand*>(other);

    if (_area != otherMoveCommand->_area)
        return false;

    _newAlias = otherMoveCommand->_newAlias;
    return true;
}

//-------------

CreateAreaCommand::CreateAreaCommand(Tools::NGrid* grid, GridScene* scene, AreaItem* area, const QPointF& position, const QSizeF& size) :
    _grid(grid), _scene(scene), _item(area), _rect(position, size), _firstTime(false)
{
    setText("Create Area");
    _name = _item->getName();
}

void CreateAreaCommand::undo()
{
    _item->setSelected(false);
    _scene->removeArea(_item);
    delete _grid->removeArea(_name);

    _scene->update();
}

void CreateAreaCommand::redo()
{
    if (!_firstTime)
    {
        _firstTime = true;
        return;
    }

    _grid->addArea(SceneTools::mapWithStandardReference(_rect), _name);
    _scene->addArea(_item);

    _scene->update();
}

void CreateAreaCommand::enableRedoOnPush()
{
    _firstTime = true;
}
