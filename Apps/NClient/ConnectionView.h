#ifndef CONNECTIONVIEW_H
#define CONNECTIONVIEW_H

#include "AbstractView.h"

#include <QElapsedTimer>

namespace Ui {
	class ConnectionView;
}

class ConnectionView : public AbstractView, public NetworkClientCommListener, public Comm::CommListener
{
	Q_OBJECT
	
public:
	explicit ConnectionView(NetworkConnection *connection, QWidget *parent = 0);
	~ConnectionView();

	void loadSettings(QSettings *settings);
	void saveSettings(QSettings *settings);
	
public slots:
	void connectionStatusChanged(NetworkConnection::ConnectionStatus status);

private:
	Ui::ConnectionView *ui;
	QTimer* _serverPingTimer;
	QElapsedTimer _serverPingTimeCounter;
	QTimer* _robotPingTimer;
	QElapsedTimer _robotPingTimeCounter;
	bool _simulation;

	QString _preferedRobotPort;
	QString _preferedAx12Port;

	void serverAnnouncement(const QByteArray& message);
	void networkNoticeOfReceipt(quint8 instruction, bool result);
	void noticeOfReceipt(quint8 instruction, bool result);
	void setSerialPorts(const QStringList& ports);

private slots:
	void sendPingToServer();
	void pingServerTimeOut();
	void connectToServer();
	void disconnectFromServer();
	
	void connectToRobot();
	void disconnectFromRobot();
	void sendPingToRobot();
	void pingRobotTimeOut();

	void refreshSerialPorts();

	void connectionError(const QString& message);

};

#endif // CONNECTIONVIEW_H
