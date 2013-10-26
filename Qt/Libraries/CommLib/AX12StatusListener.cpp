#include "AX12StatusListener.h"
#include "AX12CommManager.h"

using namespace Comm;

AX12StatusListener::AX12StatusListener(AX12CommManager* manager, QObject* parent) 
    : QObject(parent), _manager(manager), _isListenning(false)
{
}

void AX12StatusListener::setIds(const QList<quint8>& ids)
{
	_idsToListen = ids.toSet();
	_updatedIds.clear();
	_currentUnavailableIds.clear();
	_unavailableIds.clear();
}

void  AX12StatusListener::sendAllServoUpdatedMessage()
{
	_updatedIds.clear();
	_currentUnavailableIds.clear();
	_unavailableIds = _currentUnavailableIds.toList();
    emit allServoUpdated();
}

void AX12StatusListener::registerId(quint8 id)
{
    if (_idsToListen.contains(id))
    {
        if (_updatedIds.contains(id))
            sendAllServoUpdatedMessage();

        _updatedIds.insert(id);

        if (_updatedIds == _idsToListen)
            sendAllServoUpdatedMessage();
    }
}

void AX12StatusListener::registerId(const QList<quint8> &ids)
{
    foreach(quint8 id, ids)
    {
        registerId(id);
    }
}

void AX12StatusListener::registerUnavailableId(const QList<quint8>& ids)
{
    foreach(quint8 id, ids)
    {
        if (_idsToListen.contains(id))
        {
            _currentUnavailableIds.insert(id);
            registerId(id);
        }
    }
}

bool AX12StatusListener::hasUnavailableServo() const
{
	return !_unavailableIds.isEmpty();
}

const QList<quint8>& AX12StatusListener::getUnavailableServo() const
{
    return _unavailableIds;
}

bool AX12StatusListener::isListening() const
{
    return _isListenning;
}

void AX12StatusListener::setManager(AX12CommManager *manager)
{
    if (isListening())
        stopListening();

    _manager = manager;
}

void AX12StatusListener::startListening()
{
    if (!_manager)
        return;

    connect(_manager, SIGNAL(servosStatusUpdated(QList<quint8>)), this, SLOT(registerId(QList<quint8>)));
    connect(_manager, SIGNAL(requestTimeoutReceived(QList<quint8>)), this, SLOT(registerUnavailableId(QList<quint8>)));
    _isListenning = true;
}

void AX12StatusListener::stopListening()
{
    if (!_manager)
        return;

    disconnect(_manager, SIGNAL(servosStatusUpdated(QList<quint8>)), this, SLOT(registerId(QList<quint8>)));
    disconnect(_manager, SIGNAL(requestTimeoutReceived(QList<quint8>)), this, SLOT(registerUnavailableId(QList<quint8>)));
    _isListenning = false;
}
