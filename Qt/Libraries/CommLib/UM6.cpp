#include "UM6.h"

using namespace Comm;

UM6::UM6(const QString &portname, qint32 baudrate, Tools::AbstractLogger *logger) :
	Tools::LoggerInterface(logger), _baudrate(baudrate), _gyrosCalibrated(false)
{
	QSerialPort* port = new QSerialPort(portname);
	connect(port, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleSerialError(QSerialPort::SerialPortError)));

	_protocol = new ProtocolUM6(port, logger, true);
	_protocol->beQuiet(true);
	_protocol->setDebugMode(true, false, false);

	for (int i = 0; i < 133; i++)
		_registers[i] = 0;
}

UM6::~UM6()
{
	if (_protocol)
	{
		_protocol->close();
		delete _protocol->getIODevice();
	}

	delete _protocol;
}

bool UM6::open()
{
	bool ok = _protocol->open();

	if (ok)
	{
		QSerialPort* port = static_cast<QSerialPort*>(_protocol->getIODevice());

		port->setBaudRate(_baudrate);
		port->setFlowControl(QSerialPort::NoFlowControl);
		port->setParity(QSerialPort::NoParity);
		port->setDataBits(QSerialPort::Data8);
		port->setStopBits(QSerialPort::OneStop);

		if (!connect(_protocol, SIGNAL(message(quint8, const Comm::Data&)), this, SLOT(messageReceived(quint8, const Comm::Data&))))
		{
			logger() << "The connection to the slot messageReceived() failed !" << Tools::endl;
		}
	}
	else
	{
		logger() << "Warning: UM6 port cannot be opened." << Tools::endl;
	}

	return ok;
}

void UM6::messageReceived(quint8 registerAddress, const Comm::Data& data)
{
	if (registerAddress == 0xac)	// calibrate gyros success
	{
		_gyrosCalibrated = true;
	}
	else		// data received
	{
		// extract registers from data
		Comm::Data d(data);
		quint8 currentRegister = registerAddress;

		while (!d.isEmpty() && currentRegister < 133)
		{
			d.take(_registers[currentRegister]);
			currentRegister++;
		}
	}
}

void UM6::calibrateGyros()
{
	Data data(Comm::LittleEndian);

	_protocol->sendMessage(0xac, 0, data);
}

quint32 UM6::getRegister(quint8 address)
{
	if (address < 133)
		return _registers[address];
	else
		return 0;
}

qint16 UM6::get16BitRegister(quint8 address, quint8 sel)
{
	if (address < 133 && sel >= 0 && sel <= 1)
	{
		if (sel == 0)
			return (qint16)(_registers[address] >> 16);
		else
			return (qint16)_registers[address];
	}
	else
		return 0;
}

float UM6::getPhi()
{
	return (float)get16BitRegister(0x62, 0) * 0.0109863;
}

float UM6::getTheta()
{
	return (float)get16BitRegister(0x62, 1) * 0.0109863;
}

float UM6::getPsi()
{
	return (float)get16BitRegister(0x63, 0) * 0.0109863;
}

bool UM6::gyrosCalibrated()
{
	return _gyrosCalibrated;
}

void UM6::handleSerialError(QSerialPort::SerialPortError error)
{
	switch(error)
	{
		case QSerialPort::ResourceError:
			_protocol->close();
			break;
		case QSerialPort::NoError:
			break;
		default:
			logger() << "Warning: Serial Error: " << _protocol->getIODevice()->errorString() << Tools::endl;
	}
}
