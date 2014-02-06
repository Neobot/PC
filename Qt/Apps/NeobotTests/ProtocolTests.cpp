#include "ProtocolTests.h"

#include <QtGlobal>
#include <QTest>
#include <QtDebug>

using namespace Comm;

//#define SHOW_INFO
const int waitingTime = 10; //ms

ProtocolTests::ProtocolTests(QObject *parent) : QObject(parent)
{
}

void ProtocolTests::initTestCase()
{
    if (!_buffer.open(QIODevice::ReadWrite))
	QFAIL("Cannot open buffer");

    _logger = new Tools::QDebugLogger;
    _protocol = new RobotProtocol(&_buffer, _logger);

#ifndef SHOW_INFO
    _protocol->beQuiet(true);
#endif
}

void ProtocolTests::cleanupTestCase()
{
    delete _protocol;
    _protocol = 0;
}

void ProtocolTests::writingTest()
{
    Data d;
    d.add((quint8)'H').add((quint8)'e').add((quint8)'l').add((quint8)'l').add((quint8)'o');
    d.add((quint8)' ').add((quint8)'w').add((quint8)'o').add((quint8)'r').add((quint8)'l').add((quint8)'d');

    _protocol->sendMessage(0, 42, d);

#ifdef SHOW_INFO
    for(int i = 0; i < 16; ++i)
        qDebug() << (quint8)_buffer.data().at(i);
#endif

    QCOMPARE(_buffer.data().length(), d.size() + 5);

    QCOMPARE(_buffer.data().at(0), (char)0xFF);
    QCOMPARE(_buffer.data().at(1), (char)0xFF);
    QCOMPARE(_buffer.data().at(2), (char)(d.size() + 1));
    QCOMPARE(_buffer.data().at(3), (char)42);
    QCOMPARE(_buffer.data().at(4), 'H');
    QCOMPARE(_buffer.data().at(5), 'e');
    QCOMPARE(_buffer.data().at(6), 'l');
    QCOMPARE(_buffer.data().at(7), 'l');
    QCOMPARE(_buffer.data().at(8), 'o');
    QCOMPARE(_buffer.data().at(9), ' ');
    QCOMPARE(_buffer.data().at(10), 'w');
    QCOMPARE(_buffer.data().at(11), 'o');
    QCOMPARE(_buffer.data().at(12), 'r');
    QCOMPARE(_buffer.data().at(13), 'l');
    QCOMPARE(_buffer.data().at(14), 'd');
    QCOMPARE(_buffer.data().at(15), (char)141);

}

void ProtocolTests::readingTest()
{  
    readingTestBegin();
    QVERIFY(connect(&_buffer, SIGNAL(readyRead()), this, SLOT(messageReceived())));

    Data d1, d2;
    d1.add((quint8)'H').add((quint8)'e').add((quint8)'l').add((quint8)'l').add((quint8)'o');
    d2.add((quint8)'w').add((quint8)'o').add((quint8)'r').add((quint8)'l').add((quint8)'d');

    _protocol->sendMessage(0, 42, d1);
    _buffer.seek(0);
    _protocol->read();  //By-passing readyRead() signal.

    QCOMPARE(_buffer.data().length(), d1.size() + 5);
    QTest::qWait(waitingTime);
    QVERIFY(_buffer.bytesAvailable() == 0);
    QVERIFY(_messages.count() == 1);

    _protocol->sendMessage(0, 111, d2);
    _protocol->sendMessage(0, 22, d1);
    _buffer.seek(0);
    _protocol->read();
    QTest::qWait(waitingTime);

    QVERIFY(_buffer.bytesAvailable() == 0);
    QCOMPARE(_messages.count(), 4);

    readingTestEnd();
}

void ProtocolTests::bufferedRead()
{
    readingTestBegin();

    char t1[3] = {(char)0xFF,(char)0xFF, 3};
    char t2[4] = {42, 'H', 'i', 33};

    _buffer.write(t1, 3);
    _buffer.seek(0);
    _protocol->read();
    QTest::qWait(waitingTime);
    QCOMPARE(_messages.count(), 0);

    _buffer.buffer().clear();
    _buffer.seek(0);
    _buffer.write(t2, 4);
    _buffer.seek(0);
    _protocol->read();
    QTest::qWait(waitingTime);
    QCOMPARE(_messages.count(), 1);

    readingTestEnd();
}

