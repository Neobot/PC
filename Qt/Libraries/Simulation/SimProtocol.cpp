#include "SimProtocol.h"

#include <QBuffer>

SimProtocol::SimProtocol() : AbstractProtocol(new QBuffer())
{
}

SimProtocol::~SimProtocol()
{
	delete _device;
}

void SimProtocol::sendMessage2(quint8 instruction, const Comm::Data& data)
{
    emit message(instruction, data);
}

void SimProtocol::sendMessage(quint8 address, quint8 instruction, const Comm::Data &data) const
{
    Q_UNUSED(address);
    emit message2(instruction, data);
}
