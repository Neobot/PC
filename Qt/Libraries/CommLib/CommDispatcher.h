#ifndef COMMDISPATCHER_H
#define COMMDISPATCHER_H

#include "CommListener.h"

namespace Comm
{
	class CommDispatcher : public CommListener
	{
	public:
		CommDispatcher();
		virtual ~CommDispatcher() {}

		void setMainListener(CommListener* listener);
		void registerResponder(CommListener* listener);
		void unregisterResponder(CommListener* listener);

	protected:
		virtual void coordinates(qint16 x, qint16 y, double theta, quint8 forward);
		virtual void objective(qint16 x, qint16 y, double theta);
		virtual void activatedSensors(const QList<quint8>& values);
		virtual bool go(bool mirrored);
		virtual void initReceived();
		virtual bool pingReceived();
		virtual void noticeOfReceipt(quint8 instruction, bool result);
		virtual void opponentPosition(qint16 x, qint16 y);
		virtual void log(const QByteArray& text);
		virtual void parameters(const QList<float> &values);
		virtual void parameterNames(const QStringList& names);
		virtual void robotEvent(RobotEvent event);
		virtual void sensorEvent(SensorType type, int sensorId, int value);

	private:
		CommListener* _main;
		QList<CommListener*> _responders;
	};
}

#endif // COMMDISPATCHER_H
