#include "TeleportDialog.h"
#include "ui_TeleportDialog.h"

TeleportDialog::TeleportDialog(const QVector3D& defaultValue, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TeleportDialog)
{
	ui->setupUi(this);
	ui->pointEditor->setPoint(defaultValue);
	ui->pointEditor->setPrefixes("x=", "y=", QString::fromUtf8("𝛳="));
	ui->pointEditor->setSuffixes("mm", "mm", QString::fromUtf8("°"));
	ui->pointEditor->setSeparator("  ");

	connect(ui->btnSend, SIGNAL(clicked()), this, SLOT(select()));
	connect(ui->btnOrigin, SIGNAL(clicked()), this, SLOT(setOrigin()));
	connect(ui->btnOrigin2, SIGNAL(clicked()), this, SLOT(setOrigin2()));
	connect(ui->btnStart1, SIGNAL(clicked()), this, SLOT(setStart1()));
	connect(ui->btnStart2, SIGNAL(clicked()), this, SLOT(setStart2()));
}

TeleportDialog::~TeleportDialog()
{
	delete ui;
}

QVector3D TeleportDialog::getPoint() const
{
	return _point;
}

void TeleportDialog::select()
{
	_point = ui->pointEditor->getPoint();
	accept();
}

void TeleportDialog::setOrigin()
{
	_point = QVector3D(0, 0, 0);
	accept();
}

void TeleportDialog::setOrigin2()
{
	_point = QVector3D(0, 0, 90);
	accept();
}

void TeleportDialog::setStart1()
{
	_point = QVector3D(250, 250, 90);
	accept();
}

void TeleportDialog::setStart2()
{
	_point = QVector3D(250, 2750, 270);
	accept();
}
