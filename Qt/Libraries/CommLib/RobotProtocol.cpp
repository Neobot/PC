#include "RobotProtocol.h"
#include "CommException.h"
#include "Instructions.h"
#include <QtDebug>

using namespace Comm;
using namespace Tools;

RobotProtocol::RobotProtocol(QIODevice* device, AbstractLogger* logger, bool bufferMode)
	: AbstractProtocol(device, logger), _currentState(FirstFF), _maxLength(MAX_LENGTH)
{
	addLengthExceptionCommand(LOG);
	addLengthExceptionCommand(PARAMETERS);
	addLengthExceptionCommand(PARAMETER_NAMES);

    if (bufferMode)
        run();
}


void RobotProtocol::sendMessage(quint8 address, quint8 instruction, const Data &data) const
{
    Q_UNUSED(address);

    if (!_device->isOpen())
    {
	    if (_showError)
		    logger() << "Protocol: device not opened." << Tools::endl;
	    return;
    }

    Data fullFrameData;
    Data checksumData;  //Used to calculate the checksum

    quint8 length = data.length() + 1; //data length + instruction

    //Calculate the checksum
    checksumData.add(length);
    checksumData.add(instruction);
    checksumData.add(data);
    quint8 checksum = (int)checksumData.checksum();

    fullFrameData.add((quint8)0xFF);
    fullFrameData.add((quint8)0xFF);
    fullFrameData.add(checksumData);
    fullFrameData.add(checksum);

    // Write the data to the device
    if(_device->write(fullFrameData) == -1 && _showError)
		logger() << "Protocol: writing failed." << Tools::endl;

    if (_messages)
		logger() << "Send Message : " << length << " bytes: " << (unsigned int)instruction << " - " << data << Tools::endl;
}

void RobotProtocol::setMaxLength(int maxLength)
{
	_maxLength = maxLength;
}

void RobotProtocol::addLengthExceptionCommand(int instruction)
{
	_noMaxLengthCommands << instruction;
}

void RobotProtocol::read()
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
        case FirstFF:
        case SecondFF:          
            if (value != 0xFF)
            {
		if (_showError)
		    logger() << "Invalid Frame (" << value << "): Expected '0xFF0xFF' at the begining." << Tools::endl;
                incrementErrorCounter();
                _currentState = FirstFF;
                break;
            }
            _currentData.clear();
            _checksumData.clear();
            _currentByteRead = 0;
            _currentInstruction = 0;
            if (_currentState == FirstFF)
                _currentState = SecondFF;
            else
                _currentState = Lenght;
            break;
        case Lenght:
             _currentLenght = value;
             _checksumData.add(value);
             _currentState = Instruction;
             break;
        case Instruction:
			 _currentInstruction = value;
			 if (_maxLength > 0 && _currentLenght > _maxLength && !_noMaxLengthCommands.contains(_currentInstruction))
			 {
				 if (_showError)
					 logger() << "Invalid Frame (" << value << "): The length is greater than" << MAX_LENGTH << ". This is probably an error." << Tools::endl;
				 incrementErrorCounter();
				 _currentState = FirstFF;
				 break;
			 }

             _checksumData.add(_currentInstruction);
             if (_currentLenght > 1)
                _currentState = ReadingData;
             else
                 _currentState = Checksum;
             break;
         case ReadingData:
             if (_currentLenght == 0)
             {
                 _currentState = Checksum;
                 break;
             }

             _checksumData.add(value);
             _currentData.add(value);
             ++_currentByteRead;
             if (_currentByteRead == _currentLenght - 1)
                 _currentState = Checksum;
             break;
         case Checksum:
             quint8 calcChecksum = _checksumData.checksum();
             _currentState = FirstFF;
             if (value != calcChecksum)
             {
		 if (_showError)
		    logger() << "Invalid checksum." << "Expected:" << calcChecksum << "Found:" << value << Tools::endl;
                 incrementErrorCounter();
                 break;
             }

	     if (_frames)
                 logger() << " - Valid message." << Tools::endl;

             resetErrorCounter();
	     if (_messages)
                 logger() << "Received Message : " << _currentLenght << " bytes: " << (unsigned int)_currentInstruction << " - " << _currentData << Tools::endl;

			 emit message(_currentInstruction, _currentData, 0);
             break;
         }
    }

    if (_frames)
        logger() << Tools::endl;
}
