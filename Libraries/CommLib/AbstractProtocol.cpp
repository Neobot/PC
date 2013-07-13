#include "AbstractProtocol.h"

using namespace Comm;

const int ERROR_COUNTER_WARNING = 12;

AbstractProtocol::AbstractProtocol(QIODevice* device, Tools::AbstractLogger* logger) :
	QObject(0), LoggerInterface(logger), _device(device), _messages(false), _frames(false), _showError(true), _errorCounter(0)
{
}

QIODevice* AbstractProtocol::getIODevice() const
{
    return _device;
}

void AbstractProtocol::sendMessage(quint8 instruction, const Data &data) const
{
    sendMessage(0, instruction, data);
}

void AbstractProtocol::sendMessage(const Data &data) const
{
	sendMessage(0, 0, data);
}

bool AbstractProtocol::open()
{
	if (!_device->isOpen())
	{
		if (!_device->open(QIODevice::ReadWrite))
		{
			if (_showError)
				logger() << "An error occured while opening the device :" << _device->errorString() << Tools::endl;
			return false;
		}
	}
	else
	{
		if (_messages)
			logger() << "The device is already opened." << Tools::endl;
		return false;
	}

	if (_messages)
		logger() << "The device has been succesfully opened." << Tools::endl;
	return true;
}

bool AbstractProtocol::close()
{
	if (_device->isOpen())
	{
		_device->close();
		if (_messages)
			logger() << "The device has been succesfully closed." << Tools::endl;
		return true;
	}
	else
	{
		if (_messages)
			logger() << "The device is not opened." << Tools::endl;
		return true;
	}
}

bool AbstractProtocol::isOpened() const
{
    return _device->isOpen();
}

void AbstractProtocol::setDebugMode(bool errors, bool messages, bool frames)
{
    _showError = errors;
    _messages = messages;
    _frames = frames;
}

void AbstractProtocol::beQuiet(bool value)
{
    _messages = value;
}

void AbstractProtocol::stop()
{
    if (!disconnect(_device, SIGNAL(readyRead()), this, SLOT(read()))  && _showError)
	logger() << "Protocol disconnection failed !" << Tools::endl;
}

void AbstractProtocol::run()
{
    if (!connect(_device, SIGNAL(readyRead()), this, SLOT(read())) && _showError)
		logger() << "Protocol connection failed !" << Tools::endl;
    else if (_messages)
		logger() << "Protocol connected !" << Tools::endl;
}

void AbstractProtocol::resetErrorCounter()
{
    _errorCounter = 0;
}

void AbstractProtocol::incrementErrorCounter()
{
    ++_errorCounter;
    if (_errorCounter > ERROR_COUNTER_WARNING)
    {
	// This message is displayed event in quiet mode.
	if (_showError)
	    logger() << "More than " << ERROR_COUNTER_WARNING << " consecutive error occured. :(" << Tools::endl;
    }
}

int AbstractProtocol::getErrorsCount() const
{
    return _errorCounter;
}

