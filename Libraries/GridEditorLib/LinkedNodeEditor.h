#ifndef LINKEDNODEEDITOR_H
#define LINKEDNODEEDITOR_H

#include <QWidget>
#include "SceneTools.h"

namespace Tools
{
    class NGridNode;
}

namespace Ui {
    class LinkedNodeEditor;
}

class LinkedNodeEditor : public QWidget
{
    Q_OBJECT

public:
    explicit LinkedNodeEditor(Tools::NGridNode* node, Tools::NGridNode *otherNode, const QString& nodeTitle, QWidget *parent = 0);
    ~LinkedNodeEditor();

    void updateValues();

private:
    Ui::LinkedNodeEditor *ui;
    Tools::NGridNode* _node;
    Tools::NGridNode* _otherNode;
    SceneTools::ConnectionType _connection;

protected:
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);

private slots:
    void normalLinkToggled(bool value);
    void revertedLinkToggled(bool value);
    void normalLinkCostChanged(int value);
    void revertedLinkCostChanged(int value);

signals:
    void linkAdded(Tools::NGridNode* node, SceneTools::ConnectionType connection, int cost);
    void linkRemoved(Tools::NGridNode* node, SceneTools::ConnectionType connection, int cost);
    void linkUpdated(Tools::NGridNode* node, SceneTools::ConnectionType connection, int cost);
};

#endif // LINKEDNODEEDITOR_H
