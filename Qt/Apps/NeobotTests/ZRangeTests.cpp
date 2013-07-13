#include "ZRangeTests.h"
#include <ZRange.h>

ZRangeTests::ZRangeTests(QObject *parent) :
    QObject(parent)
{
}

void ZRangeTests::heightTest()
{
    ZRange z;
    QCOMPARE(z.height(), 999.0);

    ZRange z2(200, 300);
    QCOMPARE(z2.height(), 100.0);

    ZRange z3(400, 212);
    QCOMPARE(z3.height(), -188.0);

    ZRange z4(200, 200);
    QCOMPARE(z4.height(), 0.0);
}

void ZRangeTests::isInRangeTest()
{
    ZRange z2(200, 300);
    QCOMPARE(z2.isInRange(250), true);
    QCOMPARE(z2.isInRange(200), true);
    QCOMPARE(z2.isInRange(300), true);
    QCOMPARE(z2.isInRange(312), false);
    QCOMPARE(z2.isInRange(300.000001), false);
    QCOMPARE(z2.isInRange(-242), false);
}

void ZRangeTests::equalTest()
{
    QCOMPARE(ZRange(232, 255) == ZRange(232, 255), true);
    QCOMPARE(ZRange(233, 255) == ZRange(232, 255), false);
    QCOMPARE(ZRange(232, 254) == ZRange(232, 255), false);
}

void ZRangeTests::intersectionTest()
{
    ZRange z1(200, 300);
    ZRange z2(100, 250);
    ZRange z3(220, 400);
    ZRange z4(100, 300);
    ZRange z5(100, 200);
    ZRange z6(100, 182);
    ZRange z7(312, 500);
    ZRange z8(212, 212);
    ZRange z9(100, 400);

    QCOMPARE(z1.intersected(z2), true);
    QCOMPARE(z1.intersected(z3), true);
    QCOMPARE(z1.intersected(z4), true);
    QCOMPARE(z1.intersected(z5), false);
    QCOMPARE(z1.intersected(z6), false);
    QCOMPARE(z1.intersected(z7), false);
    QCOMPARE(z1.intersected(z8), true);
    QCOMPARE(z1.intersected(z9), true);

    QCOMPARE(z8.intersected(z1), true);
    QCOMPARE(z1.intersected(z1), true);
}

void ZRangeTests::intersectedTest()
{
    ZRange z1(200, 300);
    ZRange z2(100, 250);
    ZRange z3(220, 400);
    ZRange z4(100, 300);
    ZRange z5(100, 200);
    ZRange z6(100, 182);
    ZRange z7(312, 500);
    ZRange z8(212, 212);
    ZRange z9(100, 400);


    QCOMPARE(z1.intersection(z2), ZRange(200, 250));
    QCOMPARE(z1.intersection(z3), ZRange(220, 300));
    QCOMPARE(z1.intersection(z4), ZRange(200, 300));
    QCOMPARE(z1.intersection(z5), ZRange(0, 0));
    QCOMPARE(z1.intersection(z6), ZRange(0, 0));
    QCOMPARE(z1.intersection(z7), ZRange(0, 0));
    QCOMPARE(z1.intersection(z8), ZRange(212, 212));
    QCOMPARE(z1.intersection(z9), ZRange(200, 300));

    QCOMPARE(z8.intersection(z1), ZRange(212, 212));
    QCOMPARE(z1.intersection(z1), ZRange(200, 300));
}
