#ifndef SIGNALDELAYER_H
#define SIGNALDELAYER_H

#include <QObject>
#include <QTimer>

namespace Tools
{
	class SignalDelayer : public QObject
	{
		Q_OBJECT

	signals:
		void raised();

	public:
		SignalDelayer(QObject *parent);

		void setDelayedSignal(const QObject* object, const char * signal, int ms);

	private:
		QTimer* _timer;

	private slots:
		void received();

	};
}

#endif // SIGNALDELAYER_H
