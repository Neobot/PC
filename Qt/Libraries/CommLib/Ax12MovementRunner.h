#ifndef AX12MOVEMENTRUNNER_H
#define AX12MOVEMENTRUNNER_H

#include <QObject>
#include "Ax12MovementManager.h"
#include "AX12CommManager.h"
#include "Curve.h"

namespace Comm
{
	class AX12StatusListener;
	
	class Ax12MovementRunner : public QObject
	{
		Q_OBJECT
		
	signals:
		void movementStarted(const QString& group, const QString& mvt);
		void movementFinished(bool status, const QString& group, const QString& mvt);

	public:
		Ax12MovementRunner(AX12CommManager* comm, Tools::Ax12MovementManager* manager, QObject* parent = 0);

		bool startMovement(const QString& group, const QString& mvt, float speedLimit = 110.0f);
		void stop();

		bool isRunnning() const;

	private:
		bool _isRunning;
		AX12CommManager* _comm;
		Tools::Ax12MovementManager* _manager;
		AX12StatusListener* _listener;
		QTimer* _timer;

		QList<Tools::Ax12MovementManager::DetailedPosition> _positions;
		QList<quint8> _ids;
		QList<quint8> _asservedIds;
		float _speedLimit;
		QString _currentGroup;
		QString _currentMvt;
		
		float _stopAngle;
		Tools::Curve _stopAngleCurve;
		
		void movementEnd(bool status);

	private slots:
		void checkStatus();
		void goToNextPosition();
		void timeout();
	};
}

#endif // AX12MOVEMENTRUNNER_H
