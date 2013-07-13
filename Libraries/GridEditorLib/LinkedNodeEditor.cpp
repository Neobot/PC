#include "LinkedNodeEditor.h"
#include "ui_LinkedNodeEditor.h"

#include "NGrid.h"

LinkedNodeEditor::LinkedNodeEditor(Tools::NGridNode* node, Tools::NGridNode *otherNode, const QString& nodeTitle, QWidget *parent) :
    QWidget(parent), ui(new Ui::LinkedNodeEditor), _node(node), _otherNode(otherNode)
{
    ui->setupUi(this);
    ui->arrowRightToLeft->setDirection(ArrowWidget::RightToLeft);
    ui->lblNodeName->setText(nodeTitle);

    updateValues();

    connect(ui->checkNormal, SIGNAL(clicked(bool)), this, SLOT(normalLinkToggled(bool)));
    connect(ui->checkReverted, SIGNAL(clicked(bool)), this, SLOT(revertedLinkToggled(bool)));
    connect(ui->spCost, SIGNAL(valueChanged(int)), this, SLOT(normalLinkCostChanged(int)));
    connect(ui->spReversedCost, SIGNAL(valueChanged(int)), this, SLOT(revertedLinkCostChanged(int)));
}

LinkedNodeEditor::~LinkedNodeEditor()
{
    delete ui;
}

void LinkedNodeEditor::updateValues()
{
    int cost = _node->getCostToGo(_otherNode);
    int rcost = _otherNode->getCostToGo(_node);

    ui->spCost->setValue(cost);
    ui->checkNormal->setChecked(cost >= 0);

    ui->spReversedCost->setValue(rcost);
    ui->checkReverted->setChecked(rcost >= 0);
}

void LinkedNodeEditor::normalLinkToggled(bool value)
{
    if (value)
        emit linkAdded(_otherNode, SceneTools::Normal, ui->spCost->value());
    else
        emit linkRemoved(_otherNode, SceneTools::Normal, ui->spCost->value());
}

void LinkedNodeEditor::revertedLinkToggled(bool value)
{
    if (value)
        emit linkAdded(_otherNode, SceneTools::Reverted, ui->spReversedCost->value());
    else
        emit linkRemoved(_otherNode, SceneTools::Reverted, ui->spReversedCost->value());
}

void LinkedNodeEditor::normalLinkCostChanged(int value)
{
    emit linkUpdated(_otherNode, SceneTools::Normal, value);
}

void LinkedNodeEditor::revertedLinkCostChanged(int value)
{
    emit linkUpdated(_otherNode, SceneTools::Reverted, value);
}

void LinkedNodeEditor::enterEvent(QEvent *event)
{
    //ui->frame->setFrameShape(QFrame::Panel); //Doesn't work well
    QWidget::enterEvent(event);
}

void LinkedNodeEditor::leaveEvent(QEvent *event)
{
    //ui->frame->setFrameShape(QFrame::NoFrame);
    QWidget::leaveEvent(event);
}
