#include "ControllerDialog.h"
#include <QApplication>
#include "StrategyMap.h"
#include "StrategyScene.h"
#include "StrategyManager.h"

ControllerDialog::ControllerDialog(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
    ui.graphicsView->hide();
}

void ControllerDialog::on_btnQuit_clicked()
{
    qApp->quit();
}

void ControllerDialog::enableGraphicTracking(StrategyManager* strategy)
{
    ui.graphicsView->show();
    ui.graphicsView->getScene()->setMap(strategy->getMap());
    ui.graphicsView->setRefreshInterval(200);
    ui.graphicsView->setRefreshEnabled(true);
}
