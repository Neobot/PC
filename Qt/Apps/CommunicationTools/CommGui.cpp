#include "CommGui.h"
#include "CommTools.h"
#include "Instructions.h"
#include "CommException.h"

#include <QFile>
#include <QtDebug>
#include <QMessageBox>
#include <QSerialPortInfo>

#define AUTO_SEND_INTERVAL 1

using namespace Comm;

CommGui::CommGui(QWidget *parent) :
	QDialog(parent), _openedProtocolType(None), _showValidMessages(true), _connected(false)
{
    ui.setupUi(this);

    _logger = new Tools::PlainTextEditLogger(ui.textEdit);
    setLogger(_logger);

    _timerSend = new QTimer(this);
    connect(_timerSend, SIGNAL(timeout()), this, SLOT(timerTick()));

	QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
	foreach(const QSerialPortInfo& port, ports)
		ui.cbPort->insertItem(0, port.portName());

	ui.cbPort->setCurrentIndex(0);

   setGuiState(false);
}

CommGui::~CommGui()
{
    delete _protocol;
    _protocol = 0;

    delete _port;
    _port = 0;

    delete _logger;
	_logger = 0;
}

void CommGui::openConnection()
{
	if (!_connected && openPort(ui.cbPort->currentText(), ui.cbBaudRate->currentText(), static_cast<ProtocolType>(ui.cbProtocol->currentIndex())))
	{
		_connected = true;
        setGuiState(_connected);
	}
}

void CommGui::closeConnection()
{
	if (_connected && closePort())
	{
		_connected = false;
        setGuiState(_connected);
	}
}

void CommGui::parseData(Data& data) const
{
    QString text = ui.leData->text();
    if (text.isEmpty())
        return;

    if (ui.rbInputDec->isChecked())
    {
        QStringList tokens = text.split(' ');

        foreach(QString d, tokens)
        {
            quint8 a = d.toInt();
            data.add(a);
        }
    }
    else if (ui.rbInputHex->isChecked())
    {
        QStringList tokens = text.split(' ');

        foreach(QString d, tokens)
        {
            if (d.length() == 2)
            {
                bool ok;

                quint8 a = (quint8)d.toUShort(&ok, 16);

                if (ok)
                    data.add(a);
            }
        }
    }
    else		// ASCII
    {
        for (int i = 0; i < text.length(); i++)
        {
            quint8 a = text.at(i).toLatin1();

            if (a != 0)
                data.add(a);
        }
    }
}

void CommGui::messageReceived(quint8 instruction, const Comm::Data& data, quint8 id)
{
    if (_showValidMessages)
    {
		switch(_openedProtocolType)
		{
			case Robot:
				logger() << "[" << QTime::currentTime().toString("hh:mm:ss") << "] Received: " << instruction << " - " << data << Tools::endl;
				break;
			case AX12:
				logger() << "[" << QTime::currentTime().toString("hh:mm:ss") << "] Received from  addr " << id << ": " << instruction << " " << data << Tools::endl;
				break;
			case UM6:
				logger() << "[" << QTime::currentTime().toString("hh:mm:ss") << "] Received: " << instruction << " - " << data << Tools::endl;
				break;
			case Raw:
				logger() << "[" << QTime::currentTime().toString("hh:mm:ss") << "] Received: " << data << Tools::endl;
				break;
			default:
				logger() << "[" << QTime::currentTime().toString("hh:mm:ss") << "] Unknown message type received from  addr " << id << ": " << instruction << " " << data << Tools::endl;
		}
    }
}

