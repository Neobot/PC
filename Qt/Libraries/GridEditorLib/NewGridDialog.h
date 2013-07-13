#ifndef STRANDARDGRIDDIALOG_H
#define STRANDARDGRIDDIALOG_H

#include <QDialog>

namespace Ui
{
    class NewGridDialog;
}

namespace Tools
{
    class NGrid;
}

class NewGridDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewGridDialog(QWidget *parent = 0);
    ~NewGridDialog();

    void processGrid(Tools::NGrid* grid) const;

private:
    Ui::NewGridDialog *ui;
};

#endif // STRANDARDGRIDDIALOG_H
