#include "ui_GridEditor.h"
#include "GridEditor.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QtDebug>

#include "NGrid.h"
#include "Commands.h"
#include "Inspectors/Inspector.h"
#include "Inspectors/NodeInspector.h"
#include "Inspectors/AreaInspector.h"
#include "NewGridDialog.h"

#include "NodeItem.h"
#include "AreaItem.h"

GridEditor::GridEditor(QWidget *parent, bool singleEditionMode, bool emtyByDefault) :
    QMainWindow(parent), _mode(SceneTools::SelectionMode), _hasModification(false)
{
	Q_INIT_RESOURCE(WidgetToolsLib);
	Q_INIT_RESOURCE(GridEditor);

	ui = new Ui::GridEditor;

    setWindowIcon(QIcon(":/app/AppIcon"));
	ui->setupUi(this);
	ui->widgetSingleEdition->setVisible(singleEditionMode);
	connect(ui->btnOK, SIGNAL(clicked()), this, SLOT(onOK()));
	connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(onCancel()));

    resize(1050, 600);

    _newGridDialog = new NewGridDialog(this);

    _undoStack = new QUndoStack(this);
    connect(_undoStack, SIGNAL(cleanChanged(bool)), this, SLOT(undoStackCleanChanged(bool)));

    Tools::NGrid* g = new Tools::NGrid;
    //g->makeStandardGrid(QPointF(250, 250), 50, 50, QSizeF(1500, 2600), Tools::NGrid::HeightConnections);

    _inspector = new Inspector(g, this);
    _inspector->getNodeInspector()->clearInfo();
    addDockWidget(Qt::RightDockWidgetArea, _inspector);

    connect(_inspector->getNodeInspector(), SIGNAL(positionChanged(NodeItem*,QPointF)), this, SLOT(nodeMovedFromInspector(NodeItem*,QPointF)));
    connect(_inspector->getNodeInspector(), SIGNAL(aliasChanged(NodeItem*,QString)), this, SLOT(nodeAliasChangedFromInspector(NodeItem*,QString)));
    connect(_inspector->getNodeInspector(), SIGNAL(linkRemoved(Tools::NGridNode*,Tools::NGridNode*,int)), this, SLOT(linkRemovedFromInspector(Tools::NGridNode*,Tools::NGridNode*,int)));
    connect(_inspector->getNodeInspector(), SIGNAL(linkAdded(Tools::NGridNode*,Tools::NGridNode*,int)), this, SLOT(linkAddedFromInspector(Tools::NGridNode*,Tools::NGridNode*,int)));
    connect(_inspector->getNodeInspector(), SIGNAL(linkUpdated(Tools::NGridNode*,Tools::NGridNode*,int)), this, SLOT(costChangedFromInspector(Tools::NGridNode*,Tools::NGridNode*,int)));
    connect(_inspector->getNodeInspector(), SIGNAL(removeConnetionsBetweenSelectedNodes()), this, SLOT(removeSelectedNodesConnections()));
	connect(_inspector->getNodeInspector(), SIGNAL(connectSelectedNodes()), this, SLOT(connectSelectedNodes()));
	connect(_inspector->getNodeInspector(), SIGNAL(selectionPositionXChanged(double)), this, SLOT(selectionPositionXMovedFromInspector(double)));
	connect(_inspector->getNodeInspector(), SIGNAL(selectionPositionYChanged(double)), this, SLOT(selectionPositionYMovedFromInspector(double)));
	connect(_inspector->getNodeInspector(), SIGNAL(selectionPositionXShifter(double)), this, SLOT(selectionPositionXShiftedFromInspector(double)));
	connect(_inspector->getNodeInspector(), SIGNAL(selectionPositionYShifter(double)), this, SLOT(selectionPositionYShiftedFromInspector(double)));

    connect(_inspector->getAreaInspector(), SIGNAL(areaChanged(AreaItem*,QPointF,QSizeF,QPointF,QSizeF)), this, SLOT(areaChangedFromInspector(AreaItem*,QPointF,QSizeF,QPointF,QSizeF)));
    connect(_inspector->getAreaInspector(), SIGNAL(nameChanged(AreaItem*,QString)), this, SLOT(areaNameChangedFromInspector(AreaItem*,QString)));

	initActions(singleEditionMode);

    _settings = new QSettings(QSettings::UserScope, "Neobot", "Grid Editor", this);
    _recentFileList = _settings->value("RecentFiles").toStringList();
    updateRecentFileActions();

    _scene = new GridScene(0, this);
    connect(_scene, SIGNAL(nodeMoved(QList<QGraphicsItem*>,NodeItem*,QPointF,QPointF)),
            this, SLOT(nodeMoved(QList<QGraphicsItem*>,NodeItem*,QPointF,QPointF)));
    connect(_scene, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
    connect(_scene, SIGNAL(nodeCreated(NodeItem*,NodeItem*)), this, SLOT(nodeCreated(NodeItem*,NodeItem*)));
    connect(_scene, SIGNAL(areaChanged(AreaItem*,QPointF,QSizeF,QPointF,QSizeF)), this, SLOT(areaChanged(AreaItem*,QPointF,QSizeF,QPointF,QSizeF)));
    connect(_scene, SIGNAL(areaCreated(AreaItem*,QPointF,QSizeF)), this, SLOT(areaCreated(AreaItem*,QPointF,QSizeF)));

	connect(ui->graphicsView, SIGNAL(cancelMode()), this, SLOT(switchToDefaultMode()));
	ui->graphicsView->setScene(_scene);
    _scene->setGrid(g);

    if (!_recentFileList.isEmpty())
        open(_recentFileList.first());
    else
        newDocument(emtyByDefault);
}

