#include "DataTests.h"
#include "Data.h"
#include <QtGlobal>
#include <QTest>
#include <QtDebug>

using namespace Comm;

DataTests::DataTests(QObject *parent) :
    QObject(parent)
{
}

void DataTests::LittleEndianTest()
{
    Data empty;
    QCOMPARE(empty.length(), 0);

    Data data(Comm::LittleEndian);

    quint8 a = 42, b = 24, c = 36, d = 18, e = 250;
    data.add(a).add(b).add(c).add(d).add(e);

    QCOMPARE(data.size(), 5);

    quint16 r1, r2, r3;
    quint32 r4;
    QVERIFY(!data.readAt(-1, r1));
    QVERIFY(!data.readAt(42, r1));
    QVERIFY(data.readAt(0, r1));
    QVERIFY(data.readAt(1, r2));
    QVERIFY(data.readAt(2, r3));
    QVERIFY(!data.readAt(2, r4));
    QVERIFY(data.readAt(0, r4));

    QCOMPARE(r1, (quint16)6186);
    QCOMPARE(r2, (quint16)9240);
    QCOMPARE(r3, (quint16)4644);
    QCOMPARE(r4, (quint32)304355370);
    QCOMPARE(data.checksum(), (quint8)141); // 255-114

    QCOMPARE(data.size(), 5);

    quint8 r5, r6, r7, r8;
    qint8 r9;

    data.take(r5);
    QCOMPARE(data.size(), 4);
    data.take(r6);
    QCOMPARE(data.size(), 3);
    data.take(r7);
    QCOMPARE(data.size(), 2);
    data.take(r8);
    QCOMPARE(data.size(), 1);
    data.take(r9);
    QCOMPARE(data.size(), 0);

    QCOMPARE(r5, (quint8)42);
    QCOMPARE(r6, (quint8)24);
    QCOMPARE(r7, (quint8)36);
    QCOMPARE(r8, (quint8)18);
    QCOMPARE(r9, (qint8)-6);
}

void DataTests::BigEndianTest()
{
    Data data(Comm::BigEndian);

    quint8 a = 42, b = 24, c = 36, d = 18, e = 250;
    data.add(a).add(b).add(c).add(d).add(e);

    QCOMPARE(data.size(), 5);

    quint16 r1, r2, r3;
    quint32 r4;
    QVERIFY(data.readAt(0, r1));
    QVERIFY(data.readAt(1, r2));
    QVERIFY(data.readAt(2, r3));
    QVERIFY(data.readAt(0, r4));

    QCOMPARE(r1, (quint16)10776);
    QCOMPARE(r2, (quint16)6180);
    QCOMPARE(r3, (quint16)9234);
    QCOMPARE(r4, (quint32)706225170);
}

void DataTests::SimpleBooleanTest()
{
	Data data(Comm::LittleEndian);
    data.add(true, true, false, false, true, false, false, true);
    QCOMPARE(data.size(), 1);

    quint8 value;
    QVERIFY(data.readAt(0, value));
	qDebug() << value;
    QCOMPARE(value, (quint8)147);

    bool b1, b2, b3, b4, b5, b6, b7, b8;

    data.take(b1, b2, b3, b4, b5, b6, b7, b8);
    QCOMPARE(data.size(), 0);
    QCOMPARE(b1, true);
    QCOMPARE(b2, true);
    QCOMPARE(b3, false);
    QCOMPARE(b4, false);
    QCOMPARE(b5, true);
    QCOMPARE(b6, false);
    QCOMPARE(b7, false);
    QCOMPARE(b8, true);
}

void DataTests::ComplexBooleanTest()
{
	Data data(Comm::LittleEndian);

    data.add(false, true);
    QCOMPARE(data.size(), 1);

    QBitArray bits(12, false);
    bits[3] = true;
    bits[4] = true;
    bits[10] = true;
    bits[11] = true;
    data.add(bits);

    QCOMPARE(data.size(), 3);

    quint8 r1, r2, r3;
    QVERIFY(data.readAt(0, r1));
    QVERIFY(data.readAt(1, r2));
    QVERIFY(data.readAt(2, r3));

    QCOMPARE(r1, (quint8)2);
    QCOMPARE(r2, (quint8)24);
    QCOMPARE(r3, (quint8)12);

    bool initValue = true;
    QBitArray result(12, initValue);
    data.take(result);
    QCOMPARE(data.size(), 1);

    QCOMPARE(result.at(0), false);
    QCOMPARE(result.at(1), true);
    QCOMPARE(result.at(2), false);
    QCOMPARE(result.at(3), false);
    QCOMPARE(result.at(4), false);
    QCOMPARE(result.at(5), false);
    QCOMPARE(result.at(6), false);
    QCOMPARE(result.at(7), false);
    QCOMPARE(result.at(8), false);
    QCOMPARE(result.at(9), false);
    QCOMPARE(result.at(10), false);
    QCOMPARE(result.at(11), true);

    quint8 r4;
    QVERIFY(data.readAt(0, r4));
    QCOMPARE(r4, (quint8)12);

    bool initValue2 = true;
    QBitArray result2(10, initValue2);
    data.take(result2);
    QCOMPARE(data.size(), 0);

    QCOMPARE(result2.at(0), false);
    QCOMPARE(result2.at(1), false);
    QCOMPARE(result2.at(2), true);
    QCOMPARE(result2.at(3), true);
    QCOMPARE(result2.at(4), false);
    QCOMPARE(result2.at(5), false);
    QCOMPARE(result2.at(6), false);
    QCOMPARE(result2.at(7), false);
    QCOMPARE(result2.at(8), initValue2);
    QCOMPARE(result2.at(9), initValue2);
}

