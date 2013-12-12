#include "Ax12MovementRunner.h"
#include "AX12StatusListener.h"

using namespace Comm;
using namespace Tools;

Ax12MovementRunner::Ax12MovementRunner(Comm::AX12CommManager *comm, Tools::Ax12MovementManager *manager, QObject* parent)
	: QObject(parent), _isRunning(false), _comm(comm), _manager(manager)
{
	_listener = new AX12StatusListener(_comm, this);
	connect(_listener, SIGNAL(allServoUpdated()), this, SLOT(checkStatus()));
	
	_stopAngleCurve << Point(0, 3) << Point(10, 3) << Point(50, 5) << Point(114, 10);

	_timer = new QTimer(this);
	_timer->setInterval(1000);
	_timer->setSingleShot(true);
	connect(_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

bool Ax12MovementRunner::startMovement(const QString &group, const QString &mvt, float speedLimit, int lastPositionIndex)
{
	if (_isRunning || !_comm->isOpened() || !_comm->isReadingLoopStarted())
		return false;

	_positions = _manager->getDetailedMovement(group, mvt);
	_ids.clear();
	foreach(int id, _manager->getGroupIds(group))
		_ids << id;
	_currentGroup = group;
	_currentMvt = mvt;
	_speedLimit = speedLimit;
    _lastPositionIndex = lastPositionIndex;
    _currentPositionIndex = -1;

    _comm->requestServoStatus(_ids, true);

	if (!_positions.isEmpty())
	{
		goToNextPosition();
		_isRunning = true;
		
		_listener->setIds(_ids);
		_listener->startListening();
		
		emit movementStarted(_currentGroup, _currentMvt);
		
		return true;
	}
	
	return false;
}

void Ax12MovementRunner::stop()
{
	foreach(quint8 id, _ids)
	{
		_comm->lockServo(id);
		_comm->synchronize(id);
	}

	movementEnd(false);
}

bool Ax12MovementRunner::isRunnning() const
{
	return _isRunning;
}

void Ax12MovementRunner::goToNextPosition()
{
	if (!_positions.isEmpty())
	{
		_timer->stop();
		Ax12MovementManager::DetailedPosition pos = _positions.takeFirst();

		float speed = qMin(pos.second.maxSpeed, _speedLimit);
		float torque = pos.second.torque;
		_stopAngle = _stopAngleCurve.yValue(speed);
		Q_ASSERT(_stopAngle > 0);

		_asservedIds.clear();
		for(QMap<int, Ax12MovementManager::Ax12Position>::const_iterator it = pos.first.constBegin(); it != pos.first.constEnd(); ++it)
		{
			if (*it >= 0)
			{
				_asservedIds << it.key();
				_comm->setGoal(it.key(), *it, speed, torque, false);
			}
		}

		_comm->calculateSmoothServosSpeed(_ids, speed);
		_comm->synchronize(_ids);
		_timer->start();
	}
	else
	{
		//Movement finished
		movementEnd(true);
	}
}

void Ax12MovementRunner::timeout()
{
	movementEnd(false);
}

void Ax12MovementRunner::movementEnd(bool status)
{
	_listener->stopListening();
    _comm->cancelLoopStatusRequest(_ids);
	_isRunning = false;
	_asservedIds.clear();
	_timer->stop();
	emit movementFinished(status, _currentGroup, _currentMvt);
}

void Ax12MovementRunner::checkStatus()
{
	bool done = true;
	bool error = false;
	if (!_listener->hasUnavailableServo())
	{
		foreach(quint8 id, _asservedIds)
		{
			if (_comm->contains(id))
			{
				AX12 ax12 = _comm->getAX12(id);
				done = done && ax12.getAngleDifference() < _stopAngle;

				error = error || (ax12.getSpeed() == 0 && !done);
			}
		}	
		
		if (done)
        {
            ++_currentPositionIndex;
            if (_lastPositionIndex >= 0 && _lastPositionIndex >= _currentPositionIndex)
                movementEnd(true);
            else
                goToNextPosition();
        }
	}
	else
		movementEnd(false);
}