GridEditor::~GridEditor()
{
	delete ui;
}

void GridEditor::initActions(bool singleEditionMode)
{
    QAction* undoAction = _undoStack->createUndoAction(this, "&Undo");
    QAction* redoAction = _undoStack->createRedoAction(this, "&Redo");
    undoAction->setShortcut(QKeySequence::Undo);
    redoAction->setShortcut(QKeySequence::Redo);
	ui->menuEdit->addAction(undoAction);
	ui->menuEdit->addAction(redoAction);
	ui->menuEdit->addSeparator();

    _modeActionGroup = new QActionGroup(this);
    _modeActionGroup->setExclusive(true);
	_selectionModeAction = ui->menuEdit->addAction(QIcon(":/toolbar/node"), "Selection", this, SLOT(changeSelectionMode()), QKeySequence(Qt::CTRL + Qt::Key_M));
    _selectionModeAction->setCheckable(true);
    _selectionModeAction->setChecked(true);
    _modeActionGroup->addAction(_selectionModeAction);
	_connectionModeAction = ui->menuEdit->addAction(QIcon(":/toolbar/link"), "Construction", this, SLOT(changeConnectionMode()), QKeySequence(Qt::CTRL + Qt::Key_L));
    _connectionModeAction->setCheckable(true);
    _modeActionGroup->addAction(_connectionModeAction);
	_areaModeAction = ui->menuEdit->addAction(QIcon(":/toolbar/area"), "Areas", this, SLOT(changeAreaMode()), QKeySequence(Qt::CTRL + Qt::Key_R));
    _areaModeAction->setCheckable(true);
    _modeActionGroup->addAction(_areaModeAction);

	ui->menuEdit->addSeparator();
	ui->menuEdit->addAction("Select All", this, SLOT(selectAll()), QKeySequence::SelectAll);
	_removeSelectionAction = ui->menuEdit->addAction("Remove selection", this, SLOT(removeSelection()), QKeySequence(Qt::CTRL + Qt::Key_D));
    _removeSelectionAction->setEnabled(false);

	if (!singleEditionMode)
	{
		ui->menuFile->addAction("&New", this, SLOT(newDocument()), QKeySequence::New);
		ui->menuFile->addAction("&Open", this, SLOT(open()), QKeySequence::Open);
		ui->menuFile->addSeparator();
	}
	ui->menuFile->addAction("&Save", this, SLOT(save()), QKeySequence::Save);
	ui->menuFile->addAction("SaveAs", this, SLOT(saveAs()), QKeySequence::SaveAs);
	ui->menuFile->addSeparator();

	_recentFileActionsMapper = new QSignalMapper(this);
	connect(_recentFileActionsMapper, SIGNAL(mapped(int)), this, SLOT(openRecentFile(int)));

	for (int i = 0; i < MaxRecentFiles; ++i)
	{
		recentFileActs[i] = new QAction(this);
		recentFileActs[i]->setVisible(false);
		connect(recentFileActs[i], SIGNAL(triggered()), _recentFileActionsMapper, SLOT(map()));
		_recentFileActionsMapper->setMapping(recentFileActs[i], i);

		if (!singleEditionMode)
			ui->menuFile->addAction(recentFileActs[i]);
	}
	recentFileListSeparatorAction = ui->menuFile->addSeparator();
	recentFileListSeparatorAction->setVisible(false);

	ui->menuFile->addAction("&Quit", this, SLOT(close()), QKeySequence::Close);

	QAction* zoomInAction = ui->menuView->addAction(QIcon(":/toolbar/zoomIn"), "Zoom In", this, SLOT(zoomIn()), QKeySequence(Qt::Key_Plus));
	QAction* zoomOutAction = ui->menuView->addAction(QIcon(":/toolbar/zoomOut"), "Zoom Out", this, SLOT(zoomOut()), QKeySequence(Qt::Key_Minus));
	ui->menuView->addSeparator();

	ui->menuView->addAction("Set 2011 background", this, SLOT(set2011Table()));
	ui->menuView->addAction("Set 2012 background", this, SLOT(set2012Table()));
	ui->menuView->addAction("Set 2013 background", this, SLOT(set2013Table()));
    ui->menuView->addAction("Set 2014 background", this, SLOT(set2014Table()));
	ui->menuView->addSeparator();
	QAction* setBackgroundAction = ui->menuView->addAction(QIcon(":/toolbar/background"), "Change background...", this, SLOT(changeBackground()));

	QAction* showNodeInspectorAction = ui->menuWindow->addAction(QIcon(":/toolbar/info"), "Show node &inspector", _inspector, SLOT(setVisible(bool)), QKeySequence(Qt::CTRL + Qt::Key_I));
    showNodeInspectorAction->setCheckable(true);
	showNodeInspectorAction->setChecked(false);
	_inspector->hide();

	ui->mainToolBar->addAction(zoomInAction);
	ui->mainToolBar->addAction(zoomOutAction);
	ui->mainToolBar->addAction(setBackgroundAction);
	ui->mainToolBar->addSeparator();
	ui->mainToolBar->addAction(_selectionModeAction);
	ui->mainToolBar->addAction(_connectionModeAction);
	ui->mainToolBar->addAction(_areaModeAction);
	ui->mainToolBar->addSeparator();
	ui->mainToolBar->addAction(showNodeInspectorAction);
}

