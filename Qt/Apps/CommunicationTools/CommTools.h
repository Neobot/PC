#ifndef COMMTOOLS_H
#define COMMTOOLS_H

#include <QObject>
#include <QIODevice>
#include <QCoreApplication>
#include <QTime>
#include <QList>
#include <QDataStream>
#include <QSerialPort>

#include "Data.h"
#include "AbstractLogger.h"


class CommTools : public QObject
{
Q_OBJECT

public:
    enum Mode {Listening, BaudRateTest, SpeedTest};
    CommTools(const QString& portname, const QString& baudrate, Tools::AbstractLogger* logger, QObject *parent = 0);
    ~CommTools();

    void startBaudrateTest(int n, QCoreApplication* app);
    void startSpeedTest(quint8 firstValue, int n, int group);

    void setStopValue(quint8 stopValue);
    void beQuiet(bool value);



private:
    Mode _mode;
	QSerialPort* _port;
    QDataStream* _out;
    Tools::AbstractLogger* _logger; //For info messages
    bool _quiet;
    quint8 _stopValue;

    // Speed Test Data;
    int spTotalNumFrame;
    int spCurentFrame;
    QTime spTimer;
    int spValidFrames;
    int spInvalidFrames;
    quint8 spLastValueRead;
    int spGroup;
    int spBytesRead;

    void writeByte(quint8 byte);
    void writeBytes(QList<quint8> bytes);
    void displaySpeedTestResult();

public slots:
    void read();

private slots:
	void handleSerialError(QSerialPort::SerialPortError error);
};

#endif // COMMTOOLS_H
