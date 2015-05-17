#ifndef ABSTRACTVIEW_H
#define ABSTRACTVIEW_H

#include <QWidget>
#include <QSettings>

#include "CommInterface.h"

class AbstractView : public QWidget
{
	Q_OBJECT
public:
	explicit AbstractView(CommInterface* connection, const QImage& backgroundImage, QWidget *parent = 0);
	virtual ~AbstractView() {}

	CommInterface* getConnection() const;

	virtual void loadSettings(QSettings* settings);
	virtual void saveSettings(QSettings* settings);

	void setActiveView(bool value);
	bool isViewActive() const;

public slots:
	virtual void connectionStatusChanged(bool connected) {Q_UNUSED(connected);}
	virtual void activeStatusChanged(bool isActive) {Q_UNUSED(isActive);}

protected:
	CommInterface* _connection;
	QImage _backgroundImage;
	bool _isActive;

	virtual void paintEvent(QPaintEvent *event);
	
};

#endif // ABSTRACTVIEW_H