void DataTests::intToBitArrayTest()
{
    QBitArray bits(10, false);
    Data::intToBitArray(3, 2, bits, 0, Comm::LittleEndian);
    Data::intToBitArray(6, 3, bits, 2, Comm::LittleEndian);
    Data::intToBitArray(21, 5, bits, 5, Comm::LittleEndian);

    QCOMPARE(bits.at(0), true);
    QCOMPARE(bits.at(1), true);
    QCOMPARE(bits.at(2), false);
    QCOMPARE(bits.at(3), true);
    QCOMPARE(bits.at(4), true);
    QCOMPARE(bits.at(5), true);
    QCOMPARE(bits.at(6), false);
    QCOMPARE(bits.at(7), true);
    QCOMPARE(bits.at(8), false);
    QCOMPARE(bits.at(9), true);

    Data::intToBitArray(2, 2, bits, 10, Comm::LittleEndian);
    QCOMPARE(bits.size(), 12);
    QCOMPARE(bits.at(10), false);
    QCOMPARE(bits.at(11), true);

    Data::intToBitArray(3, 2, bits, 12, Comm::BigEndian);
    Data::intToBitArray(6, 3, bits, 14, Comm::BigEndian);
    Data::intToBitArray(25, 5, bits, 17, Comm::BigEndian);
    QCOMPARE(bits.size(), 22);
    QCOMPARE(bits.at(12), true);
    QCOMPARE(bits.at(13), true);
    QCOMPARE(bits.at(14), true);
    QCOMPARE(bits.at(15), true);
    QCOMPARE(bits.at(16), false);
    QCOMPARE(bits.at(17), true);
    QCOMPARE(bits.at(18), true);
    QCOMPARE(bits.at(19), false);
    QCOMPARE(bits.at(20), false);
    QCOMPARE(bits.at(21), true);
}

void DataTests::bitArrayToIntTest()
{
    QBitArray bits(15, false);
    bits[0] = true;
    bits[1] = false;
    bits[2] = false;
    bits[3] = true;
    bits[4] = true;
    bits[5] = true;
    bits[6] = false;
    bits[7] = true;
    bits[8] = false;
    bits[9] = false;
    bits[10] = false;
    bits[11] = true;
    bits[12] = false;
    bits[13] = true;
    bits[14] = true;

    int r1, r2, r3, r4;
    int r5, r6, r7, r8;
    Data::bitArrayToInt(bits, 8, r1, 0, Comm::LittleEndian);
    Data::bitArrayToInt(bits, 4, r2, 8, Comm::LittleEndian);
    Data::bitArrayToInt(bits, 2, r3, 12, Comm::LittleEndian);
    Data::bitArrayToInt(bits, 1, r4, 14, Comm::LittleEndian);

    Data::bitArrayToInt(bits, 8, r5, 0, Comm::BigEndian);
    Data::bitArrayToInt(bits, 4, r6, 8, Comm::BigEndian);
    Data::bitArrayToInt(bits, 2, r7, 12, Comm::BigEndian);
    Data::bitArrayToInt(bits, 1, r8, 14, Comm::BigEndian);

    QCOMPARE(r1, 185);
    QCOMPARE(r2, 8);
    QCOMPARE(r3, 2);
    QCOMPARE(r4, 1);

    QCOMPARE(r5, 157);
    QCOMPARE(r6, 1);
    QCOMPARE(r7, 1);
	QCOMPARE(r8, 1);
}

void DataTests::floatTest()
{
	float f = 42.5;

	Data data;
	data.add(f);

	float res = 0.0;
	data.take(res);

	qDebug() << data;

	QCOMPARE(f, res);
}
