#include "ProtocolRaw.h"
#include "CommException.h"
#include <QtDebug>

using namespace Comm;
using namespace Tools;

ProtocolRaw::ProtocolRaw(QIODevice* device, AbstractLogger* logger, bool bufferMode)
	: AbstractProtocol(device, logger)
{
    if (bufferMode)
        run();
}

void ProtocolRaw::sendMessage(quint8 address, quint8 instruction, const Data& data) const
{
    Q_UNUSED(address);
    Q_UNUSED(instruction);

    if (!_device->isOpen())
    {
        if (_showError)
            logger() << "Protocol: device not opened." << Tools::endl;
        return;
    }

    // Write the data to the device
    if(_device->write(data) == -1 && _showError)
        logger() << "Protocol: writing failed." << Tools::endl;

    if (_messages)
        logger() << "Sent: " << data << Tools::endl;
}


void ProtocolRaw::read()
{
    // Buffer mode
    QDataStream in(_device);
    in.setByteOrder(QDataStream::BigEndian);

    Data data;

    while (_device->bytesAvailable())
    {
        quint8 value;
        in >> value;
        data.add(value);
    }

    if (_messages)
        logger() << "Received: " << data << Tools::endl;

    emit message(0, data, 0);

    if (_frames)
        logger() << Tools::endl;
}
