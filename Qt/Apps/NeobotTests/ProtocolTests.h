#ifndef PROTOCOLTEST_H
#define PROTOCOLTEST_H

#include <QObject>
#include <QBuffer>
#include "RobotProtocol.h"
#include "Data.h"
#include "AbstractLogger.h"

class ProtocolTests : public QObject
{
    Q_OBJECT
public:
    ProtocolTests(QObject *parent = 0);

private:
    Comm::RobotProtocol* _protocol;
    QBuffer _buffer;

    QList<quint8> _messages;
    Tools::QDebugLogger* _logger;

    void readingTestBegin();
    void readingTestEnd();

public slots:
    void messageReceived(quint8 instruction, const Comm::Data& data);
    void messageReceived();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void writingTest();
    void readingTest();
    void bufferedRead();
    void invalidChecksumTest();
    void invalidHeaderTest();
    void invalidLengthTest();
    void invalidLengthTest2();
    void invalidLengthTest3();


};

#endif // PROTOCOLTEST_H
