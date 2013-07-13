#include "NodeInspector.h"
#include "ui_NodeInspector.h"

#include "LinkedNodeEditor.h"
#include "NodeItem.h"
#include <QMessageBox>

NodeInspector::NodeInspector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NodeInspectorWidget), _grid(0), _node(0)
{
    ui->setupUi(this);

    connect(ui->btnShowConnection, SIGNAL(toggled(bool)), this, SLOT(btnShowConnectionToggled(bool)));
    connect(ui->leAlias, SIGNAL(editingFinished()), this, SLOT(aliasChanged()));
    connect(ui->spX, SIGNAL(editingFinished()), this, SLOT(positionChanged()));
    connect(ui->spY, SIGNAL(editingFinished()), this, SLOT(positionChanged()));
    connect(ui->btnRemoveConnection, SIGNAL(clicked()), this, SIGNAL(removeConnetionsBetweenSelectedNodes()));
    connect(ui->btnConnectNodes, SIGNAL(clicked()), this, SIGNAL(connectSelectedNodes()));
	connect(ui->spShift, SIGNAL(editingFinished()), this, SLOT(shiftValueChanged()));
}

NodeInspector::~NodeInspector()
{
    delete ui;
}

void NodeInspector::setGrid(Tools::NGrid *grid)
{
    _grid = grid;
}

void NodeInspector::clearInfo()
{
    showInfo(0);
}

void NodeInspector::showInfo(NodeItem *node)
{
    clearLinkedNodes();

    _node = node;
    _nbSelectedNodes = 1;

    updateAliasAndPosition(node);

    QList<Tools::NGridNode*> addedLinkedNodes;

    QList<Tools::NGridNode*> reachableNodes = _node->getNode()->getReachableNodes().keys();
    foreach(Tools::NGridNode* n, reachableNodes)
    {
        addLinkedNode(n);
        addedLinkedNodes << n;
    }

    const QList<Tools::NGridNode*>& revertedNodes = _node->getNode()->getRevertedConnections();
    foreach(Tools::NGridNode* n, revertedNodes)
    {
        if (!addedLinkedNodes.contains(n))
            addLinkedNode(n);
    }
}

void NodeInspector::showInfo(const QList<NodeItem*> &nodes)
{
    _node = 0;
    _nbSelectedNodes = nodes.count();

    showInfo(_nbSelectedNodes);

	if (!nodes.isEmpty())
	{
		double commonX = nodes.first()->getNode()->getPosition().x();
		double commonY =  nodes.first()->getNode()->getPosition().y();

		bool commonXOk = true;
		bool commonYOk = true;

		foreach(NodeItem* node, nodes)
		{
			if (commonXOk && node->getNode()->getPosition().x() != commonX)
				commonXOk = false;

			if (commonYOk && node->getNode()->getPosition().y() != commonY)
				commonYOk = false;

			if (!commonXOk && commonXOk)
				break;
		}

		if (!commonXOk)
		{
			ui->spX->setSpecialValueText(" ");
			ui->spX->setMinimum(-1);
			ui->spX->setEnabled(false);
			ui->spX->setValue(-1);
		}
		else
			ui->spX->setValue(commonX);

		if (!commonYOk)
		{
			ui->spY->setSpecialValueText(" ");
			ui->spY->setMinimum(-1);
			ui->spY->setEnabled(false);
			ui->spY->setValue(-1);
		}
		else
			ui->spY->setValue(commonY);

	}
}

void NodeInspector::showInfo(int nbNode)
{
    clearLinkedNodes();

    QString title;
    title += QString::number(nbNode);
    title += " node(s) selected";

    ui->lblTile->setText(title);
	ui->lblAlias->setVisible(false);
	ui->leAlias->setVisible(false);
    ui->connectionsInfo->setVisible(false);
    ui->spacerWidget->setVisible(true);
    ui->btnRemoveConnection->setVisible(true);
    ui->btnRemoveConnection->setEnabled(nbNode > 0);
    ui->btnConnectNodes->setVisible(true);
    ui->btnConnectNodes->setEnabled(nbNode > 0);
	ui->nodeInfoSeparator->setVisible(true);
	ui->widgetShift->setVisible(true);

    ui->leAlias->clear();
}

void NodeInspector::btnShowConnectionToggled(bool checked)
{
    ui->connectionFrame->setVisible(checked);
    ui->btnShowConnection->setArrowType(checked ? Qt::DownArrow : Qt::RightArrow);
}

void NodeInspector::addLinkedNode(Tools::NGridNode *node)
{
    QString title = _grid->getAlias(node);
    if (title.isEmpty())
    {
        title += "N";
        title += QString::number(_grid->indexOf(node));
    }
    LinkedNodeEditor* e = new LinkedNodeEditor(_node->getNode(), node, title, this);
    connect(e, SIGNAL(linkRemoved(Tools::NGridNode*,SceneTools::ConnectionType, int)), this, SLOT(linkRemoved(Tools::NGridNode*,SceneTools::ConnectionType, int)));
    connect(e, SIGNAL(linkAdded(Tools::NGridNode*,SceneTools::ConnectionType, int)), this, SLOT(linkAdded(Tools::NGridNode*,SceneTools::ConnectionType, int)));
    connect(e, SIGNAL(linkUpdated(Tools::NGridNode*,SceneTools::ConnectionType,int)), this, SLOT(linkUpdated(Tools::NGridNode*,SceneTools::ConnectionType, int)));

    _linkedNodeWidgets << e;

    static_cast<QBoxLayout*>(ui->connectionFrame->layout())->insertWidget(0, e);
}

