#ifndef PROTOCOLUM6_H
#define PROTOCOLUM6_H

#include "AbstractProtocol.h"

namespace Comm
{

	/**
	 * \brief Communication protocol for UM6 Orientation Sensor
	 */

	class ProtocolUM6 : public AbstractProtocol
	{
	Q_OBJECT

	public:

		ProtocolUM6(QIODevice* device, Tools::AbstractLogger* logger = 0, bool bufferMode = true);

		/**
		  * \brief Send a message on the device.
		  */
		void sendMessage(quint8 address, quint8 instruction, const Data &data) const;

	private:
		enum MessageStates{StartChar1, StartChar2, StartChar3, PacketType, Address, ReadingData, Checksum1, Checksum0};

		Data       _currentData;
		Data       _checksumData;
		quint8		_currentAddress;
		quint8     _currentLength;
		quint8     _currentByteRead;
		quint8     _readChecksum;
		MessageStates _currentState;

	public slots:
		void read();
	};
}

#endif // PROTOCOLUM6_H
