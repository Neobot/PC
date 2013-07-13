#include "NewGridDialog.h"
#include "ui_NewGridDialog.h"

#include "NGrid.h"

NewGridDialog::NewGridDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewGridDialog)
{
    ui->setupUi(this);
}

NewGridDialog::~NewGridDialog()
{
    delete ui;
}

void NewGridDialog::processGrid(Tools::NGrid *grid) const
{
    grid->clear();
    if (ui->rbStandardGrid->isChecked())
    {
        Tools::NGrid::StandardGridConnectionType connections = Tools::NGrid::NoConnection;
        switch(ui->cbNbConnections->currentIndex())
        {
            case 0: connections = Tools::NGrid::NoConnection; break;
            case 1: connections = Tools::NGrid::FourConnections; break;
            case 2: connections = Tools::NGrid::HeightConnections; break;
            default: Q_ASSERT(false);
        }

        grid->makeStandardGrid(QPointF(ui->spOx->value(), ui->spOy->value()),
                               ui->spDx->value(),
                               ui->spDy->value(),
                               QSizeF(ui->spEx->value() - ui->spOx->value(), ui->spEy->value() - ui->spOy->value()),
                               connections);
    }
}
