#include "AbstractView.h"

#include <QPainter>
#include <QPaintEvent>

AbstractView::AbstractView(NetworkConnection *connection, const QImage& backgroundImage, QWidget *parent) :
	QWidget(parent), _connection(connection), _backgroundImage(backgroundImage), _isActive(false)
{
	connect(_connection, SIGNAL(statusChanged(NetworkConnection::ConnectionStatus)),
			this, SLOT(connectionStatusChanged(NetworkConnection::ConnectionStatus)));

	//Add the alpha mask
	QPainter p(&_backgroundImage);
	p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
	p.fillRect(_backgroundImage.rect(), QColor(0, 0, 0, 50));

}

NetworkConnection *AbstractView::getConnection() const
{
	return _connection;
}

void AbstractView::loadSettings(QSettings *settings)
{
	Q_UNUSED(settings);
}

void AbstractView::saveSettings(QSettings *settings)
{
	Q_UNUSED(settings);
}

void AbstractView::refreshConnectionStatusGui()
{
	connectionStatusChanged(_connection->getConnectionStatus());
}

void AbstractView::setActiveView(bool value)
{
	if (_isActive != value)
	{
		_isActive = value;
		activeStatusChanged(_isActive);
	}
}

bool AbstractView::isViewActive() const
{
	return _isActive;
}

void AbstractView::paintEvent(QPaintEvent *event)
{
	if (!_backgroundImage.isNull())
	{
		QPainter p(this);

		QImage realImage =_backgroundImage.scaled(rect().size(), Qt::KeepAspectRatio);

		QRect dr = realImage.rect();
		dr.moveCenter(rect().center());

		p.drawImage(dr.topLeft(), realImage);
	}

	QWidget::paintEvent(event);
}