bool CommGui::openPort(const QString& portname, const QString& baudrate, ProtocolType protocol)
{
	QSerialPort* port = new QSerialPort(portname);
	_port = port;
	connect(port, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleSerialError(QSerialPort::SerialPortError)));

	switch(protocol)
	{
		case Robot:
			_protocol = new RobotProtocol(port, _logger, true);
			break;
		case AX12:
			_protocol = new ProtocolAX12(port, _logger, true);
			break;
		case UM6:
			_protocol = new ProtocolUM6(port, _logger, true);
			break;
		case Raw:
			_protocol = new ProtocolRaw(port, _logger, true);
			break;
		default:
			_protocol = 0;
			logger() << "Unknow protocol. Opening failed..." << Tools::endl;
			return false;
	}

    _protocol->beQuiet(true);
    _protocol->setDebugMode(true, false, false);

    logger() << "[" << QTime::currentTime().toString("hh:mm:ss") << "] Opening '"  << portname << "'..." << Tools::endl;

    if (_protocol->open())
    {
        logger() << "[" << QTime::currentTime().toString("hh:mm:ss") << "] Port '"  << portname << "' successfully opened" << Tools::endl;

		port->setBaudRate(baudrate.toInt());
		port->setFlowControl(QSerialPort::NoFlowControl);
		port->setParity(QSerialPort::NoParity);
		port->setDataBits(QSerialPort::Data8);
		port->setStopBits(QSerialPort::OneStop);
    }
    else
    {
        logger() << "[" << QTime::currentTime().toString("hh:mm:ss") << "] Failed to open port '"  << portname << "'" << Tools::endl;
        delete _protocol; _protocol = 0;
        return false;
    }

    if (!connect(_protocol, SIGNAL(message(quint8, const Comm::Data&, quint8)), this, SLOT(messageReceived(quint8, const Comm::Data&, quint8))))
    {
        logger() << "[" << QTime::currentTime().toString("hh:mm:ss") << "] The connection to the slot MessageReceived() failed !";
        return false;
    }

	_openedProtocolType = protocol;

	return _port;
}

bool CommGui::closePort()
{
    if (_protocol->close())
    {
        delete _protocol;
        _protocol = 0;

		if (_port)
			_port->disconnect();
        delete _port;
        _port = 0;

		_openedProtocolType = None;

        logger() << "[" << QTime::currentTime().toString("hh:mm:ss") << "] Port closed" << Tools::endl;

        return true;
    }
    else
    {
        logger() << "[" << QTime::currentTime().toString("hh:mm:ss") << "] Unable to close port" << Tools::endl;

        return false;
    }
}

void CommGui::setGuiState(bool connectionOpened)
{
	ui.btnConnect->setText(connectionOpened ? "Close" : "Open");
    ui.cbPort->setEnabled(!connectionOpened);
    ui.cbBaudRate->setEnabled(!connectionOpened);
    ui.cbProtocol->setEnabled(!connectionOpened);
    ui.gbSendMessage->setEnabled(connectionOpened);
    ui.cbQuiet->setEnabled(connectionOpened);
    ui.cbDebugMode->setEnabled(connectionOpened);
    ui.cbValidMessage->setEnabled(connectionOpened);
    ui.btnStopRun->setEnabled(connectionOpened);
}

void CommGui::send(bool log)
{
    try
    {
        Data data;
        parseData(data);

		switch(_openedProtocolType)
		{
			case Robot:
				_protocol->sendMessage((quint8)ui.sbInstruction->value(), data);
				if (log)
					logger() << "[" << QTime::currentTime().toString("hh:mm:ss") << "] Send: " << ui.sbInstruction->value() << " - " << data << Tools::endl;
				break;
			case AX12:
				_protocol->sendMessage((quint8)ui.sbID->value(), (quint8)ui.sbInstruction->value(), data);
				if (log)
					logger() << "[" << QTime::currentTime().toString("hh:mm:ss") << "] Send to addr " << ui.sbID->value() << ": " << ui.sbInstruction->value() << " - " << data << Tools::endl;
				break;
			case UM6:
				//_protocol->sendMessage(data);
				logger() << "Sending data not implemented yet for protocol UM6!";
				break;
			case Raw:
				_protocol->sendMessage(data);
				if (log)
					logger() << "[" << QTime::currentTime().toString("hh:mm:ss") << "] Send: " << data << Tools::endl;
				break;
			default:
				_protocol->sendMessage((quint8)ui.sbID->value(), (quint8)ui.sbInstruction->value(), data);
				if (log)
					logger() << "[" << QTime::currentTime().toString("hh:mm:ss") << "] Send unknown message type to addr " << ui.sbID->value() << ": " << ui.sbInstruction->value() << " - " << data << Tools::endl;
				break;
		}
    }
    catch(const CommException& e)
    {
        if (log)
            logger() << "[" << QTime::currentTime().toString("hh:mm:ss") << "] Error: " << e.what();
    }
    catch(...)
    {
        if (log)
            logger() << "[" << QTime::currentTime().toString("hh:mm:ss") << "] Error: Unsupported Error";
    }
}