void GridEditor::addToRecentFileList(const QString &file)
{
    if (!_recentFileList.isEmpty() && _recentFileList.first() == file)
        return;

    if (_recentFileList.contains(file))
        _recentFileList.removeAll(file);

    _recentFileList.prepend(file);
    if (_recentFileList.count() > MaxRecentFiles)
        _recentFileList.removeLast();

    _settings->setValue("RecentFiles", _recentFileList);

    updateRecentFileActions();
}

void GridEditor::updateRecentFileActions()
{
    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        const QString& file = _recentFileList.value(i);
        if (!file.isEmpty())
        {
            recentFileActs[i]->setVisible(true);

            QString actionText("&");
            actionText += QString::number(i + 1);
            actionText += " ";
            actionText += QFileInfo(file).fileName();
            recentFileActs[i]->setText(actionText);
        }
        else
        {
            recentFileActs[i]->setVisible(false);
            recentFileActs[i]->setText(QString());
        }
    }

    recentFileListSeparatorAction->setVisible(!_recentFileList.isEmpty());
}

void GridEditor::newDocument(bool empty)
{
    if (!checkDataToSave())
        return;

    if (empty || _newGridDialog->exec() == QDialog::Accepted)
    {
        _currentFilename.clear();
        if (!empty)
            _newGridDialog->processGrid(_scene->getGrid());
        else
            _scene->getGrid()->clear();
        _scene->reloadGrid();

        _hasModification = false;
        setTitle();
    }
}

void GridEditor::open(const QString& file)
{
    addToRecentFileList(file);

    _currentFilename = file;
    _scene->getGrid()->readFromFile(_currentFilename);
    _scene->reloadGrid();

    _hasModification = false;
    _undoStack->clear();
    setTitle();
	setMode(_mode);
}

double GridEditor::getZoom() const
{
	return ui->graphicsView->getScaleFactor();
}

void GridEditor::setZoom(double value)
{
	ui->graphicsView->scaleViewBy(value);
}

