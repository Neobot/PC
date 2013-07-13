#ifndef CONTROLLERDIALOG_H
#define CONTROLLERDIALOG_H

#include "ui_ControllerDialog.h"

class StrategyManager;

class ControllerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ControllerDialog(QWidget *parent = 0);

    void enableGraphicTracking(StrategyManager* strategy);

private:
    Ui::ControllerDialog ui;

private slots:
    void on_btnQuit_clicked();
};

#endif // CONTROLLERDIALOG_H
