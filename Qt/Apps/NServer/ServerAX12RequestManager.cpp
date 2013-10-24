#include "ServerAX12RequestManager.h"
#include "NetworkCommInterface.h"
#include "AX12StatusListener.h"
#include "AX12CommManager.h"

ServerAX12RequestManager::ServerAX12RequestManager(Comm::AX12CommManager *ax12Manager, NetworkCommInterface *commInterface, QObject *parent) :
    QObject(parent), _ax12Manager(0), _interface(commInterface), _recursive(false)
{
    _ax12StatusListener = new Comm::AX12StatusListener(ax12Manager, this);
    connect(_ax12StatusListener, SIGNAL(allServoUpdated()), this, SLOT(allServoUpdated()));

    setAX12CommManager(ax12Manager);
}

ServerAX12RequestManager::~ServerAX12RequestManager()
{
    if (_recursive)
        _ax12Manager->cancelLoopStatusRequest(_requestedAX12Ids);
}

void ServerAX12RequestManager::setAX12CommManager(Comm::AX12CommManager *ax12Manager)
{
    Q_ASSERT(!_ax12StatusListener->isListening());

    _ax12Manager = ax12Manager;
    if (_ax12Manager)
        connect(_ax12Manager, SIGNAL(requestTimeoutReceived(QList<quint8>)), this, SLOT(servoStatusTimeout(QList<quint8>)));
}

void ServerAX12RequestManager::setRequest(const QList<quint8> &ids, bool recursive)
{
    if (_ax12Manager)
    {
        if (_recursive)
            _ax12Manager->cancelLoopStatusRequest(_requestedAX12Ids);

        _requestedAX12Ids = ids;
        _recursive = recursive;
        _ax12StatusListener->setIds(ids);

        _ax12StatusListener->startListening();
        _ax12Manager->requestServoStatus(ids, recursive);
    }
}

void ServerAX12RequestManager::allServoUpdated()
{
    if (!_recursive)
        _ax12StatusListener->stopListening();

    QList<float> positions;
    foreach(quint8 id, _requestedAX12Ids)
        positions << _ax12Manager->getServoPosition(id);

    _interface->sendAx12Positions(_requestedAX12Ids, positions);
}

void ServerAX12RequestManager::servoStatusTimeout(const QList<quint8> &ids)
{
    if (_ax12Manager && _ax12StatusListener->isListening())
    {
        foreach(quint8 id, ids)
        {
            if (_requestedAX12Ids.contains(id))
                _interface->sendAnnouncement(QByteArray("AX-12 status request timeout on id ").append(QByteArray::number(id)));
        }
    }
}
