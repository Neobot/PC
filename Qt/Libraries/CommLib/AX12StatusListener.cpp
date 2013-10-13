#include "AX12StatusListener.h"
#include "AX12CommManager.h"

using namespace Comm;

AX12StatusListener::AX12StatusListener(AX12CommManager* manager, QObject* parent) 
	: QObject(parent), _manager(manager)
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

void AX12StatusListener::registerUnavailableId(quint8 id)
{
	if (_idsToListen.contains(id))
	{
		_currentUnavailableIds.insert(id);
		registerId(id);
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

void AX12StatusListener::startListening()
{
	connect(_manager, SIGNAL(servosStatusUpdated(quint8)), this, SLOT(registerId(quint8)));
	connect(_manager, SIGNAL(requestTimeoutReceived(quint8)), this, SLOT(registerUnavailableId(quint8)));
}

void AX12StatusListener::stopListening()
{
	disconnect(_manager, SIGNAL(servosStatusUpdated(quint8)), this, SLOT(registerId(quint8)));
	disconnect(_manager, SIGNAL(requestTimeoutReceived(quint8)), this, SLOT(registerUnavailableId(quint8)));
}
