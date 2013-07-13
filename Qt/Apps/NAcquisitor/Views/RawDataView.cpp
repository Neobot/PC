#include "RawDataView.h"
#include "ui_RawDataView.h"
#include <QScrollBar>

RawDataView::RawDataView(QWidget *parent) :
    QWidget(parent),
	ui(new Ui::RawDataView), _selectLast(true)
{
    ui->setupUi(this);
    connect(ui->tableRobot->verticalScrollBar(), SIGNAL(valueChanged(int)), ui->tableConsigne->verticalScrollBar(), SLOT(setValue(int)));
    connect(ui->tableConsigne->verticalScrollBar(), SIGNAL(valueChanged(int)), ui->tableRobot->verticalScrollBar(), SLOT(setValue(int)));

    connect(ui->tableRobot->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(robotSelectionChanged()));
    connect(ui->tableConsigne->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(consigneSelectionChanged()));
}

RawDataView::~RawDataView()
{
	delete ui;
}

void RawDataView::addPositionAcquisition(int t, const PositionSample &sample, int ct, const CommandSample &csample)
{
	int row = ui->tableRobot->rowCount();

	ui->tableRobot->setRowCount(row + 1);
	ui->tableRobot->setItem(row, 0, new QTableWidgetItem(QString::number(sample._x)));
	ui->tableRobot->setItem(row, 1, new QTableWidgetItem(QString::number(sample._y)));
	ui->tableRobot->setItem(row, 2, new QTableWidgetItem(QString::number(sample._theta)));
	//ui->tableRobot->setItem(row, 2, new QTableWidgetItem(QString::number(t)));

	ui->tableConsigne->setRowCount(row + 1);
	ui->tableConsigne->setItem(row, 0, new QTableWidgetItem(QString::number(csample._cx)));
	ui->tableConsigne->setItem(row, 1, new QTableWidgetItem(QString::number(csample._cy)));
	ui->tableConsigne->setItem(row, 2, new QTableWidgetItem(QString::number(csample._ctheta)));
	//ui->tableConsigne->setItem(row, 2, new QTableWidgetItem(QString::number(t)));

	if (_selectLast)
	{
		ui->tableRobot->blockSignals(true);
		ui->tableRobot->setCurrentCell(row, 0, QItemSelectionModel::ClearAndSelect);
		ui->tableRobot->blockSignals(false);

		ui->tableConsigne->blockSignals(true);
		ui->tableConsigne->setCurrentCell(row, 0, QItemSelectionModel::ClearAndSelect);
		ui->tableConsigne->blockSignals(false);
	}
}

void RawDataView::addAvoidingSharpAcquisition(int t, const SharpSample &sample)
{
	Q_UNUSED(t);
	Q_UNUSED(sample);
}

void RawDataView::setAcquisition(const Acquisition *acquisition)
{
	clear();

	_selectLast = false;

	PositionSamples samples = acquisition->getPositionSamples();
	CommandSamples csamples = acquisition->getCammandSamples();

	CommandSamples::const_iterator cit = csamples.constBegin();
	for(PositionSamples::const_iterator it = samples.constBegin(); it != samples.constEnd(); ++it)
	{
		addPositionAcquisition(it.key(), *it, cit.key(), *cit);

		if (cit != csamples.end())
			++cit;
	}

	_selectLast = true;
}

void RawDataView::clear()
{
	ui->tableConsigne->clear();
	ui->tableRobot->clear();
	ui->tableRobot->setRowCount(0);
	ui->tableConsigne->setRowCount(0);
}

void RawDataView::robotSelectionChanged()
{
    ui->tableConsigne->blockSignals(true);
    ui->tableConsigne->selectionModel()->select(ui->tableRobot->selectionModel()->selection(), QItemSelectionModel::ClearAndSelect);
    ui->tableConsigne->blockSignals(false);
}

void RawDataView::consigneSelectionChanged()
{
    ui->tableRobot->blockSignals(true);
    ui->tableRobot->selectionModel()->select(ui->tableConsigne->selectionModel()->selection(), QItemSelectionModel::ClearAndSelect);
    ui->tableRobot->blockSignals(false);
}
