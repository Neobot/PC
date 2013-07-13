#ifndef NETWORKPROTOCOL_H
#define NETWORKPROTOCOL_H

#include "AbstractProtocol.h"

namespace Comm
{
    /**
	 * \brief Same as #RobotProtocol except message length is on 4 byte
	 * \note Use of templates ?
	 */
	class NetworkProtocol : public AbstractProtocol
    {
        Q_OBJECT

    public:
		NetworkProtocol(QIODevice* device, Tools::AbstractLogger* logger = 0, bool bufferMode = true);

	/**
	  * \brief Send a message on the device.
	  */
	void sendMessage(quint8 address, quint8 instruction, const Data& data) const;

    private:
        enum MessageStates{FirstFF, SecondFF, Lenght, Instruction, ReadingData, Checksum};

        Data       _currentData;
        Data       _checksumData;
		quint8     _currentInstruction;
		quint32    _currentLenght;
		Data	   _lengthData;
		quint32     _currentByteRead;
        MessageStates _currentState;

    public slots:
        void read();
    };
}

#endif // NETWORKPROTOCOL_H
