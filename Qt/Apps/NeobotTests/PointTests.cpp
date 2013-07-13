#include "PointTests.h"
#include "ToolsLib.h"

#include "vmath.h"
using namespace Tools;

namespace QTest
{
     template<>
     char *toString(const RPoint &p)
     {
         QByteArray ba = "RPoint(";
         ba += QByteArray::number(p.getX());
         ba += ", ";
         ba += QByteArray::number(p.getY());
         ba += " - ";
         ba += QByteArray::number(p.getTheta());
         ba += ")";
         return qstrdup(ba.data());
     }
 }

PointTests::PointTests(QObject *parent) : QObject(parent) {}

void PointTests::testConstruction()
{
    RPoint p1(10.5,20.42, 2.234);
    QCOMPARE(p1.getX(),10.5);
    QCOMPARE(p1.getY(),20.42);
    QCOMPARE(p1.getTheta(),2.234);
}
void PointTests::testDistance()
{
    RPoint p1(1,5);
    RPoint p2(4,5);
    RPoint p3(1,1);

    double d12 = p1.distance(p2);
    double d21 = p2.distance(p1);
    QVERIFY(d12==d21);
    QCOMPARE(d12,3.0);

    double d13 = p1.distance(p3);
    double d31 = p3.distance(p1);
    QVERIFY(d13==d31);
    QCOMPARE(d13,4.0);

    double d23 = p2.distance(p3);
    double d32 = p3.distance(p2);
    QVERIFY(d23==d32);
    QCOMPARE(d23,5.0);
}

void PointTests::testIsInCircle()
{
    RPoint p1(1,5);
    RPoint p2(4,5);
    RPoint p3(1,1);

    QCOMPARE(p1.isInCircle(p1, -1.0), false);
    QCOMPARE(p1.isInCircle(p1, 0.0), true);
    QCOMPARE(p1.isInCircle(p2, 0.0), false);
    QCOMPARE(p1.isInCircle(p2, 3.5), true);
    QCOMPARE(p1.isInCircle(p3, 3.5), false);
}
void PointTests::testAngle()
{
    RPoint p0(0,0);
    RPoint p1(1,5);
    RPoint p2(4,5);
    RPoint p3(1,2);
    RPoint p4(-1,1);


    QCOMPARE(p1.angle(p2), 0.0);
    QCOMPARE(p2.angle(p1), pi);

    QCOMPARE(p1.angle(p3), -pi/2);
    QCOMPARE(p3.angle(p1), pi/2);

    QCOMPARE(p2.angle(p3),-3*pi/4);
    QCOMPARE(p3.angle(p2),pi/4);

    QCOMPARE(p1.angle(p1),0.0);

    QCOMPARE(p0.angle(p4),3 * pi / 4);

}
void PointTests::testDiffAngle()
{
     RPoint p1(0,0, pi/3);
     RPoint p2(0,0, pi/4);

     QCOMPARE(p1.diffAngle(p2),pi/12);
     QCOMPARE(p2.diffAngle(p1),-pi/12);
     QCOMPARE(p1.diffAngle(p1),0.0);
}
void PointTests::testOperators()
{
    RPoint p1(1,5);
    RPoint p2(2,3);
    double a = 2.5;

    QCOMPARE(p1 + p2, RPoint(3,8));
    QCOMPARE(p1 - p2, RPoint(-1,2));
    QCOMPARE(p1 * a, RPoint(2.5,12.5));
    QCOMPARE(p1 / a, RPoint(0.4,2.0));

    QCOMPARE(p1==p1, true);
    QCOMPARE(p1!=p2, true);
}

void PointTests::testReverse()
{
    Point p1(1,5);
    Point p2(2,3);

    Point r1 = p1.reverse();
    Point r2 = p2.reverse();

    QCOMPARE(r1, Point(5, 1));
    QCOMPARE(r2, Point(3, 2));
}

void PointTests::testGetThetaDegree()
{
    RPoint p1(0, 0, 2*pi/3);
    QCOMPARE(p1.getThetaDegree(), 120);
}

void PointTests::fuzzyCompare(const RPoint& p1, const RPoint& p2)
{
    fuzzyCompare((Point)p1, (Point)p2);
    QCOMPARE(p1.getTheta(), p2.getTheta());
}

void PointTests::fuzzyCompare(const Point& p1, const Point& p2)
{
    QCOMPARE(p1.getX(), p2.getX());
    QCOMPARE(p1.getY(), p2.getY());
}

void PointTests::testManhattanLength()
{
    RPoint p0(0,0);
    RPoint p1(1,5);
    RPoint p2(4,5);
    RPoint p3(1,2);
    RPoint p4(-1,1);
    RPoint p5(-2.2,-4.4);

    QCOMPARE(p0.manhattanLength(), 0.0);
    QCOMPARE(p1.manhattanLength(), 6.0);
    QCOMPARE(p2.manhattanLength(), 9.0);
    QCOMPARE(p3.manhattanLength(), 3.0);
    QCOMPARE(p4.manhattanLength(), 2.0);
    QCOMPARE(p5.manhattanLength(), 6.6);
}

void PointTests::testRelativePosition()
{
    QPointF p1 = relativePosition(QPointF(1.2, 4.7), QPointF(0,0), QVector2D(3, 0), 4, pi);
    QCOMPARE(p1, QPointF(-2.8, 4.7));

    QPointF p2 = relativePosition(QPointF(1.2, 4.7), QPointF(0,0), QVector2D(0, 15), 4, pi);
    QCOMPARE(p2, QPointF(1.2, 0.7));

    QPointF p3 = relativePosition(QPointF(1.2, 4.7), QPointF(0,0), QVector2D(0, 15), 4, pi/2);
    QCOMPARE(p3, QPointF(-2.8, 4.7));

    QPointF p4 = relativePosition(QPointF(1.2, 4.7), QPointF(0,0), QVector2D(2, 0), 4, pi/4);
    QCOMPARE(p4, QPointF(1.2 + cos(pi/4)*4, 4.7 + sin(pi/4)*4));
}
