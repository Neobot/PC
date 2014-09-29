#ifndef GRIDEDITOR_H
#define GRIDEDITOR_H

#include <QUndoStack>
#include <QCloseEvent>
#include <QSignalMapper>
#include <QSettings>
#include <QMainWindow>
#include <QActionGroup>

#include "GridScene.h"

class NodeItem;
class AreaItem;
class Inspector;
class NewGridDialog;

namespace Ui {class GridEditor;}

class GridEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit GridEditor(QWidget *parent = 0, bool singleEditionMode = false, bool emtyByDefault = false);
	~GridEditor();

	void open(const QString& file);

	double getZoom() const;
	void setZoom(double value);

public slots:
	void set2011Table();
	void set2012Table();
	void set2013Table();
    void set2014Table();
	void set2015Table();

signals:
	void accepted();
	void rejected();

private:
	Ui::GridEditor* ui;
    QUndoStack* _undoStack;
    GridScene* _scene;
    NewGridDialog* _newGridDialog;

    QAction* _removeSelectionAction;

    QAction* _connectionModeAction;
    QAction* _areaModeAction;
    QAction* _selectionModeAction;
    QActionGroup* _modeActionGroup;

    SceneTools::Mode _mode;

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActs[MaxRecentFiles];
    QAction* recentFileListSeparatorAction;
    QStringList _recentFileList;
    QSignalMapper* _recentFileActionsMapper;
    QSettings* _settings;

    QString _currentFilename;
    bool _hasModification;

    Inspector* _inspector;

	void initActions(bool singleEditionMode);
    void setTitle();
    bool checkDataToSave();

    void addToRecentFileList(const QString& file);
    void updateRecentFileActions();

    void loadSettings();

    void setMode(SceneTools::Mode mode);

protected:
    void closeEvent(QCloseEvent* event);
    void contextMenuEvent(QContextMenuEvent *event);

private slots:
	void onOK();
	void onCancel();
    void open();
    void openRecentFile(int index);
    void newDocument(bool empty = false);
    bool save();
    bool saveAs();

    void selectAll();   
    void removeSelection();

    void switchToDefaultMode();
    void changeConnectionMode();
    void changeSelectionMode();
    void changeAreaMode();

    void zoomIn();
    void zoomOut();

    void changeBackground();

    void selectionChanged();
    void undoStackCleanChanged(bool value);

    void nodeMoved(const QList<QGraphicsItem*>& selectedNodes, NodeItem* focusedNode, const QPointF& oldPosition, const QPointF& newPosition);
    void nodeRemoved(const QList<QGraphicsItem*>& selectedNodes);
    void nodeCreated(NodeItem* node, NodeItem* previousNode);

    void areaChanged(AreaItem* item, const QPointF& oldPoint, const QSizeF& oldSize, const QPointF& newPoint, const QSizeF& newSize);
    void areaCreated(AreaItem* item, const QPointF& pos, const QSizeF& size);
    void areaChangedFromInspector(AreaItem* item, const QPointF& oldPoint, const QSizeF& oldSize, const QPointF& newPoint, const QSizeF& newSize);
    void areaNameChangedFromInspector(AreaItem* item, const QString& newName);

    void nodeMovedFromInspector(NodeItem* node, const QPointF& position);
    void nodeAliasChangedFromInspector(NodeItem* node, const QString& newAlias);
    void linkRemovedFromInspector(Tools::NGridNode *node1, Tools::NGridNode* node2, int cost);
    void linkAddedFromInspector(Tools::NGridNode *node1, Tools::NGridNode* node2, int cost);
    void costChangedFromInspector(Tools::NGridNode *node1, Tools::NGridNode* node2, int cost);
    void removeSelectedNodesConnections();
    void connectSelectedNodes();
	void selectionPositionXMovedFromInspector(double x);
	void selectionPositionYMovedFromInspector(double y);
	void selectionPositionXShiftedFromInspector(double x);
	void selectionPositionYShiftedFromInspector(double y);


};

#endif // GRIDEDITOR_H
