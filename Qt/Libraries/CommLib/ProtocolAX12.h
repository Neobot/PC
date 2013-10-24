#ifndef PROTOCOLAX12_H
#define PROTOCOLAX12_H

#include "AbstractProtocol.h"

namespace Comm
{
    /**
	 * \brief AX-12 communication protocol.
	 */

    class ProtocolAX12 : public AbstractProtocol
    {
	Q_OBJECT

    public:
	enum Instructions
	{
	    PING = 0x01,
	    READ_DATA = 0x02,
	    WRITE_DATA = 0x03,
	    REG_WRITE = 0x04,
	    ACTION = 0x05,
	    RESET = 0x06,
        SYNC_WRITE = 0x83,
        SYNC_READ = 0x84 //Controller only
	};

	ProtocolAX12(QIODevice* device, Tools::AbstractLogger* logger = 0, bool bufferMode = true);

	/**
	  * \brief Send a message on the device.
	  */
	void sendMessage(quint8 address, quint8 instruction, const Data &data) const;

    private:
	enum MessageStates{FirstFF, SecondFF, ID, Length, Instruction, ReadingData, Checksum};

	Data       _currentData;
	Data       _checksumData;
	quint8		_currentID;
	quint8     _currentInstruction;
	quint8     _currentLength;
	quint8     _currentByteRead;
	MessageStates _currentState;

    public slots:
	void read();
    };
}

#endif // PROTOCOLAX12_H
