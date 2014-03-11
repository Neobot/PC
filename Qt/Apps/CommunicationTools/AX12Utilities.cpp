#include "AX12Utilities.h"
#include "ui_AX12Utilities.h"

#include "AX12CommManager.h"

#include <QSerialPortInfo>

using namespace Comm;

AX12Utilities::AX12Utilities(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AX12Utilities), _manager(0)
{
	ui->setupUi(this);

	_logger = new Tools::PlainTextEditLogger(ui->plainTextEdit);
	setLogger(_logger);

	QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
	foreach(const QSerialPortInfo& port, ports)
		ui->cbPort->insertItem(0, port.portName());

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

		_manager = new AX12CommManager(ui->cbPort->currentText(), baudrate.toInt(), AX12CommManager::USB2AX_CONTROLLER, _logger);
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

		connect(_manager, SIGNAL(servosStatusUpdated(QList<quint8>)), this, SLOT(scanServosStatusUpdated(QList<quint8>)));
		connect(_manager, SIGNAL(requestTimeoutReceived(QList<quint8>)), this, SLOT(scanRequestTimeoutReceived(QList<quint8>)));
        connect(_manager, SIGNAL(allMessagesSent()), this, SLOT(allIdReceived()));

		if (ui->rbAnyID->isChecked() || ui->rbAnyAny->isChecked())
		{
            QList<quint8> ids;
			for(quint8 i = 0; i < 254; ++i)
                ids << i;
            _manager->requestServoStatus(ids);
		}
		else
		{
            _manager->requestServoStatus(ui->spScannedId->value());
		}
	}
	else
	{
		logger() << "Scan finished..." << Tools::endl;
		closeConnection();
	}
}

void AX12Utilities::scanServosStatusUpdated(const QList<quint8> &ids)
{
    foreach(quint8 id, ids)
        logger() << "ID " << id << " found!" << Tools::endl;
}

void AX12Utilities::scanRequestTimeoutReceived(const QList<quint8>& ids)
{
    Q_UNUSED(ids);
}

void AX12Utilities::allIdReceived()
{
	QTimer::singleShot(100, this, SLOT(scanNextBaudrate()));
}
