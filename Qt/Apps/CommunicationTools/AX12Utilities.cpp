#include "AX12Utilities.h"
#include "ui_AX12Utilities.h"
#include "CommUtil.h"

#include "AX12CommManager.h"

#include <qextserialenumerator.h>

using namespace Comm;

AX12Utilities::AX12Utilities(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AX12Utilities), _manager(0)
{
	ui->setupUi(this);

	_logger = new Tools::PlainTextEditLogger(ui->plainTextEdit);
	setLogger(_logger);

	QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
	foreach(const QextPortInfo& port, ports)
		ui->cbPort->insertItem(0, port.portName);

	ui->cbPort->setCurrentIndex(0);

	connect(ui->btnScan, SIGNAL(clicked()), this, SLOT(scan()));
}

AX12Utilities::~AX12Utilities()
{
	delete ui;
}

bool AX12Utilities::openConnection(const QString& baudrate)
{
	if (!_manager)
	{
		ui->gbScan->setEnabled(false);

		_manager = new AX12CommManager(ui->cbPort->currentText(), CommUtil::getQextSerialPortBaudrate(baudrate), _logger);
		return _manager->open();
	}
	else
		return false;
}

void AX12Utilities::closeConnection()
{
	delete _manager;
	_manager = 0;
	ui->gbScan->setEnabled(true);
}

void AX12Utilities::scan()
{
	_remainingBaudrateToScan.clear();
	if (ui->rbAnyID->isChecked())
		_remainingBaudrateToScan << ui->cbBaudRate->currentText();
	else if (ui->rbAnyBaudrate->isChecked() || ui->rbAnyAny->isChecked())
	{
		for(int i = 0; i < ui->cbBaudRate->count(); ++i)
			_remainingBaudrateToScan << ui->cbBaudRate->itemText(i);
	}

	scanNextBaudrate();
}

void AX12Utilities::scanNextBaudrate()
{
	if (!_remainingBaudrateToScan.isEmpty())
	{
		QString baudrate = _remainingBaudrateToScan.takeFirst();
		logger() << "Scanning baudrate " << baudrate << Tools::endl;

		closeConnection();
		if (!openConnection(baudrate))
			logger() << "Error: Cannot open the connection" << Tools::endl;

		_manager->clearServoList();
		if (ui->rbAnyID->isChecked() || ui->rbAnyAny->isChecked())
		{
			for(quint8 i = 0; i < 254; ++i)
				_manager->addServo(i);
		}
		else
		{
			_manager->addServo(ui->spScannedId->value());
		}

		connect(_manager, SIGNAL(servosStatusUpdated(quint8)), this, SLOT(scanServosStatusUpdated(quint8)));
		connect(_manager, SIGNAL(requestTimeoutReceived(quint8)), this, SLOT(scanRequestTimeoutReceived(quint8)));
		connect(_manager, SIGNAL(allMessagesSent()), this, SLOT(allIdReceived()));

		_manager->requestAllServoStatus();
	}
	else
	{
		logger() << "Scan finished..." << Tools::endl;
		closeConnection();
	}
}

void AX12Utilities::scanServosStatusUpdated(quint8 id)
{
	logger() << "ID " << id << " found!" << Tools::endl;
}

void AX12Utilities::scanRequestTimeoutReceived(quint8 id)
{
	Q_UNUSED(id);
}

void AX12Utilities::allIdReceived()
{
	QTimer::singleShot(100, this, SLOT(scanNextBaudrate()));
}