void GridEditor::open()
{
    if (!checkDataToSave())
        return;

    QString file = QFileDialog::getOpenFileName(this, "Select a grid file", QString(), "Grid files (*.ngrid);;all files (*.*)");
    if (!file.isEmpty())
        open(file);
}

void GridEditor::openRecentFile(int index)
{
    QString file = _recentFileList.value(index);
    if (!file.isEmpty())
        open(file);
}


bool GridEditor::save()
{
    if (_currentFilename.isEmpty())
        return saveAs();

    _scene->updateAreas();
    _scene->getGrid()->writeToFile(_currentFilename);

    _undoStack->setClean();
    setTitle();

    return true;
}

bool GridEditor::saveAs()
{
    QString file =  QFileDialog::getSaveFileName(this, "Select a grid file", QString(), "Grid files (*.ngrid)");
    if (!file.isEmpty())
    {
        _currentFilename = file;

        _scene->updateAreas();
        _scene->getGrid()->writeToFile(_currentFilename);

        _undoStack->setClean();
        setTitle();

        addToRecentFileList(_currentFilename);

        return true;
    }

    return false;
}

void GridEditor::setTitle()
{
    QString title;
    if (_currentFilename.isEmpty())
        title += "New grid";
    else
        title += _currentFilename;

    if (_hasModification)
        title += '*';

    title += " - Neobot Grid Editor";

    setWindowTitle(title);
}

void GridEditor::undoStackCleanChanged(bool value)
{
    _hasModification = !value;
    setTitle();
}

void GridEditor::selectionChanged()
{
    QList<QGraphicsItem*> selection = _scene->selectedItems();
    if (selection.isEmpty())
        _inspector->setContentType(Inspector::Nothing);

    else if (_mode != SceneTools::AreaMode)
    {
        _inspector->setContentType(Inspector::Nodes);
        QList<NodeItem*> selectedNodes = _scene->getSelectedNodes();

        _removeSelectionAction->setEnabled(!selectedNodes.isEmpty());
        if (selectedNodes.count() == 1)
            _inspector->getNodeInspector()->showInfo(selectedNodes.first());
        else
            _inspector->getNodeInspector()->showInfo(selectedNodes);
    }
    else
    {
        _inspector->setContentType(Inspector::Areas);
        //TODO

        _removeSelectionAction->setEnabled(!selection.isEmpty());
        _inspector->getAreaInspector()->showInfo(qgraphicsitem_cast<AreaItem*>(selection.first()));
    }
}

bool GridEditor::checkDataToSave()
{
    if (_hasModification)
    {
        switch(QMessageBox::question(this, "Save changes",
                QString("Your document has unsaved changes.\nDo you want to save them?"),
                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel))
        {
        case QMessageBox::Save:
            if (!save())
                return false;
            break;
        case QMessageBox::Discard:
            break;
        case QMessageBox::Cancel:
        default:
            return false;
        }
    }

    return true;
}

void GridEditor::closeEvent(QCloseEvent* event)
{
    if (!checkDataToSave())
        event->ignore();
    else
        event->accept();
}

void GridEditor::changeBackground()
{
    QString file = QFileDialog::getOpenFileName(this, "Select a background", QString(), "Images (*.jpg *.png *.bmp);;all files (*.*)");
    if (!file.isEmpty())
    {
        //ChangeBackgroundCommand* c = new ChangeBackgroundCommand(_scene, QPixmap(file));
        //_undoStack->push(c);
        _scene->setBackground(QPixmap(file));

    }
}

void GridEditor::set2011Table()
{
    _scene->setBackground(QPixmap(":/tables/table2011"));
}

void GridEditor::set2012Table()
{
	_scene->setBackground(QPixmap(":/tables/table2012"));
}

void GridEditor::set2013Table()
{
    _scene->setBackground(QPixmap(":/tables/table2013"));
}

void GridEditor::set2014Table()
{
    _scene->setBackground(QPixmap(":/tables/table2014"));
}

void GridEditor::nodeMoved(const QList<QGraphicsItem*>& selectedNodes, NodeItem* focusedNode, const QPointF &oldPosition, const QPointF &newPosition)
{
    MoveNodeCommand* c = new MoveNodeCommand(_scene, _inspector->getNodeInspector(), selectedNodes, focusedNode, oldPosition, newPosition);
    _undoStack->push(c);
    _inspector->update();
}

