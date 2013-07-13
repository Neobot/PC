#ifndef ABSTRACTVIEW_H
#define ABSTRACTVIEW_H

#include <QWidget>
#include <QSettings>

#include "NetworkConnection.h"

class AbstractView : public QWidget
{
	Q_OBJECT
public:
	explicit AbstractView(NetworkConnection* connection, const QImage& backgroundImage, QWidget *parent = 0);
	virtual ~AbstractView() {}

	NetworkConnection* getConnection() const;

	virtual void loadSettings(QSettings* settings);
	virtual void saveSettings(QSettings* settings);

	void refreshConnectionStatusGui();

	void setActiveView(bool value);
	bool isViewActive() const;

public slots:
	virtual void connectionStatusChanged(NetworkConnection::ConnectionStatus status) {Q_UNUSED(status);}
	virtual void activeStatusChanged(bool isActive) {Q_UNUSED(isActive);}

protected:
	NetworkConnection* _connection;
	QImage _backgroundImage;
	bool _isActive;

	virtual void paintEvent(QPaintEvent *event);
	
};

#endif // ABSTRACTVIEW_H
