#ifndef DATAMAPTESTS_H
#define DATAMAPTESTS_H

#include <QtTest/QtTest>
#include <QObject>
#include <StrategyMap.h>

class DataMapTests : public QObject
{
    Q_OBJECT
public:
    DataMapTests(QObject *parent = 0);

private:
    StrategyMap* _map;

private slots:
    void collisionCircleCircleTest();
    void benchmarkcollisionCircleCircle();

    void collisionCircleRectangleTest();
    void benchmarkcollisionCircleRectangle();

    void collisionCircleLineTest();
    void benchmarkcollisionCircleLine();

    void collisionCircleTrajectory();

    void collisionRange();
    void robotIsInTableTest();
};

#endif // DATAMAPTESTS_H
