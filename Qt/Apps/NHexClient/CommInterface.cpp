#include "CommInterface.h"
#include "RobotProtocol.h"
#include "AX12CommManager.h"

CommInterface::CommInterface() : _comm(0), _connected(false)
{
}

CommInterface::~CommInterface()
{
	delete _protocol;
	delete _port;
	delete _comm;
}

bool CommInterface::openPort(const QString& portname, const QString& baudrate)
{
	QSerialPort* port = new QSerialPort(portname);
	_port = port;
	connect(port, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleSerialError(QSerialPort::SerialPortError)));
	_protocol = new Comm::RobotProtocol(port, 0, true);
	_protocol->beQuiet(true);
	_protocol->setDebugMode(true, false, false);

	if (_protocol->open())
	{
		port->setBaudRate(baudrate.toInt());
		port->setFlowControl(QSerialPort::NoFlowControl);
		port->setParity(QSerialPort::NoParity);
		port->setDataBits(QSerialPort::Data8);
		port->setStopBits(QSerialPort::OneStop);
	}
	else
	{
		delete _protocol; _protocol = 0;
		return false;
	}

	_comm = new Comm::RobotCommInterface(_protocol, new Comm::AX12CommManager, 0, this, this);
	changeStatus(true);

	return true;
}

bool CommInterface::closePort()
{
	if (_protocol->close())
	{
		delete _protocol;
		_protocol = 0;

		if (_port)
			_port->disconnect();
		delete _port;
		_port = 0;
		changeStatus(false);

		//logger() << "[" << QTime::currentTime().toString("hh:mm:ss") << "] Port closed" << Tools::endl;

		return true;
	}
	else
	{
		//logger() << "[" << QTime::currentTime().toString("hh:mm:ss") << "] Unable to close port" << Tools::endl;

		return false;
	}
}

void CommInterface::handleSerialError(QSerialPort::SerialPortError error)
{
	switch(error)
	{
		case QSerialPort::ResourceError:
			//logger() << "Serial Connection lost" << Tools::endl;
			changeStatus(false);
			break;
		case QSerialPort::NoError:
			break;
		default:
			//logger() << "Serial Error: " << _protocol->getIODevice()->errorString() << Tools::endl;
			break;
	}
}

Comm::RobotCommInterface *CommInterface::getComm() const
{
	return _comm;
}

void CommInterface::changeStatus(bool connected)
{
	if (_connected != connected)
	{
		_connected = connected;
		emit statusChanged(_connected);
	 }
}
