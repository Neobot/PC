#ifndef NSHELPDIALOG_H
#define NSHELPDIALOG_H

#include <QDialog>

namespace Ui {
	class NSHelpDialog;
}

class NSHelpDialog : public QDialog
{
	Q_OBJECT

public:
	explicit NSHelpDialog(QWidget *parent = 0);
	~NSHelpDialog();

private:
	Ui::NSHelpDialog *ui;
};

#endif // NSHELPDIALOG_H
