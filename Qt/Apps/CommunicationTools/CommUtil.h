#ifndef COMMUTIL_H
#define COMMUTIL_H

#include <QIODevice>
#include <qextserialport.h>


class CommUtil
{
public:
    static BaudRateType getQextSerialPortBaudrate(const QString& baudrate);
    static QIODevice* getQextSerialPortDevice(const QString& portname, BaudRateType baudrate);
    static QIODevice* getQextSerialPortDevice(const QString& portname, const QString& baudrate);
};

#endif // COMMUTIL_H
