#ifndef CONNECTIONVIEW_H
#define CONNECTIONVIEW_H

#include <QElapsedTimer>

#include "AbstractView.h"

namespace Ui {
	class ConnectionView;
}

class ConnectionView : public AbstractView, public Comm::CommListener
{
	Q_OBJECT
	
public:
	explicit ConnectionView(CommInterface *connection, QWidget *parent = 0);
	~ConnectionView();

	void loadSettings(QSettings *settings);
	void saveSettings(QSettings *settings);

public slots:
	void connectionStatusChanged(bool connected);

private:
	Ui::ConnectionView *ui;
	QTimer* _robotPingTimer;
	QElapsedTimer _robotPingTimeCounter;

	QString _preferedRobotPort;

	void noticeOfReceipt(quint8 instruction, bool result);
	void setSerialPorts(const QStringList& ports);
	void refreshSerialPorts();

private slots:
	void sendPingToRobot();
	void pingRobotTimeOut();
	void openPort();
	void closePort();
};

#endif // CONNECTIONVIEW_H
