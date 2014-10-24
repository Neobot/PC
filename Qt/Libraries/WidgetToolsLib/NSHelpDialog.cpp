#include "NSHelpDialog.h"
#include "ui_NSHelpDialog.h"

#include <QDir>

NSHelpDialog::NSHelpDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::NSHelpDialog)
{
	ui->setupUi(this);
	QDir d(qApp->applicationDirPath());
	QString helpLocation = d.absoluteFilePath("nsreference.html");
	ui->webView->load(QUrl::fromLocalFile(helpLocation));
}

NSHelpDialog::~NSHelpDialog()
{
	delete ui;
}
