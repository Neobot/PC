#include "DataMapTests.h"

using namespace Tools;

DataMapTests::DataMapTests(QObject *parent) :
    QObject(parent)
{
    _map = new StrategyMap(QSizeF(3000, 2000), 100, 100);
}

void DataMapTests::collisionCircleCircleTest()
{
    CircleObject c1(100, QPointF(200, 200));
    CircleObject c2(100, QPointF(300, 200));
    QCOMPARE(DataMap::collide(&c1, &c2), true);

    c2.setPosition(QPointF(500, 200));
    QCOMPARE(DataMap::collide(&c1, &c2), false);

    c2.setPosition(QPointF(400, 200));
    QCOMPARE(DataMap::collide(&c1, &c2), true);

    c2.setPosition(QPointF(320, 260));
    QCOMPARE(DataMap::collide(&c1, &c2), true);
}

void DataMapTests::collisionCircleRectangleTest()
{
    RectangleObject r1(QRectF(100, 100, 400, 300));
    CircleObject c1(100, QPointF(550, 250));
    QCOMPARE(DataMap::collide(&r1, &c1), true);

    c1.setPosition(QPointF(550, 450));
    QCOMPARE(DataMap::collide(&r1, &c1), true);

    c1.setPosition(QPointF(300, 250));
    QCOMPARE(DataMap::collide(&r1, &c1), true);

    c1.setPosition(QPointF(300, 450));
    QCOMPARE(DataMap::collide(&r1, &c1), true);

    c1.setPosition(QPointF(600, 250));
    QCOMPARE(DataMap::collide(&r1, &c1), true);

    c1.setPosition(QPointF(601, 250));
    QCOMPARE(DataMap::collide(&r1, &c1), false);

    c1.setPosition(QPointF(580, 470));
    QCOMPARE(DataMap::collide(&r1, &c1), false);

    c1.setPosition(QPointF(600, 250));
}

void DataMapTests::collisionCircleLineTest()
{
    LineObject l1(QLineF(50, 300, 600, 300));
    CircleObject c1(200, QPointF(300, 300));
    QCOMPARE(DataMap::collide(&l1, &c1), true);

    l1.setPosition(QPointF(50, 450));
    QCOMPARE(DataMap::collide(&l1, &c1), true);

    l1.setPosition(QPointF(50, 500));
    QCOMPARE(DataMap::collide(&l1, &c1), true);

    l1.setPosition(QPointF(700, 300));
    QCOMPARE(DataMap::collide(&l1, &c1), false);

    l1.setPosition(QPointF(700, 501));
    QCOMPARE(DataMap::collide(&l1, &c1), false);

    LineObject l2(QLineF(250, 50, 450, 250));
    QCOMPARE(DataMap::collide(&l2, &c1), true);

    l2.setPosition(QPointF(450, 250));
    QCOMPARE(DataMap::collide(&l1, &c1), false);

    l1.setPosition(QPointF(50, 500));
}

void DataMapTests::collisionRange()
{
    DataObject* c1 = new CircleObject(100, QPointF(200, 200), 0.0, ZRange(300, 500));
    DataObject* c2 = new CircleObject(100, QPointF(300, 200), 0.0, ZRange(200, 400));
    DataObject* c3 = new CircleObject(100, QPointF(300, 200), 0.0, ZRange(600, 700));
    QCOMPARE(DataMap::collide(c1, c2), true);
    QCOMPARE(DataMap::collide(c2, c3), false);

    delete c1;
    delete c2;
    delete c3;
}

void DataMapTests::robotIsInTableTest()
{
    _map->moveRobot(QPointF(200, 200));
    QCOMPARE(_map->robotIsInTable(), true);

    _map->moveRobot(QPointF(50, 50));
    QCOMPARE(_map->robotIsInTable(), false);

    _map->moveRobot(QPointF(2950, 1950));
    QCOMPARE(_map->robotIsInTable(), false);
}

void DataMapTests::benchmarkcollisionCircleCircle()
{
    CircleObject c1(100, QPointF(200, 200));
    CircleObject c2(100, QPointF(300, 200));

    QBENCHMARK
    {
	DataMap::collide(&c1, &c2);
    }
}

void DataMapTests::benchmarkcollisionCircleRectangle()
{
    RectangleObject r1(QRectF(100, 100, 400, 300));
    CircleObject c1(100, QPointF(550, 250));

    QBENCHMARK
    {
	DataMap::collide(&r1, &c1);
    }
}

void DataMapTests::benchmarkcollisionCircleLine()
{
    LineObject l1(QLineF(50, 300, 600, 300));
    CircleObject c1(200, QPointF(300, 300));

    QBENCHMARK
    {
	DataMap::collide(&l1, &c1);
    }

}

void DataMapTests::collisionCircleTrajectory()
{
    CircleObject r(100, QPointF(0, 0));
    TrajectoryObject to(&r);

    Tools::Trajectory t;
    t << Tools::RPoint(150, 150);
    t << Tools::RPoint(350, 850);
    t << Tools::RPoint(1050, 1050);
    t << Tools::RPoint(1150, 300);

    to.setTrajectory(t);

    CircleObject o(200, QPointF(700, 450));
    QCOMPARE(DataMap::collide(&to, &o), false);

    QBENCHMARK
    {
	DataMap::collide(&to, &o);
    }

    o.setPosition(QPointF(850, 450));
    QCOMPARE(DataMap::collide(&to, &o), true);

    o.setPosition(QPointF(1150, 1300));
    QCOMPARE(DataMap::collide(&to, &o), true);

    o.setPosition(QPointF(1150, 0));
    QCOMPARE(DataMap::collide(&to, &o), true);

    o.setPosition(QPointF(650, 1300));
    QCOMPARE(DataMap::collide(&to, &o), false);

    o.setPosition(QPointF(200, 200));
    QCOMPARE(DataMap::collide(&to, &o), true);
}
