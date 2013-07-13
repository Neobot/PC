#include "LogView.h"
#include "ui_LogView.h"

LogView::LogView(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::LogView)
{
	ui->setupUi(this);
	connect(ui->btnClear, SIGNAL(clicked()), this, SLOT(clear()));
}

LogView::~LogView()
{
	delete ui;
}

void LogView::appendLog(const QString& log)
{
	ui->plainTextEdit->appendPlainText(log);
}

void LogView::clear()
{
	ui->plainTextEdit->clear();
}
