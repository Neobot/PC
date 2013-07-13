#include "UM6.h"

using namespace Comm;

UM6::UM6()
{
	_protocol = 0;
}

UM6::UM6(const QString &portname, BaudRateType baudrate, Tools::AbstractLogger *logger) :
	Tools::LoggerInterface(logger), _gyrosCalibrated(false)
{
	QextSerialPort* port = new QextSerialPort(portname,  QextSerialPort::EventDriven);
	port->setBaudRate(baudrate);
	port->setFlowControl(FLOW_OFF);
	port->setParity(PAR_NONE);
	port->setDataBits(DATA_8);
	port->setStopBits(STOP_1);

	_protocol = new ProtocolUM6(port, logger, true);
	_protocol->beQuiet(true);
	_protocol->setDebugMode(true, false, false);

	for (int i = 0; i < 133; i++)
		_registers[i] = 0;
}

bool UM6::open()
{
	bool ok = _protocol->open();

	if (!ok)
	logger() << "Warning: UM6 port cannot be opened." << Tools::endl;


	if (!connect(_protocol, SIGNAL(message(quint8, const Comm::Data&)), this, SLOT(messageReceived(quint8, const Comm::Data&))))
	{
		logger() << "The connection to the slot messageReceived() failed !" << Tools::endl;
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
