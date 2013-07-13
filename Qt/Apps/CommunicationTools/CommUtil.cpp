#include "CommUtil.h"

BaudRateType CommUtil::getQextSerialPortBaudrate(const QString& baudrate)
{
    int value = baudrate.toInt();
    switch(value)
    {
        case 300:
            return BAUD300;
        case 600:
            return BAUD600;
        case 1200:
            return BAUD1200;
        case 2400:
            return BAUD2400;
        case 4800:
            return BAUD4800;
        default:
        case 9600:
            return BAUD9600;
        case 19200:
            return BAUD19200;
        case 38400:
            return BAUD38400;
        case 57600:
            return BAUD57600;
        case 76800:
            return BAUD76800;
        case 115200:
            return BAUD115200;
        case 128000:
            return BAUD128000;
        case 256000:
            return BAUD256000;
        case 230400:
            return BAUD230400;
        case 500000:
            return BAUD500000;
        case 576000:
            return BAUD576000;
        case 921600:
            return BAUD921600;
        case 1000000:
            return BAUD1000000;
        case 1152000:
            return BAUD1152000;
        case 1500000:
            return BAUD1500000;
        case 2000000:
            return BAUD2000000;
        case 2500000:
            return BAUD2500000;
        case 3000000:
            return BAUD3000000;
        case 3500000:
            return BAUD3500000;
        case 4000000:
            return BAUD4000000;
    }
}

QIODevice * CommUtil::getQextSerialPortDevice(const QString& portname, BaudRateType baudrate)
{
    QextSerialPort* port = new QextSerialPort(portname,  QextSerialPort::EventDriven);
    port->setBaudRate(baudrate);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);

    return port;
}

QIODevice * CommUtil::getQextSerialPortDevice(const QString& portname, const QString& baudrate)
{
    return getQextSerialPortDevice(portname, getQextSerialPortBaudrate(baudrate));
}
