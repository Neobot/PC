#include "AbstractCommInterface.h"
#include "Instructions.h"
#include <QTimer>

using namespace Comm;
using namespace Tools;

AbstractCommInterface::AbstractCommInterface(AbstractLogger* logger, QObject* parent)
    : QObject(parent), LoggerInterface(logger), _timeOutAR(300), _arChecking(true)
{
}

AbstractCommInterface::AbstractCommInterface(AbstractProtocol* protocol, AbstractLogger* logger, QObject* parent)
    : QObject(parent), LoggerInterface(logger), _timeOutAR(300), _arChecking(true)
{
    addProtocol(protocol);
}

AbstractCommInterface::AbstractCommInterface(QList<AbstractProtocol*> protocolList, AbstractLogger* logger, QObject* parent)
    : QObject(parent), LoggerInterface(logger), _timeOutAR(300), _arChecking(true)
{
    _protocols << protocolList;
    foreach(AbstractProtocol* protocol, _protocols)
    {
		connectProtocol(protocol);
    }

}

void AbstractCommInterface::addProtocol(AbstractProtocol* protocol)
{
    _protocols << protocol;
    connectProtocol(protocol);
}

AbstractProtocol* AbstractCommInterface::getProtocol(int i) const
{
    return _protocols.value(i, 0);
}

void AbstractCommInterface::connectProtocol(AbstractProtocol* protocol)
{
    if (!connect(protocol, SIGNAL(message(quint8, const Comm::Data&)), this, SLOT(read(quint8, const Comm::Data&))))
	LoggerInterface::logger() << "The connection to the protocol failed." << Tools::endl;
}

void AbstractCommInterface::setNoticeOfReceiptTimeOut(int time)
{
    _timeOutAR = time;
}

void AbstractCommInterface::checkNoticeOfReceipt(quint8 instruction, const Data& data, AbstractProtocol* protocol, int n)
{
    if (_arChecking)
    {
        _pendingAR.enqueue(ARData(instruction, data, protocol, n));
        QTimer::singleShot(_timeOutAR, this, SLOT(timeOut()));
    }
}

void AbstractCommInterface::noticeOfReceiptReceived(quint8 instruction)
{
    if (_arChecking)
	_receivedAR.insert(instruction);
}

void AbstractCommInterface::timeOut()
{
    ARData data = _pendingAR.dequeue();
    if (_receivedAR.contains(data._instruction))
    {
        _receivedAR.remove(data._instruction);
    }
    else
    {
        data._protocol->sendMessage(data._instruction, data._data);
        if (data._nb > 1)
            checkNoticeOfReceipt(data._instruction, data._data, data._protocol, --data._nb);
    }
}

void AbstractCommInterface::disableNoticeOfReceiptChecking()
{
    _arChecking = false;
}
