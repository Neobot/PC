#include "ServerAX12RequestManager.h"
#include "NetworkCommInterface.h"
#include "AX12StatusListener.h"
#include "AX12CommManager.h"

ServerAX12RequestManager::ServerAX12RequestManager(Comm::AX12CommManager *ax12Manager, NetworkCommInterface *commInterface, int updateIntervalInMs, QObject *parent) :
    QObject(parent), _ax12Manager(0), _interface(commInterface), _recursive(false)
{
    _ax12StatusListener = new Comm::AX12StatusListener(ax12Manager, this);
    connect(_ax12StatusListener, SIGNAL(allServoUpdated()), this, SLOT(allServoUpdated()));

    setAX12CommManager(ax12Manager);
    _recursiveTimer = new QTimer;
    _recursiveTimer->setInterval(updateIntervalInMs);
    connect(_recursiveTimer, SIGNAL(timeout()), this, SLOT(sendPositions()));
}

ServerAX12RequestManager::~ServerAX12RequestManager()
{
    if (_recursive)
        _ax12Manager->cancelLoopStatusRequest(_requestedAX12Ids);
}

void ServerAX12RequestManager::setAX12CommManager(Comm::AX12CommManager *ax12Manager)
{
    _ax12StatusListener->setManager(ax12Manager);
    setRequest(QList<quint8>(), false); //cancel loop
    _ax12Manager = ax12Manager;
}

void ServerAX12RequestManager::setRequest(const QList<quint8> &ids, bool recursive)
{
    if (_ax12Manager)
    {
        if (_recursive)
            _ax12Manager->cancelLoopStatusRequest(_requestedAX12Ids);

        _requestedAX12Ids = ids;
        _ax12StatusListener->setIds(ids);
        _timeoutServo.clear();
        foreach(quint8 id, ids)
            _timeoutServo[id] = false;

        if (!ids.isEmpty())
        {
            _recursive = recursive;

            _ax12StatusListener->startListening();
            _ax12Manager->requestServoStatus(ids, recursive);

            if (recursive)
                _recursiveTimer->start();
        }
        else
        {
            if (_recursive)
                _recursiveTimer->stop();

            _timeoutServo.clear();
            _recursive = false;
        }
    }
}

void ServerAX12RequestManager::sendPositions()
{
    QList<float> positions;
	QList<float> loads;
    QList<quint8> sentIds;

    foreach(quint8 id, _requestedAX12Ids)
    {
        bool isTimeout = _ax12Manager->isServoTimeout(id);
        bool wasTimeout = _timeoutServo.value(id, true);

        if (!isTimeout || !wasTimeout)
        {
            if (isTimeout)
                _interface->sendAnnouncement(QByteArray("AX-12 status request timeout on id ").append(QByteArray::number(id)));

            _timeoutServo[id] = isTimeout;
            sentIds << id;
            positions << _ax12Manager->getServoPosition(id);
			loads << _ax12Manager->getServoLoad(id);
        }
    }

	_interface->sendAx12Positions(sentIds, positions, loads);
}

void ServerAX12RequestManager::allServoUpdated()
{
    if (!_recursive)
    {
        _ax12StatusListener->stopListening();
        sendPositions();
    }
}
