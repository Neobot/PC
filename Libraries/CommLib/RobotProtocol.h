#ifndef ROBOTPROTOCOL_H
#define ROBOTPROTOCOL_H

#include "AbstractProtocol.h"

namespace Comm
{
    /**
     * \brief A communication protocol based on AX-12.
     *
     * The differences with AX-12 are :
     * - [ID] is nt used.
     * - To avoid some errors, the data have a maximum length of #MAX_LENGTH bytes.
     *
     * The errors and warning messages are written in a logger.
     *
     * It is advised to use the bufferMode.
	 */
	class RobotProtocol : public AbstractProtocol
    {
        Q_OBJECT

    public:
		RobotProtocol(QIODevice* device, Tools::AbstractLogger* logger = 0, bool bufferMode = true);

	/**
	  * \brief Send a message on the device.
	  */
	void sendMessage(quint8 address, quint8 instruction, const Data& data) const;

	void setMaxLength(int maxLength);
	void addLengthExceptionCommand(int instruction);

    private:
        enum MessageStates{FirstFF, SecondFF, Lenght, Instruction, ReadingData, Checksum};

        Data       _currentData;
        Data       _checksumData;
		quint8     _currentInstruction;
		quint8     _currentLenght;
        quint8     _currentByteRead;
        MessageStates _currentState;

		int			_maxLength;
		QList<int>	_noMaxLengthCommands;


    public slots:
        void read();
    };
}

#endif // ROBOTPROTOCOL_H
