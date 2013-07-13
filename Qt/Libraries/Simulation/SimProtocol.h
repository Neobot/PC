#ifndef SIMPROTOCOL_H
#define SIMPROTOCOL_H

#include "AbstractProtocol.h"

class SimProtocol : public Comm::AbstractProtocol
{
    Q_OBJECT

public:
    SimProtocol();
    ~SimProtocol();

    void sendMessage(quint8 address, quint8 instruction, const Comm::Data& data) const;
    void sendMessage2(quint8 instruction, const Comm::Data& data);

signals:
    void message2(quint8 instruction, const Comm::Data& data) const;



};

#endif // SIMPROTOCOL_H
