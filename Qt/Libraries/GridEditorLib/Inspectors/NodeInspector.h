#ifndef NODEINSPECTORWIDGET_H
#define NODEINSPECTORWIDGET_H

#include <QWidget>
#include "NGrid.h"
#include "SceneTools.h"


class LinkedNodeEditor;
class NodeItem;

namespace Ui {
    class NodeInspectorWidget;
}

class NodeInspector : public QWidget
{
    Q_OBJECT
    
public:
    explicit NodeInspector(QWidget *parent = 0);
    ~NodeInspector();

    void setGrid(Tools::NGrid* grid);
    void clearInfo();
    void showInfo(NodeItem* node);
    void showInfo(const QList<NodeItem*>& nodes);

     void update();
    
private:
    Ui::NodeInspectorWidget *ui;
    Tools::NGrid* _grid;
    NodeItem* _node;
    int _nbSelectedNodes;

    QList<LinkedNodeEditor*> _linkedNodeWidgets;

    void clearLinkedNodes();
    void addLinkedNode(Tools::NGridNode* node);
    void showInfo(int nbNode);

    void updateAliasAndPosition(NodeItem* node);

private slots:
    void btnShowConnectionToggled(bool checked);
    void aliasChanged();
    void positionChanged();
	void shiftValueChanged();

    void linkRemoved(Tools::NGridNode* node, SceneTools::ConnectionType connection, int cost);
    void linkAdded(Tools::NGridNode* node, SceneTools::ConnectionType connection, int cost);
    void linkUpdated(Tools::NGridNode* node, SceneTools::ConnectionType connection, int cost);

signals:
    void positionChanged(NodeItem* node, const QPointF& newPosition);
    void aliasChanged(NodeItem* node, const QString& newAlias);
    void linkRemoved(Tools::NGridNode *node1, Tools::NGridNode* node2, int cost);
    void linkAdded(Tools::NGridNode *node1, Tools::NGridNode* node2, int cost);
    void linkUpdated(Tools::NGridNode *node1, Tools::NGridNode* node2, int cost);
    void removeConnetionsBetweenSelectedNodes();
    void connectSelectedNodes();
	void selectionPositionXChanged(double x);
	void selectionPositionYChanged(double y);
	void selectionPositionXShifter(double x);
	void selectionPositionYShifter(double y);
};

#endif // NODEINSPECTORWIDGET_H
