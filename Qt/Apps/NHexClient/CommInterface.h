#ifndef COMMINTERFACE_H
#define COMMINTERFACE_H

#include <QObject>
#include <QIODevice>
#include <QSerialPort>

#include "CommDispatcher.h"
#include "RobotCommInterface.h"
#include "RobotProtocol.h"


class CommInterface : public QObject, public Comm::CommDispatcher
{
	Q_OBJECT

private:
	Comm::RobotCommInterface* _comm;
	Comm::RobotProtocol* _protocol;
	QIODevice* _port;
	bool _connected;

private slots:
	void handleSerialError(QSerialPort::SerialPortError error);

public:
	CommInterface();
	~CommInterface();
	bool openPort(const QString& portname, const QString& baudrate = "115200");
	bool closePort();
	Comm::RobotCommInterface* getComm() const;
	void changeStatus(bool connected);

signals:
	void statusChanged(bool connected);

};

#endif // COMMINTERFACE_H