void ProtocolTests::invalidChecksumTest()
{
    readingTestBegin();
    char inv1[7] = {(char)0xFF, (char)0xFF, 3, 33, 'A', 'H', 70};             //invalid checksum, correct checksum : 82
    char val1[7] = {(char)0xFF, (char)0xFF, 3, 42, 'H', 'i', 33};             //valid

    _buffer.write(inv1, 7);
    _buffer.write(val1, 7);
    QVERIFY(_buffer.seek(0));
    QCOMPARE(_buffer.bytesAvailable(), (qint64)14);
    _protocol->read();
    QTest::qWait(waitingTime);
    QCOMPARE(_messages.count(), 1);
    QCOMPARE(_messages.value(0), (quint8)42);

    readingTestEnd();
}

void ProtocolTests::invalidHeaderTest()
{
    readingTestBegin();
    char inv3[7] = {(char)0xFF, 0x3A, 3, 33, 'L', 'O', 64};             //invalid header
    char val1[7] = {(char)0xFF, (char)0xFF, 3, 42, 'H', 'i', 33};             //valid

    _buffer.write(inv3, 7);
    _buffer.write(val1, 7);

    QVERIFY(_buffer.seek(0));
    QCOMPARE(_buffer.bytesAvailable(), (qint64)14);
    _protocol->read();
    QTest::qWait(waitingTime);
    QCOMPARE(_messages.count(), 1);
    QCOMPARE(_messages.value(0), (quint8)42);

    readingTestEnd();
}

void ProtocolTests::invalidLengthTest()
{
    // The length is smaller than the frame -> one frame is skipped
    readingTestBegin();
    char inv2[9] = {(char)0xFF, (char)0xFF, 4, 33, 'L', 'A', 'S', 'A', (char)184};  //invalid lentgh
    char val1[7] = {(char)0xFF, (char)0xFF, 3, 42, 'H', 'i', 33};             //valid

    _buffer.write(inv2, 9);
    _buffer.write(val1, 7);

    QVERIFY(_buffer.seek(0));
    QCOMPARE(_buffer.bytesAvailable(), (qint64)16);
    _protocol->read();
    QTest::qWait(waitingTime);
    QCOMPARE(_messages.count(), 1);
    QCOMPARE(_messages.value(0), (quint8)42);

    readingTestEnd();
}

void ProtocolTests::invalidLengthTest2()
{
    // The length is grater than the frame -> two frames are skipped
    readingTestBegin();
    char inv2[9] = {(char)0xFF, (char)0xFF, 8, 33, 'L', 'A', 'S', 'A', (char)184};  //invalid lentgh & checksum
    char val1[7] = {(char)0xFF, (char)0xFF, 3, 42, 'H', 'i', 33};             //valid
    //note : char inv2[9] = {0xFF, 0xFF, 7, 33, 'L', 'A', 'S', 'A', 184};
    //it leads to a false frame being seen as valid because the cheksum is 255.

    _buffer.write(inv2, 9);
    _buffer.write(val1, 7);
    _buffer.write(val1, 7);

    QVERIFY(_buffer.seek(0));
    _protocol->read();
    QTest::qWait(waitingTime);
    QCOMPARE(_messages.count(), 1);
    QCOMPARE(_messages.value(0), (quint8)42);

    readingTestEnd();
}

void ProtocolTests::invalidLengthTest3()
{
    // The length is too big, it's probably an error -> one frame is skipped.
    readingTestBegin();
    char inv2[9] = {(char)0xFF, (char)0xFF, (char)242, 100, 'L', 'A', 'S', 'A', (char)184};  //invalid lentgh
    char val1[7] = {(char)0xFF, (char)0xFF, 3, 42, 'H', 'i', 33};             //valid

    _buffer.write(inv2, 9);
    _buffer.write(val1, 7);

    QVERIFY(_buffer.seek(0));
    _protocol->read();
    QTest::qWait(waitingTime);
    QCOMPARE(_messages.count(), 1);
    QCOMPARE(_messages.value(0), (quint8)42);

    readingTestEnd();
}

void ProtocolTests::messageReceived(quint8 instruction, const Data& data)
{
    Q_UNUSED(data);
    _messages << instruction;
#ifdef SHOW_INFO
    qDebug() << "Instruction Received : " << instruction;
#endif
}

void ProtocolTests::messageReceived()
{

}

void ProtocolTests::readingTestBegin()
{
    QVERIFY(connect(_protocol, SIGNAL(message(quint8, const Comm::Data&)), this, SLOT(messageReceived(quint8, const Comm::Data&))));

    QVERIFY(_buffer.isOpen());
    _buffer.buffer().clear();
    QVERIFY(_buffer.seek(0));
    _messages.clear();
}

void ProtocolTests::readingTestEnd()
{
    QVERIFY(disconnect(_protocol, SIGNAL(message(quint8, const Comm::Data&)), this, SLOT(messageReceived(quint8, const Comm::Data&))));
}
