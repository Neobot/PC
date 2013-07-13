#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include "GridScene.h"

class NodeItem;
class NodeInspector;
class AreaInspector;

class ChangeBackgroundCommand : public QUndoCommand
{
public:
    explicit ChangeBackgroundCommand(GridScene* scene, const QPixmap& pixmap);

    void undo();
    void redo();

private:
    GridScene* _scene;
    QPixmap _pixmap;
    QPixmap _oldPixmap;

};

class MoveNodeCommand : public QUndoCommand
{
public:
    explicit MoveNodeCommand(GridScene* scene, NodeInspector* inspector, const QList<QGraphicsItem*>& nodes,
                             NodeItem* focusedNode, const QPointF& oldPosition, const QPointF& newPosition);

    void enableRedoOnPush();
    void undo();
    void redo();

    bool mergeWith(const QUndoCommand *other);
    int id() const {return 1;}

private:
    GridScene* _scene;
    NodeInspector* _inspector;
    QList<QGraphicsItem*> _nodes;
    NodeItem* _focusedNode;
    QPointF _oldPosition;
    QPointF _newPosition;

    bool _firstTime;
};

class RemoveNodeCommand : public QUndoCommand
{
public:
    explicit RemoveNodeCommand(Tools::NGrid* grid, GridScene* scene, const QList<QGraphicsItem*>& nodes);

    void undo();
    void redo();

private:
    Tools::NGrid* _grid;
    GridScene* _scene;
    QList<QGraphicsItem*> _nodes;
    QHash<Tools::NGridNode*, QHash<Tools::NGridNode*, int> > _connectedNodesLists;
    QHash<Tools::NGridNode*, QHash<Tools::NGridNode*, int> > _revertedConnectedNodesLists;
};

class AliasChangedCommand : public QUndoCommand
{
public:
    explicit AliasChangedCommand(Tools::NGrid* grid, GridScene* scene, NodeInspector* inspector, NodeItem* node, const QString& newAlias);

    void undo();
    void redo();

    bool mergeWith(const QUndoCommand *other);
    int id() const {return 2;}

private:
    Tools::NGrid* _grid;
    GridScene* _scene;
    NodeInspector* _inspector;
    NodeItem* _node;

    QString _newAlias;
    QString _oldAlias;
};

class LinkAddedOrRemovedCommand : public QUndoCommand
{
public:
    explicit LinkAddedOrRemovedCommand(Tools::NGrid* grid, GridScene* scene, NodeInspector* inspector, Tools::NGridNode* node, Tools::NGridNode* secondNode, int cost, bool add);

    void undo();
    void redo();

private:
    Tools::NGrid* _grid;
    GridScene* _scene;
    NodeInspector* _inspector;
    Tools::NGridNode* _node;
    Tools::NGridNode* _secondNode;
    bool _add;
    int _cost;

    void add();
    void remove();
};

class CostChangedCommand : public QUndoCommand
{
public:
    explicit CostChangedCommand(GridScene* scene, NodeInspector* inspector, Tools::NGridNode* node, Tools::NGridNode* secondNode, int cost);

    void undo();
    void redo();

    bool mergeWith(const QUndoCommand *other);
    int id() const {return 5;}

private:
    GridScene* _scene;
    NodeInspector* _inspector;
    Tools::NGridNode* _node;
    Tools::NGridNode* _secondNode;
    int _cost;
    int _oldCost;
};

class RemoveConnectionsCommand : public QUndoCommand
{
public:
    explicit RemoveConnectionsCommand(GridScene* scene, const QList<NodeItem*>& nodes);

    void undo();
    void redo();

private:
    GridScene* _scene;
    QList<NodeItem*> _nodes;

    QHash<NodeItem*, QHash<NodeItem*, int> > _connectedNodesLists;
};

class AddConnectionsCommand : public QUndoCommand
{
public:
    explicit AddConnectionsCommand(GridScene* scene, const QList<NodeItem*>& nodes);

    void undo();
    void redo();

private:
    GridScene* _scene;
    QList<NodeItem*> _nodes;

    QHash<NodeItem*, QHash<NodeItem*, int> > _connectedNodesLists;
};

/**
  * \note \p n2 can be null but note \p n1.
  */
class AddSingleConnectionCommand : public QUndoCommand
{
public:
    explicit AddSingleConnectionCommand(Tools::NGrid* grid, GridScene* scene, NodeItem* n1, NodeItem* n2 = 0);

    void undo();
    void redo();

private:
    Tools::NGrid* _grid;
    GridScene* _scene;
    NodeItem* _n1;
    NodeItem* _n2;

    bool _n1ToBeAdded;
    bool _n2ToBeAdded;
    bool _connect1to2;
    bool _connect2to1;
};

class ChangeAreaCommand : public QUndoCommand
{
public:
    explicit ChangeAreaCommand(GridScene* scene, AreaInspector* inspector, AreaItem* item, const QPointF& oldPosition, const QSizeF& oldSize
                               , const QPointF& newPosition, const QSizeF& newSize);

    void enableRedoOnPush();
    void undo();
    void redo();

    bool mergeWith(const QUndoCommand *other);
    int id() const {return 3;}

private:
    GridScene* _scene;
    AreaItem* _item;
    QPointF _oldPosition;
    QSizeF _oldSize;
    QPointF _newPosition;
    QSizeF _newSize;
    AreaInspector* _inspector;

    bool _firstTime;
};

class RemoveAreaCommand : public QUndoCommand
{
public:
    explicit RemoveAreaCommand(Tools::NGrid* grid, GridScene* scene, AreaItem* item);

    void undo();
    void redo();

private:
    Tools::NGrid* _grid;
    GridScene* _scene;
    AreaItem* _item;
    QString _name;
    QRectF _rect;
    bool _isSelected;
};

class AreaNameChangedCommand : public QUndoCommand
{
public:
    explicit AreaNameChangedCommand(Tools::NGrid* grid, GridScene* scene, AreaInspector* inspector, AreaItem* area, const QString& newAlias);

    void undo();
    void redo();

    bool mergeWith(const QUndoCommand *other);
    int id() const {return 4;}

private:
    Tools::NGrid* _grid;
    GridScene* _scene;
    AreaInspector* _inspector;
    AreaItem* _area;

    QString _newAlias;
    QString _oldAlias;
};

class CreateAreaCommand : public QUndoCommand
{
public:
    explicit CreateAreaCommand(Tools::NGrid* grid, GridScene* scene, AreaItem* area, const QPointF& position, const QSizeF& size);

    void enableRedoOnPush();
    void undo();
    void redo();

private:
    Tools::NGrid* _grid;
    GridScene* _scene;
    AreaItem* _item;
    QString _name;
    QRectF _rect;

    bool _firstTime;
};

#endif // COMMAND_H