void NodeInspector::aliasChanged()
{
    if (_node)
    {
        QString alias = ui->leAlias->text();
        QString originalAlias = _grid->getAlias(_node->getNode());
        if (alias != originalAlias)
        {
            if (_grid->getNode(alias))
            {
                ui->leAlias->setText(_grid->getAlias(_node->getNode()));
                QMessageBox::warning(this, "This alias is already used", "This alias is used by another node.\nPlease choose another alias.");
            }
            else
            {
                emit aliasChanged(_node, alias);
                ui->leAlias->clearFocus();
            }
        }
    }
}

void NodeInspector::positionChanged()
{
    if (_node)
    {
        emit positionChanged(_node, QPointF(ui->spX->value(), ui->spY->value()));
        ui->spX->clearFocus();
        ui->spY->clearFocus();
    }
	else
	{
		//multiple node change

		if(ui->spX->value() >= 0)
			emit selectionPositionXChanged(ui->spX->value());

		if(ui->spY->value() >= 0)
			emit selectionPositionYChanged(ui->spY->value());
	}
}

void NodeInspector::shiftValueChanged()
{
	ui->spShift->blockSignals(true);
	ui->spShift->clearFocus();
	switch(ui->cbShiftAxis->currentIndex())
	{
		case 0:
			emit selectionPositionXShifter(ui->spShift->value());
			break;
		case 1:
			emit selectionPositionYShifter(ui->spShift->value());
			break;
	}
	ui->spShift->blockSignals(false);
}

void NodeInspector::update()
{
    if (_nbSelectedNodes == 0)
        clearInfo();
    else if (_nbSelectedNodes == 1)
        updateAliasAndPosition(_node);
    else
        showInfo(_nbSelectedNodes);
}

void NodeInspector::clearLinkedNodes()
{
    //ui->connectionFrame->
    qDeleteAll(_linkedNodeWidgets);
    _linkedNodeWidgets.clear();
}

void NodeInspector::updateAliasAndPosition(NodeItem* node)
{
    QString alias = _grid->getAlias(node->getNode());
    QString title("Node ");
    title += QString::number(_grid->indexOf(node->getNode()));
    if (!alias.isEmpty())
    {
        title += ": ";
        title += alias;
    }

	ui->lblAlias->setVisible(true);
	ui->leAlias->setVisible(true);
	ui->spX->setSpecialValueText("");
	ui->spX->setMinimum(0);
	ui->spX->setEnabled(true);
	ui->spY->setSpecialValueText("");
	ui->spY->setMinimum(0);
	ui->spY->setEnabled(true);

    ui->lblTile->setText(title);
    ui->connectionsInfo->setVisible(true);
    ui->spacerWidget->setVisible(false);
    ui->btnRemoveConnection->setVisible(false);
    ui->btnConnectNodes->setVisible(false);
	ui->nodeInfoSeparator->setVisible(false);
	ui->widgetShift->setVisible(false);

    ui->leAlias->setText(alias);

    ui->spX->setValue(_node->getNode()->getPosition().x());
    ui->spY->setValue(_node->getNode()->getPosition().y());
}

void NodeInspector::linkRemoved(Tools::NGridNode *node, SceneTools::ConnectionType connection,int cost)
{
    switch(connection)
    {
        case SceneTools::Normal:
            emit linkRemoved(_node->getNode(), node, cost);
            break;
        case SceneTools::Reverted:
            emit linkRemoved(node, _node->getNode(), cost);
            break;
        case SceneTools::Both:
            emit linkRemoved(_node->getNode(), node, cost);
            emit linkRemoved(node, _node->getNode(), cost);
            break;
    }
}

void NodeInspector::linkAdded(Tools::NGridNode *node, SceneTools::ConnectionType connection, int cost)
{
    Q_UNUSED(cost);
    switch(connection)
    {
        case SceneTools::Normal:
            emit linkAdded(_node->getNode(), node, cost);
            break;
        case SceneTools::Reverted:
            emit linkAdded(node, _node->getNode(), cost);
            break;
        case SceneTools::Both:
            emit linkAdded(_node->getNode(), node, cost);
            emit linkAdded(node, _node->getNode(), cost);
            break;
    }
}

void NodeInspector::linkUpdated(Tools::NGridNode *node, SceneTools::ConnectionType connection, int cost)
{
    switch(connection)
    {
        case SceneTools::Normal:
            emit linkUpdated(_node->getNode(), node, cost);
            break;
        case SceneTools::Reverted:
            emit linkUpdated(node, _node->getNode(), cost);
            break;
        case SceneTools::Both:
            emit linkUpdated(_node->getNode(), node, cost);
            emit linkUpdated(node, _node->getNode(), cost);
            break;
    }
}
