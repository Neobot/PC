#include "UtilitySelector.h"
#include "ui_UtilitySelector.h"

UtilitySelector::UtilitySelector(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::UtilitySelector), _protocolUtilities(0)
{
	ui->setupUi(this);
	_group = new QButtonGroup(this);
	_group->addButton(ui->btnProtocolUtilities);
	_group->addButton(ui->btnAx12Utilities);

	_protocolUtilities = new CommGui;
	_ax12utilities = new AX12Utilities;

	move(0, 0);
}

UtilitySelector::~UtilitySelector()
{
	delete _protocolUtilities;
	delete ui;
}

void UtilitySelector::on_btnProtocolUtilities_toggled(bool checked)
{
	_protocolUtilities->setVisible(checked);
	if (!checked)
		_protocolUtilities->closeConnection();
}

void UtilitySelector::on_btnAx12Utilities_toggled(bool checked)
{
	_ax12utilities->setVisible(checked);
	if (!checked)
		_ax12utilities->closeConnection();
}