void CommGui::timerTick()
{
    send(false);
}

void CommGui::on_btnConnect_clicked()
{
    if (!_connected)
		openConnection();
    else
		closeConnection();
}

void CommGui::on_btnSend_clicked()
{
    if (!ui.btnSend->isCheckable())
    {
        send();
    }
    else if (ui.btnSend->isChecked())
    {
        _timerSend->start(AUTO_SEND_INTERVAL);
    }
    else
    {
        _timerSend->stop();
    }
}

void CommGui::on_cbQuiet_stateChanged(int state)
{
    _protocol->beQuiet(state != Qt::Checked);
}

void CommGui::on_cbValidMessage_stateChanged(int state)
{
    _showValidMessages = state == Qt::Checked;
}

void CommGui::on_cbDebugMode_stateChanged(int state)
{
    _protocol->setDebugMode(true, false, state == Qt::Checked);
}

void CommGui::on_btnClear_clicked()
{
    ui.textEdit->clear();
}

void CommGui::on_btnStopRun_clicked(bool value)
{
    if (value)
    {
        ui.btnStopRun->setText("Run");
        _protocol->stop();
    }
    else
    {
        ui.btnStopRun->setText("Stop");
        _protocol->run();
    }
}

void CommGui::on_cbProtocol_currentIndexChanged(int index)
{
	switch(index)
	{
		case Robot:
			ui.lblInstruction->setVisible(true);
			ui.sbInstruction->setVisible(true);
			ui.lblID->setVisible(false);
			ui.sbID->setVisible(false);
			break;
		case AX12:
			ui.lblInstruction->setVisible(true);
			ui.sbInstruction->setVisible(true);
			ui.lblID->setVisible(true);
			ui.sbID->setVisible(true);
			break;
		case UM6:
			ui.lblInstruction->setVisible(false);
			ui.sbInstruction->setVisible(false);
			ui.lblID->setVisible(false);
			ui.sbID->setVisible(false);
			break;
		case Raw:
			ui.lblInstruction->setVisible(false);
			ui.sbInstruction->setVisible(false);
			ui.lblID->setVisible(false);
			ui.sbID->setVisible(false);
			break;
    }
}

void CommGui::on_rbOutputDec_toggled(bool checked)
{
    if (checked)
    {
        _logger->setBase(10);
        _logger->displayCharsAsNumbers(true);
    }
}

void CommGui::on_rbOutputHex_toggled(bool checked)
{
    if (checked)
    {
        _logger->setBase(16);
        _logger->displayCharsAsNumbers(true);
    }
}

void CommGui::on_rbOutputASCII_toggled(bool checked)
{
    if (checked)
    {
        _logger->setBase(10);
        _logger->displayCharsAsNumbers(false);
    }
}

void CommGui::on_cbSendContinuously_stateChanged(int state)
{
    if (state == Qt::Checked)
    {
        ui.btnSend->setCheckable(true);
    }
    else
    {
        ui.btnSend->setChecked(false);
        ui.btnSend->setCheckable(false);
        _timerSend->stop();
    }
}


void CommGui::handleSerialError(QSerialPort::SerialPortError error)
{
	switch(error)
	{
		case QSerialPort::ResourceError:
			logger() << "Serial Connection lost" << Tools::endl;
			closeConnection();
			break;
		case QSerialPort::NoError:
			break;
		default:
			logger() << "Serial Error: " << _protocol->getIODevice()->errorString() << Tools::endl;
	}
}
