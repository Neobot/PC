#ifndef PROPERTIESWIDGET_H
#define PROPERTIESWIDGET_H

#include <QDockWidget>
#include "NGrid.h"
#include "SceneTools.h"


class LinkedNodeEditor;
class NodeItem;
class AreaItem;

class NodeInspector;
class AreaInspector;

namespace Ui {
    class Inspector;
}


class Inspector : public QDockWidget
{
    Q_OBJECT


public:
    explicit Inspector(Tools::NGrid* grid, QWidget *parent = 0);
    ~Inspector();

    enum InspectorContentType {Nothing, Nodes, Areas};

    void setContentType(InspectorContentType content);

    NodeInspector* getNodeInspector() const;
    AreaInspector* getAreaInspector() const;

private:
    Ui::Inspector *ui;
    InspectorContentType _content;
};

#endif // PROPERTIESWIDGET_H
