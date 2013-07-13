#include "CurveTests.h"

#include "Curve.h"
#include "Point.h"
#include "PointTests.h"
#include "ToolsLib.h"

using namespace Tools;

CurveTests::CurveTests(QObject *parent) : QObject(parent) {}

void CurveTests::testConstruction()
{
    Curve c;
    c.append(Point(1,7));
    c.append(Point(1,2));
    c.append(Point(3,4));

    QCOMPARE(c.size(), 3);
    QCOMPARE(c.value(0).getX(), 1.0);
    QCOMPARE(c.value(0).getY(), 7.0);
}

void CurveTests::testBezier_data()
{
    QTest::addColumn<Point>("P1");
    QTest::addColumn<Point>("P2");
    QTest::addColumn<Point>("P3");
    QTest::addColumn<Point>("P4");
    QTest::addColumn<QList<Point> >("resultList");
    QTest::addColumn<int>("nbPoint");

    QList<Point> firtsTestList;
    firtsTestList <<  Point(0,0) << Point(0.3,0.27) << Point(0.6,0.48) << Point(0.9,0.63) << Point(1.2,0.72)
            << Point(1.5,0.75) << Point(1.8,0.72) << Point(2.1,0.63) << Point(2.4,0.48) << Point(2.7,0.27) << Point(3,0);

    QTest::newRow("First Test") << Point(0,0) << Point(1,1) << Point(2,1) << Point(3,0) << firtsTestList << 11;
}

void CurveTests::testBezier()
{
    QFETCH(Point, P1);
    QFETCH(Point, P2);
    QFETCH(Point, P3);
    QFETCH(Point, P4);
    QFETCH(QList<Point>, resultList);
    QFETCH(int, nbPoint);

    Curve C;

    C = Curve::bezier(P1, P2, P3, P4, nbPoint);

    QCOMPARE(C.size(),nbPoint);
    QVERIFY2(C.size() == resultList.size(), "Wrong number of Point in the restlt list.");

    int courbeIndex = 0;
    for (Curve::const_iterator it = resultList.constBegin(); it != resultList.constEnd(); ++it)
    {
        PointTests::fuzzyCompare(C.value(courbeIndex), *it);
        ++courbeIndex;
    }

#ifdef DEBUG_MESSAGE
    int i =0;
    for (Courbe::iterator it = C.begin(); it != C.end(); ++it)
    {
        qDebug("%i : %f - %f",i,(*it).x(), (*it).y());
        ++i;
    }
#endif
}

void CurveTests::testInterpolate()
{
    Point p1(0,0);
    Point p2(3, 3);
    Point p3(5, 4);

    QCOMPARE(Curve::interpolateX(p1, p2, 1.5), 1.5);
    QCOMPARE(Curve::interpolateX(p1, p2, -2.0), -2.0);
    QCOMPARE(Curve::interpolateX(p2, p3, 4.0), 3.5);
    QCOMPARE(Curve::interpolateX(p2, p3, 0.0), 1.5);

    QCOMPARE(Curve::interpolateX(p2, p2, 0.0), 3.0);
    QCOMPARE(Curve::interpolateX(p3, p2, 4.0), 3.5);
}

void CurveTests::testMinMax()
{
    Curve c;
    c.append(Point(1,7));

    QCOMPARE(c.maxYValue(), 7.0);
    QCOMPARE(c.minYValue(), 7.0);

    c.append(Point(2.2,2));

    QCOMPARE(c.maxYValue(), 7.0);
    QCOMPARE(c.minYValue(), 2.0);

    c.append(Point(3.8,-12));
    c.append(Point(0.5,33));
    c.append(Point(9.1,22));
    c.append(Point(-5,42));

    QCOMPARE(c.maxYValue(), 42.0);
    QCOMPARE(c.minYValue(), -12.0);

    c.append(Point(-9,42));

    QCOMPARE(c.maxYValue(), 42.0);
}

void CurveTests::testSortPointsOnAbscissa()
{
    Curve c;
    c.append(Point(1,7));
    c.append(Point(2.2,2));
    c.append(Point(3.8,-12));
    c.append(Point(0.5,33));
    c.append(Point(9.1,22));
    c.append(Point(-5,42));

    c.sortPointsOnAbscissa();

    QCOMPARE(c.value(0), Point(-5, 42));
    QCOMPARE(c.value(1), Point(0.5, 33));
    QCOMPARE(c.value(2), Point(1, 7));
    QCOMPARE(c.value(3), Point(2.2, 2));
    QCOMPARE(c.value(4), Point(3.8, -12));
    QCOMPARE(c.value(5), Point(9.1, 22));
}

