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
	static void openReference();

private:
	explicit NSHelpDialog(QWidget *parent = 0);
	~NSHelpDialog();

	Ui::NSHelpDialog *ui;
};

#endif // NSHELPDIALOG_H