void GridEditor::nodeRemoved(const QList<QGraphicsItem *>& selectedNodes)
{
    RemoveNodeCommand* c = new RemoveNodeCommand(_scene->getGrid(), _scene, selectedNodes);
    _undoStack->push(c);
}

void GridEditor::selectAll()
{
    _scene->selectAll();
}

void GridEditor::removeSelection()
{
    QList<QGraphicsItem*> selection(_scene->selectedItems());
	if (_mode == SceneTools::SelectionMode && ui->graphicsView->hasFocus())
        nodeRemoved(selection);
    else if (_mode == SceneTools::AreaMode && !selection.isEmpty())
    {
        AreaItem* item = qgraphicsitem_cast<AreaItem*>(selection.first());
        if (item)
        {
            RemoveAreaCommand* c = new RemoveAreaCommand(_scene->getGrid(), _scene, item);
            _undoStack->push(c);
        }
    }
}

void GridEditor::contextMenuEvent(QContextMenuEvent *event)
{
	Q_UNUSED(event);
}

void GridEditor::onOK()
{
	save();
	close();
	emit accepted();
}

void GridEditor::onCancel()
{
	_hasModification = false;
	close();
	emit rejected();
}

void GridEditor::zoomIn()
{
	ui->graphicsView->zoomIn();
}

void GridEditor::zoomOut()
{
	ui->graphicsView->zoomOut();
}

void GridEditor::nodeMovedFromInspector(NodeItem *node, const QPointF &position)
{
    QList<QGraphicsItem*> nodes;
    nodes.append(node);

    MoveNodeCommand* c = new MoveNodeCommand(_scene, _inspector->getNodeInspector(), nodes, node, node->getNode()->getPosition(), position);
    c->enableRedoOnPush();
    _undoStack->push(c);
}

void GridEditor::nodeAliasChangedFromInspector(NodeItem *node, const QString &newAlias)
{
    AliasChangedCommand* c = new AliasChangedCommand(_scene->getGrid(), _scene, _inspector->getNodeInspector(), node, newAlias);
    _undoStack->push(c);
}

void GridEditor::linkRemovedFromInspector(Tools::NGridNode *node1, Tools::NGridNode *node2, int cost)
{
    LinkAddedOrRemovedCommand* c = new LinkAddedOrRemovedCommand(_scene->getGrid(), _scene, _inspector->getNodeInspector(), node1, node2, cost, false);
    _undoStack->push(c);
}

void GridEditor::linkAddedFromInspector(Tools::NGridNode *node1, Tools::NGridNode *node2, int cost)
{
    LinkAddedOrRemovedCommand* c = new LinkAddedOrRemovedCommand(_scene->getGrid(), _scene, _inspector->getNodeInspector(), node1, node2, cost, true);
    _undoStack->push(c);
}

void GridEditor::costChangedFromInspector(Tools::NGridNode *node1, Tools::NGridNode *node2, int cost)
{
    CostChangedCommand* c = new CostChangedCommand(_scene, _inspector->getNodeInspector(), node1, node2, cost);
    _undoStack->push(c);
}

void GridEditor::removeSelectedNodesConnections()
{
    RemoveConnectionsCommand* c = new RemoveConnectionsCommand(_scene, _scene->getSelectedNodes());
    _undoStack->push(c);
}

void GridEditor::nodeCreated(NodeItem *node, NodeItem *previousNode)
{
    AddSingleConnectionCommand* c = new AddSingleConnectionCommand(_scene->getGrid(), _scene, node, previousNode);
    _undoStack->push(c);
}

void GridEditor::areaChangedFromInspector(AreaItem *item, const QPointF &oldPoint, const QSizeF &oldSize, const QPointF &newPoint, const QSizeF &newSize)
{
    ChangeAreaCommand* c = new ChangeAreaCommand(_scene, _inspector->getAreaInspector(), item, oldPoint, oldSize, newPoint, newSize);
    c->enableRedoOnPush();
    _undoStack->push(c);
}

void GridEditor::areaNameChangedFromInspector(AreaItem *item, const QString &newName)
{
    AreaNameChangedCommand* c = new AreaNameChangedCommand(_scene->getGrid(), _scene, _inspector->getAreaInspector(), item, newName);
    _undoStack->push(c);
}

