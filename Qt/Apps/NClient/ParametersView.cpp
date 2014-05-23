#include "ParametersView.h"
#include "ui_ParametersView.h"
#include "NetworkClientCommInterface.h"

#include <QLabel>
#include <QDoubleSpinBox>

ParameterWidget::ParameterWidget(QWidget *parent) : QWidget(parent)
{
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setMargin(0);

    _lbl = new QLabel("Unknown Parameter", this);
	_spValue = new QDoubleSpinBox(this);
	_spValue->setMinimum(-99999999999999);
	_spValue->setMaximum(99999999999999);
	_spValue->setDecimals(6);
	_spValue->setMaximumWidth(200);

	layout->addWidget(_lbl);
	layout->addWidget(_spValue);
}

void ParameterWidget::setValue(double value)
{
	_spValue->setValue(value);
}

void ParameterWidget::setName(const QString &name)
{
	_lbl->setText(name);
}

double ParameterWidget::getValue() const
{
	return _spValue->value();
}

//----------------------------------------------------------

ParametersView::ParametersView(NetworkConnection *connection, QWidget *parent) :
	AbstractView(connection, QImage(":/toolbar/upload"), parent),
	ui(new Ui::ParametersView)
{
	ui->setupUi(this);
	ui->frameParamsLayout->addStretch();
	ui->frameParamsLayout->setSpacing(6);

	_connection->registerRobotResponder(this);
	_connection->registerNetworkResponder(this);

	connectionStatusChanged(_connection->getConnectionStatus());

	connect(ui->btnSend, SIGNAL(clicked()), this, SLOT(sendParameters()));
	connect(ui->btnRefresh, SIGNAL(clicked()), this, SLOT(refreshParameters()));

	connect(ui->btnEditSendAutoStart, SIGNAL(clicked()), this, SLOT(editSendAutoStrategy()));
	connect(ui->btnRefreshAutoStart, SIGNAL(clicked()), this, SLOT(refreshAutoStrategy()));
}

ParametersView::~ParametersView()
{
	delete ui;
}

void ParametersView::connectionStatusChanged(NetworkConnection::ConnectionStatus status)
{
	switch(status)
	{
		case NetworkConnection::Lookup:
		case NetworkConnection::Disconnected:
			ui->gbAutoStart->setEnabled(false);
			ui->btnSend->setVisible(false);
			ui->btnRefresh->setVisible(false);
			ui->frameParams->setVisible(false);
			ui->lblParametersTitle->setVisible(false);
			break;
		case NetworkConnection::Connected:
			ui->gbAutoStart->setEnabled(true);
			refreshAutoStrategy();
			ui->btnSend->setVisible(false);
			ui->btnRefresh->setVisible(false);
			ui->frameParams->setVisible(false);
			ui->lblParametersTitle->setVisible(false);
			break;
		case NetworkConnection::Controled:
			refreshParameters();
			ui->btnSend->setVisible(true);
			ui->btnRefresh->setVisible(true);
			ui->frameParams->setVisible(true);
			ui->lblParametersTitle->setVisible(true);
			break;
	}
}

void ParametersView::setStrategies(const QStringList &strategies)
{
	ui->cbAutoStrategy->clear();
	ui->cbAutoStrategy->addItems(strategies);

	updateAutoStrategyWidget();
}

void ParametersView::setSerialPorts(const QStringList &ports)
{
	ui->cbAutoRobotPort->clear();
	ui->cbAutoRobotPort->addItems(ports);

	ui->cbAutoAx12Port->clear();
	ui->cbAutoAx12Port->addItems(ports);

	updateAutoStrategyWidget();
}