void CurveTests::testYValue()
{
    Curve c;
    c.append(Point(-1,2));
    c.append(Point(0,0));
    c.append(Point(1,2));
    c.append(Point(3, 4));

    QCOMPARE(c.yValue(1.0), 2.0);
    QCOMPARE(c.yValue(-1.0), 2.0);
    QCOMPARE(c.yValue(0.75), 1.5);
    QCOMPARE(c.yValue(3), 4.0);
    QCOMPARE(c.yValue(-10), 0.0);
    QCOMPARE(c.yValue(10), 0.0);

    c.append(Point(2.5,3));
    c.append(Point(0, 1.5));

    QCOMPARE(c.yValue(2.5), 3.5);
    QCOMPARE(c.yValue(0.0), 0.0);

    QList<double> yValues;
    yValues = c.yValues(2.5);
    QCOMPARE(yValues.size(), 2);
    QCOMPARE(yValues.value(0), 3.5);
    QCOMPARE(yValues.value(1), 3.0);

    yValues = c.yValues(0.0);
    QCOMPARE(yValues.size(), 2);
    QCOMPARE(yValues.value(0), 0.0);
    QCOMPARE(yValues.value(1), 1.5);

    yValues = c.yValues(-5);
    QCOMPARE(yValues.size(), 0);

    yValues = c.yValues(42);
    QCOMPARE(yValues.size(), 0);

    yValues = c.yValues(-0.75);
    QCOMPARE(yValues.size(), 1);
    QCOMPARE(yValues.value(0), 1.5);

    yValues = c.yValues(-1);
    QCOMPARE(yValues.size(), 1);
    QCOMPARE(yValues.value(0), 2.0);

    yValues = c.yValues(3);
    QCOMPARE(yValues.size(), 1);
    QCOMPARE(yValues.value(0), 4.0);
}

void CurveTests::testYValue2()
{
    Curve c;
    c.append(Point( 0, 0));
    c.append(Point(-1, 2));
    c.append(Point(-3, 2));
    c.append(Point( 3, 4));
    c.append(Point( 0, 0));

    QCOMPARE(c.yValue(-0.5), 1.0);
    QCOMPARE(c.yValue(0), 0.0);
    QCOMPARE(c.yValue(-2.24242), 2.0);
    QCOMPARE(c.yValue(-3), 2.0);
    QCOMPARE(c.yValue(3), 4.0);

    QList<double> yValues;
    yValues = c.yValues(0.0);
    QCOMPARE(yValues.size(), 3);
    QCOMPARE(yValues.value(0), 0.0);
    QCOMPARE(yValues.value(1), 3.0);
    QCOMPARE(yValues.value(2), 0.0);

    yValues = c.yValues(-2.25);
    QCOMPARE(yValues.size(), 2);
    QCOMPARE(yValues.value(0), 2.0);
    QCOMPARE(yValues.value(1), 2.25);

    yValues = c.yValues(-3);
    QCOMPARE(yValues.size(), 1);
    QCOMPARE(yValues.value(0), 2.0);

    yValues = c.yValues(3);
    QCOMPARE(yValues.size(), 1);
    QCOMPARE(yValues.value(0), 4.0);
}

void CurveTests::testXValue()
{
    Curve c;
    c.append(Point( 0, 0));
    c.append(Point(-1, 2));
    c.append(Point(-3, 2));
    c.append(Point( 3, 4));
    c.append(Point( 0, 0));

    QList<double> xValues;

    QCOMPARE(c.xValue(3.0), 0.0);
    xValues = c.xValues(3.0);
    QCOMPARE(xValues.size(), 2);
    QCOMPARE(xValues.value(0), 0.0);
    QCOMPARE(xValues.value(1), 2.25);


    QCOMPARE(c.xValue(2.0), -1.0);
    xValues = c.xValues(2.0);
    QCOMPARE(xValues.size(), 3);
    QCOMPARE(xValues.value(0), -1.0);
    QCOMPARE(xValues.value(1), -3.0);
    QCOMPARE(xValues.value(2), 1.5);


    QCOMPARE(c.xValue(0.0), 0.0);
    xValues = c.xValues(0.0);
    QCOMPARE(xValues.size(), 2);
    QCOMPARE(xValues.value(0), 0.0);
    QCOMPARE(xValues.value(0), 0.0);


    QCOMPARE(c.xValue(4.0), 3.0);
    xValues = c.xValues(4.0);
    QCOMPARE(xValues.size(), 1);
    QCOMPARE(xValues.value(0), 3.0);
}

void CurveTests::testToTrajectory()
{
    Curve c;
    c.append(Point( 0, 0));
    c.append(Point(-1, 2));
    c.append(Point(-3, 2));
    c.append(Point( 3, 4));
    c.append(Point( 0, 0));

    Trajectory t = c.toTrajectory();

    QCOMPARE(c.size(), t.size());
    QCOMPARE(t.value(0).getThetaDegree(), 90 + 27);
    QCOMPARE(t.value(1).getThetaDegree(), 180);
    QCOMPARE(t.value(2).getThetaDegree(), 18);
    QCOMPARE(t.value(3).getThetaDegree(), 233);
    QCOMPARE(t.value(4).getThetaDegree(), 233);
}

void CurveTests::testTrajectoryStraightLineSmoothing()
{
    Curve c;
    c.append(Point(0, 0));
    c.append(Point(1, 1));
    c.append(Point(2, 2));
    c.append(Point(3, 3));
    c.append(Point(4, 3));
    c.append(Point(5, 3));
    c.append(Point(8, 3));
    c.append(Point(6, 6));
    c.append(Point(2, 12));
    c.append(Point(0, 15));

    Trajectory t = c.toTrajectory();

    Tools::trajectoryStraightLineSmoothing(t);

    QCOMPARE(t.size(), 4);
}

