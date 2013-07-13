#include "ProtocolUM6.h"
#include "CommException.h"
#include <QtDebug>

using namespace Comm;
using namespace Tools;

ProtocolUM6::ProtocolUM6(QIODevice* device, AbstractLogger* logger, bool bufferMode)
	: AbstractProtocol(device, logger), _currentState(StartChar1)
{
	if (bufferMode)
		run();

	_currentData.setEndianness(BigEndian);
}

void ProtocolUM6::sendMessage(quint8 address, quint8 instruction, const Data& data) const
{
	// send command to UM6. address=command. data & instruction not used
	if (!_device->isOpen())
	{
		if (_showError)
			logger() << "Protocol: device not opened." << Tools::endl;
		return;
	}

	Data fullFrameData;

	fullFrameData.add((quint8)'s');
	fullFrameData.add((quint8)'n');
	fullFrameData.add((quint8)'p');
	fullFrameData.add((quint8)0x00); // packet type, 0 for command
	fullFrameData.add((quint8)address);

	quint16 checksum = fullFrameData.checksum16bits();
	fullFrameData.add((quint16)checksum);

	// Write the data to the device
	if(_device->write(fullFrameData) == -1 && _showError)
		logger() << "Protocol: writing failed." << Tools::endl;

	if (_messages)
		logger() << "Send Message to UM6 : command " << address << Tools::endl;
}

void ProtocolUM6::read()
{
	// Buffer mode
	QDataStream in(_device);
	in.setByteOrder(QDataStream::BigEndian);

	while (_device->bytesAvailable())
	{
		quint8 value;
		in >> value;

		if (_frames)
			logger() << ' ' << value;

		switch(_currentState)
		{
			case StartChar1:
				if (value != 's')
					break;

				_currentData.clear();
				_checksumData.clear();
				_checksumData.add(value);
				_currentByteRead = 0;
				_currentState = StartChar2;
				break;

			case StartChar2:
				if (value != 'n')
				{
					if (_showError)
						logger() << "Invalid Frame (" << value << "): Expected 'snp' at the beginning." << Tools::endl;

					incrementErrorCounter();
					_currentState = StartChar1;
					break;
				}
				_checksumData.add(value);
				_currentState = StartChar3;
				break;

			case StartChar3:
				if (value != 'p')
				{
					if (_showError)
						logger() << "Invalid Frame (" << value << "): Expected 'snp' at the beginning." << Tools::endl;

					incrementErrorCounter();
					_currentState = StartChar1;
					break;
				}
				_checksumData.add(value);
				_currentState = PacketType;
				break;

			case PacketType:
				if (value < 192 &&  // data packet : 1 1 xxxx 00 hasData-isBatch-length-no error
					value != 0)		// command success packet : 0 0 0000 00
					break;

				if (value >= 192)
					_currentLength = value - 192;
				else
					_currentLength = 0;

				_checksumData.add(value);
				_currentState = Address;
				break;

			case Address:
				_currentAddress = value;
				_checksumData.add(value);

				if (_currentLength == 0)
					_currentState = Checksum1;
				else
					_currentState = ReadingData;

				break;

			case ReadingData:
				_checksumData.add(value);
				_currentData.add(value);
				++_currentByteRead;
				if (_currentByteRead == _currentLength)
					_currentState = Checksum1;
				break;

			case Checksum1:
				_readChecksum = value;
				_currentState = Checksum0;
				break;

			case Checksum0:
				quint16 readChecksum = ((quint16)_readChecksum << 8) + (quint16)value;
				quint16 calcChecksum = _checksumData.checksum16bits();

				if (readChecksum != calcChecksum)
				{
					if (_showError)
						logger() << "Invalid checksum." << "Expected:" << calcChecksum << "Found:" << readChecksum << Tools::endl;

					incrementErrorCounter();
					_currentState = StartChar1;
					break;
				}

				if (_frames)
					logger() << " - Valid message." << Tools::endl;

				resetErrorCounter();

				if (_messages)
					logger() << "Received Message from UM6 : address " << _currentAddress << " - " << _currentData << Tools::endl;

				emit message(_currentAddress, _currentData);
				_currentState = StartChar1;
				break;
		}
	}

	if (_frames)
		logger() << Tools::endl;
}
