#include "SignalDelayer.h"

using namespace Tools;

SignalDelayer::SignalDelayer(QObject *parent)
	: QObject(parent)
{
	_timer = new QTimer(this);
	_timer->setSingleShot(true);
	_timer->setInterval(500);

	connect(_timer, SIGNAL(timeout()), this, SIGNAL(raised()));
}

void SignalDelayer::setDelayedSignal(const QObject* object, const char * signal, int ms)
{
	disconnect(0, 0, this, SLOT(received()));
	connect(object, signal, this, SLOT(received()));
	_timer->setInterval(ms);
}

void SignalDelayer::received()
{
	_timer->stop();
	_timer->start();
}