void CurveTests::testTrajectoryDiagonalSmoothing()
{
    Curve c;
    c.append(Point(0, 0));
    c.append(Point(1, 1));
    c.append(Point(1, 2));
    c.append(Point(2, 3));
    c.append(Point(2, 4));
    c.append(Point(3, 5));
    c.append(Point(5, 5));

    Trajectory t = c.toTrajectory();
    Tools::trajectoryDiagonalSmoothing(t, 200);

    QCOMPARE(t.size(), 3);
    QCOMPARE(t.value(0).toPoint(), Point(0, 0));
    QCOMPARE(t.value(1).toPoint(), Point(3, 5));
    QCOMPARE(t.value(2).toPoint(), Point(5, 5));


    c.clear();
    c.append(Point(0, 0));
    c.append(Point(1, 1));
    c.append(Point(3, 1));
    c.append(Point(4, 2));
    c.append(Point(6, 2));
    c.append(Point(7, 3));
    c.append(Point(9, 3));
    c.append(Point(10, 4));
    c.append(Point(13, 4));
    c.append(Point(14, 5));
    c.append(Point(17, 5));
    c.append(Point(17, 10));

    //qDebug("2");
    t = c.toTrajectory();
    Tools::trajectoryDiagonalSmoothing(t, 200);

    QCOMPARE(t.size(), 4);
    QCOMPARE(t.value(0).toPoint(), Point(0, 0));
    QCOMPARE(t.value(1).toPoint(), Point(10, 4));
    QCOMPARE(t.value(2).toPoint(), Point(17, 5));
    QCOMPARE(t.value(3).toPoint(), Point(17, 10));



    c.clear();
    c.append(Point(10, 10));
    c.append(Point(8, 10));
    c.append(Point(7, 11));
    c.append(Point(4, 8));
    c.append(Point(3, 8));
    c.append(Point(0, 5));
    c.append(Point(0, 3));
    c.append(Point(1, 2));
    c.append(Point(2, 2));
    c.append(Point(3, 1));
    c.append(Point(6, 1));
    c.append(Point(8, 3));
    c.append(Point(10, 3));
    c.append(Point(12, 5));
    c.append(Point(12, 7));
    c.append(Point(13, 8));
    c.append(Point(13, 14));

    //qDebug("3");
    t = c.toTrajectory();
    Tools::trajectoryDiagonalSmoothing(t, 200);

    QCOMPARE(t.size(), 9);
    QCOMPARE(t.value(0).toPoint(), Point(10, 10));
    QCOMPARE(t.value(1).toPoint(), Point(7, 11));
    QCOMPARE(t.value(2).toPoint(), Point(0, 5));
    QCOMPARE(t.value(3).toPoint(), Point(1, 2));
    QCOMPARE(t.value(4).toPoint(), Point(3, 1));
    QCOMPARE(t.value(5).toPoint(), Point(6, 1));
    QCOMPARE(t.value(6).toPoint(), Point(12, 5));
    QCOMPARE(t.value(7).toPoint(), Point(13, 8));
    QCOMPARE(t.value(8).toPoint(), Point(13, 14));

    //qDebug("4");

    t = c.toTrajectory();
    Tools::trajectoryDiagonalSmoothing(t, 4);
    QCOMPARE(t.size(), 13);
}

void CurveTests::testTrajectoryStopAtDistance()
{
    Curve c;
    c.append(Point(0, 0));
    c.append(Point(1, 1));
    c.append(Point(2, 2));
    c.append(Point(3, 3));
    c.append(Point(4, 4));
    c.append(Point(5, 5));
    c.append(Point(6, 6));

    Trajectory t0 = c.toTrajectory();
    Tools::trajectoryStopAtDistance(t0,RPoint(6, 6), 50);
    QCOMPARE(t0.size(), 0);

    Trajectory t = c.toTrajectory();
    Tools::trajectoryStopAtDistance(t,RPoint(6, 6), 0);
    QCOMPARE(t.size(), 7);

    Trajectory t2 = c.toTrajectory();
    Tools::trajectoryStopAtDistance(t2,RPoint(6, 6), 1);
    QCOMPARE(t2.size(), 6);

    Trajectory t3 = c.toTrajectory();
    Tools::trajectoryStopAtDistance(t3,RPoint(6, 6), 2);
    QCOMPARE(t3.size(), 5);

    Trajectory t4 = c.toTrajectory();
    Tools::trajectoryStopAtDistance(t4,RPoint(6, 6), 3);
    QCOMPARE(t4.size(), 4);

    Trajectory t5 = c.toTrajectory();
    Tools::trajectoryStopAtDistance(t5,RPoint(6, 6), 4);
    QCOMPARE(t5.size(), 4);

    Trajectory t6 = c.toTrajectory();
    Tools::trajectoryStopAtDistance(t6,RPoint(6, 6), 4.3);
    QCOMPARE(t6.size(), 3);
}
