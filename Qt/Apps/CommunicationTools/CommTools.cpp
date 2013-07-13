#include "CommTools.h"
#include "Data.h"
#include "CommUtil.h"


CommTools::CommTools(const QString& portname, const QString& baudrate, Tools::AbstractLogger* logger, QObject *parent) :
    QObject(parent), _mode(Listening), _out(0), _logger(logger), _quiet(false)
{
    _port = CommUtil::getQextSerialPortDevice(portname, baudrate);

    *_logger << "Opening "  << portname << " ..." << '\n';
    if (!_port->open(QIODevice::ReadWrite))
        *_logger << "An error occured while opening the port " << portname << " :" << '\n'
                 << _port->errorString();


    connect(_port, SIGNAL(readyRead()), this, SLOT(read()));
    *_logger << "Listening... " << '\n';

    _out = new QDataStream(_port);
}

CommTools::~CommTools()
{
    if (_port)
    {
        _port->close();
        delete _port;
        _port = 0;
    }

    if (_out)
    {
        delete _out;
        _out = 0;
    }
}



void CommTools::writeByte(quint8 byte)
{
    Q_ASSERT(_port->isOpen());
    *_out << byte;
}

void CommTools::writeBytes(QList<quint8> bytes)
{
    Q_ASSERT(_port->isOpen());
    for(QList<quint8>::const_iterator it = bytes.constBegin(); it != bytes.constEnd(); ++it)
        *_out << *it;
}

void CommTools::beQuiet(bool value)
{
    _quiet = value;
}

void CommTools::setStopValue(quint8 stopValue)
{
    _stopValue = stopValue;
}

void CommTools::startBaudrateTest(int n, QCoreApplication* app)
{
    _mode = BaudRateTest;
    while(1)
    {
        writeByte(n);
#ifdef Q_OS_WIN32
        Sleep(1);
#else
        sleep(1);
#endif
        app->processEvents();
    }
}

void CommTools::startSpeedTest(quint8 firstValue, int n, int group)
{
    _mode = SpeedTest;
    spTotalNumFrame = n;
    spCurentFrame = 0;
    spTimer.start();
    spValidFrames = 0;
    spInvalidFrames = 0;
    spGroup = group;
    spBytesRead = 0;

    spLastValueRead = firstValue;
    if (!_quiet) *_logger << "Write ";
    for(int i = 0; i < spGroup; ++i)
    {
        if (!_quiet)
            *_logger << firstValue << "  ";

        writeByte(firstValue++);
    }

    if (!_quiet)
        *_logger << '\n';
}

void CommTools::read()
{
    bool hasReadSomthing = false;
    quint8 byte;
    QDataStream in(_port);
    int nbByteToRead = _port->bytesAvailable();
    hasReadSomthing = nbByteToRead > 0;
    if (hasReadSomthing && !_quiet)
        *_logger << "Read ";
    while(nbByteToRead > 0)
    {
        in >> byte;

        if (!_quiet)
        {
            *_logger << (unsigned int)byte;
            if (_mode == SpeedTest)
                *_logger << "(" << spLastValueRead << ")";
            *_logger << " ";
        }

        ++spBytesRead;
        --nbByteToRead;
    }

    if (hasReadSomthing && !_quiet)
        *_logger << '\n';

    //Speed Test
    if (spBytesRead >= spGroup && _mode == SpeedTest)
    {
        spBytesRead = 0;

        if (byte == spLastValueRead)
            ++spValidFrames;
        else
            ++spInvalidFrames;

        ++spCurentFrame;
        if (spCurentFrame >= spTotalNumFrame)
            displaySpeedTestResult();
        else
        {
            spLastValueRead = ++byte;
            if (!_quiet) *_logger << "Write ";
            for(int i = 0; i < spGroup; ++i)
            {
                if (!_quiet) *_logger << byte << "  ";
                writeByte(byte++);
            }
            if (!_quiet) *_logger << '\n';
        }
    }
}

void CommTools::displaySpeedTestResult()
{
    _mode = Listening;
    *_logger << "Speed Test Result :" << '\n'
             << spTotalNumFrame << " frames in " << spTimer.elapsed() << "ms" << '\n'
             << spValidFrames << " valid frame(s) received" << '\n'
             << spInvalidFrames << " invalid frame(s) received" << '\n';
}
