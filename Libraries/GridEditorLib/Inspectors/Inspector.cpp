#include "Inspector.h"
#include "ui_Inspector.h"

#include "LinkedNodeEditor.h"
#include "NodeItem.h"
#include <QMessageBox>

Inspector::Inspector(Tools::NGrid* grid, QWidget *parent) :
    QDockWidget(parent), ui(new Ui::Inspector)
{
    ui->setupUi(this);
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	setMinimumWidth(200);

    ui->nodeInspector->setGrid(grid);

    setContentType(Nothing);
}

Inspector::~Inspector()
{
    delete ui;
}

void Inspector::setContentType(Inspector::InspectorContentType content)
{
    _content = content;
    switch(_content)
    {
        case Nodes:
            ui->areaInspector->hide();
            ui->nodeInspector->show();
            setWindowTitle("Node inspector");
            break;
        case Areas:
            ui->areaInspector->show();
            ui->nodeInspector->hide();
            setWindowTitle("Area inspector");
            break;
        case Nothing:
            ui->areaInspector->hide();
            ui->nodeInspector->hide();
            ui->areaInspector->clearInfo();
            ui->nodeInspector->clearInfo();
            setWindowTitle("Inspector");
            break;
    }
}

NodeInspector *Inspector::getNodeInspector() const
{
    return ui->nodeInspector;
}

AreaInspector *Inspector::getAreaInspector() const
{
    return ui->areaInspector;
}

