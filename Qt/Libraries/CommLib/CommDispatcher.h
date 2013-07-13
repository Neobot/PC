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
		virtual void avoidingSensors(const QList<quint8>& values);
		virtual void microswitchs(const QList<quint8>& values);
		virtual void otherSensors(const QList<quint8>& values);
		virtual bool initDone();
		virtual bool go(bool mirrored);
		virtual bool pingReceived();
		virtual void noticeOfReceipt(quint8 instruction, bool result);
		virtual void opponentPosition(qint16 x, qint16 y);
		virtual bool restart();
		virtual void quit();
		virtual void isArrived();
		virtual void isBlocked();
		virtual void log(const QByteArray& text);
		virtual void parameters(const QList<float> &values);
		virtual void parameterNames(const QStringList& names);

	private:
		CommListener* _main;
		QList<CommListener*> _responders;
	};
}

#endif // COMMDISPATCHER_H
