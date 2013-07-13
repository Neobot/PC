#ifndef PROTOCOLRAW_H
#define PROTOCOLRAW_H

#include "AbstractProtocol.h"

namespace Comm
{
	/**
	 * \brief Raw communication protocol (no parsing).
	 */

	class ProtocolRaw : public AbstractProtocol
	{
		Q_OBJECT

	public:
		ProtocolRaw(QIODevice* device, Tools::AbstractLogger* logger = 0, bool bufferMode = true);

	/**
	  * \brief Send a message on the device.
	  */
		void sendMessage(quint8 address, quint8 instruction, const Data &data) const;

	public slots:
		void read();
	};
}

#endif // PROTOCOLRAW_H
