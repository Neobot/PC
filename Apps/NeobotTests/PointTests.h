#ifndef POINTTESTS_H
#define POINTTESTS_H

#include <QtTest/QtTest>
#include "RPoint.h"

Q_DECLARE_METATYPE(Tools::Point)
Q_DECLARE_METATYPE(QList<Tools::Point>)

#define QCOMPARE_RPOINT(p1, p2) QCOMPARE(p1.getX(), p2.getX()) \
				 QCOMPARE(p1.getY(), p2.getY()) \
				 QCOMPARE(p1.getTheta(), p2.getTheta())

namespace QTest
{
    template<>
    char *toString(const Tools::RPoint &p);
}

class PointTests : public QObject
{
    Q_OBJECT

public:
    PointTests(QObject *parent);
    static void fuzzyCompare(const Tools::RPoint& p1, const  Tools::RPoint& p2);
    static void fuzzyCompare(const Tools::Point& p1, const  Tools::Point& p2);

private slots:
    void testConstruction();
    void testDistance();
    void testIsInCircle();
    void testAngle();
    void testDiffAngle();
    void testOperators();
    void testReverse();
    void testGetThetaDegree();
    void testManhattanLength();
    void testRelativePosition();

};

#endif // POINTTESTS_H