void GridEditor::areaChanged(AreaItem *item, const QPointF &oldPoint, const QSizeF &oldSize, const QPointF &newPoint, const QSizeF &newSize)
{
    ChangeAreaCommand* c = new ChangeAreaCommand(_scene, _inspector->getAreaInspector(), item, oldPoint, oldSize, newPoint, newSize);
    _undoStack->push(c);
}

void GridEditor::areaCreated(AreaItem *item, const QPointF &pos, const QSizeF &size)
{
    CreateAreaCommand* c = new CreateAreaCommand(_scene->getGrid(), _scene, item, pos, size);
    _undoStack->push(c);
}

void GridEditor::connectSelectedNodes()
{
    AddConnectionsCommand* c = new AddConnectionsCommand(_scene, _scene->getSelectedNodes());
	_undoStack->push(c);
}

void GridEditor::selectionPositionXMovedFromInspector(double x)
{
	QList<NodeItem*> nodes = _scene->getSelectedNodes();
	if (!nodes.isEmpty())
	{
		NodeItem* focusedNode = nodes.first();
		QPointF oldPosition = focusedNode->getNode()->getPosition();
		QPointF newPosition(x, focusedNode->getNode()->getPosition().y());

		QList<QGraphicsItem*>itemList;
		foreach(NodeItem* n, nodes)
			itemList << n;

		MoveNodeCommand* c = new MoveNodeCommand(_scene, _inspector->getNodeInspector(), itemList, focusedNode, oldPosition, newPosition);
		c->enableRedoOnPush();
		_undoStack->push(c);
	}
}

void GridEditor::selectionPositionYMovedFromInspector(double y)
{
	QList<NodeItem*> nodes = _scene->getSelectedNodes();
	if (!nodes.isEmpty())
	{
		NodeItem* focusedNode = nodes.first();
		QPointF oldPosition = focusedNode->getNode()->getPosition();
		QPointF newPosition(focusedNode->getNode()->getPosition().x(), y);

		QList<QGraphicsItem*>itemList;
		foreach(NodeItem* n, nodes)
			itemList << n;

		MoveNodeCommand* c = new MoveNodeCommand(_scene, _inspector->getNodeInspector(), itemList, focusedNode, oldPosition, newPosition);
		c->enableRedoOnPush();
		_undoStack->push(c);
	}
}

void GridEditor::selectionPositionXShiftedFromInspector(double x)
{
	QList<NodeItem*> nodes = _scene->getSelectedNodes();
	if (!nodes.isEmpty())
	{
		NodeItem* focusedNode = nodes.first();
		QPointF oldPosition = focusedNode->getNode()->getPosition();
		QPointF newPosition = oldPosition;
		newPosition.rx() += x;

		QList<QGraphicsItem*>itemList;
		foreach(NodeItem* n, nodes)
			itemList << n;

		MoveNodeCommand* c = new MoveNodeCommand(_scene, _inspector->getNodeInspector(), itemList, focusedNode, oldPosition, newPosition);
		c->enableRedoOnPush();
		_undoStack->push(c);
	}
}

void GridEditor::selectionPositionYShiftedFromInspector(double y)
{
	QList<NodeItem*> nodes = _scene->getSelectedNodes();
	if (!nodes.isEmpty())
	{
		NodeItem* focusedNode = nodes.first();
		QPointF oldPosition = focusedNode->getNode()->getPosition();
		QPointF newPosition = oldPosition;
		newPosition.ry() += y;

		QList<QGraphicsItem*>itemList;
		foreach(NodeItem* n, nodes)
			itemList << n;

		MoveNodeCommand* c = new MoveNodeCommand(_scene, _inspector->getNodeInspector(), itemList, focusedNode, oldPosition, newPosition);
		c->enableRedoOnPush();
		_undoStack->push(c);
	}
}

void GridEditor::changeSelectionMode()
{
   setMode(SceneTools::SelectionMode);
}


void GridEditor::changeConnectionMode()
{
    setMode(SceneTools::ConnectionMode);
}

void GridEditor::changeAreaMode()
{
    setMode(SceneTools::AreaMode);
}

void GridEditor::switchToDefaultMode()
{
    changeSelectionMode();
    _selectionModeAction->setChecked(true);
}

void GridEditor::setMode(SceneTools::Mode mode)
{
    _mode = mode;
    _scene->setMode(mode);
	ui->graphicsView->setMode(mode);
}