void ParametersView::autoStrategyInfo(bool enabled, int strategyNum, const QString &robotPort, const QString &ax12Port, bool simulation, bool mirror, int delayInSeconds)
{
	ui->checkAutoStratEnabled->setChecked(enabled);

	_currentStrategyNum = strategyNum;
	_currentAutoRobotPort = robotPort;
	_currentAutoAx12Port = ax12Port;
	_currentAutoStartDelay = delayInSeconds;

	int type;
	if (!simulation)
		type = 0;
	else if (mirror)
		type = 2;
	else
		type = 1;
	ui->cbAutoType->setCurrentIndex(type);

	updateAutoStrategyWidget();
}

void ParametersView::parameters(const QList<float> &values)
{
	if (values.count() > _params.count())
	{
		for(int i = _params.count(); i < values.count(); ++i)
			createParameterWidget();
	}
	else if (values.count() < _params.count())
	{
		while (values.count() != _params.count())
		{
			ParameterWidget* lastParamWidget = _params.takeLast();
			ui->frameParamsLayout->removeWidget(lastParamWidget);
			delete lastParamWidget;
		}
	}

	Q_ASSERT(values.count() == _params.count());
	for(int i = 0; i < values.count(); ++i)
		_params[i]->setValue(values[i]);
}

void ParametersView::parameterNames(const QStringList &names)
{
	for(int i = 0; i < names.count(); ++i)
	{
		ParameterWidget* paramWidget = _params.value(i, 0);
		if (paramWidget)
			paramWidget->setName(names[i]);
	}
}

void ParametersView::createParameterWidget()
{
	ParameterWidget* paramWidget = new ParameterWidget(this);
	ui->frameParamsLayout->insertWidget(_params.count(), paramWidget);
	_params << paramWidget;
}

void ParametersView::updateAutoStrategyWidget()
{
	ui->cbAutoStrategy->setCurrentIndex(_currentStrategyNum);

	int robotPortIndex = ui->cbAutoRobotPort->findText(_currentAutoRobotPort);
	if (robotPortIndex < 0)
	{
		ui->cbAutoRobotPort->addItem(_currentAutoRobotPort);
		robotPortIndex = ui->cbAutoRobotPort->count() - 1;
	}
	ui->cbAutoRobotPort->setCurrentIndex(robotPortIndex);

	int ax12PortIndex = ui->cbAutoAx12Port->findText(_currentAutoAx12Port);
	if (ax12PortIndex < 0)
	{
		ui->cbAutoAx12Port->addItem(_currentAutoAx12Port);
		ax12PortIndex = ui->cbAutoAx12Port->count() - 1;
	}
	ui->cbAutoAx12Port->setCurrentIndex(ax12PortIndex);
	
	ui->spDelay->setValue(_currentAutoStartDelay);
}

void ParametersView::refreshParameters()
{
	_connection->getComm()->askParameters();
}

void ParametersView::sendParameters()
{
	QList<float> values;
	foreach(ParameterWidget* paramWidget, _params)
	{
		float v = paramWidget->getValue();
		values << v;
		paramWidget->setValue(v);
	}

	_connection->getComm()->setParameters(values);
}

void ParametersView::editSendAutoStrategy()
{
	if (ui->autoStartContentWidget->isEnabled())
	{
		//send
		int type = ui->cbAutoType->currentIndex();
		bool simulation = type >= 1; //1 or 2
		bool mirrored = type == 2;

		_connection->getComm()->setAutoStrategy(ui->checkAutoStratEnabled->isChecked(), ui->cbAutoStrategy->currentIndex(),
												ui->cbAutoRobotPort->currentText(), ui->cbAutoAx12Port->currentText(), 
												simulation, mirrored, ui->spDelay->value());

		ui->autoStartContentWidget->setEnabled(false);
		ui->btnEditSendAutoStart->setText("Edit");
	}
	else
	{
		//edit
		ui->autoStartContentWidget->setEnabled(true);
		ui->btnEditSendAutoStart->setText("Send");
	}
}

void ParametersView::refreshAutoStrategy()
{
	_connection->getComm()->askAutoStrategyInfo();
}


