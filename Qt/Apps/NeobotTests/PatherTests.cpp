#include "PatherTests.h"
#include "NMicropather.h"
#include <StrategyMap.h>
#include "PointTests.h"
#include <vmath.h>
#include "ToolsLib.h"

using namespace Tools;

/*-------------------------Pather test options-----------------------------------*/
//#define RUN_BENCHMARK         //Benchmark the obstacle plans
#define DISPLAY_TRAJECTORIES    //Show the planned trajectories
#define SMOOTHING               //Smooth the planned trajectories
/*-------------------------------------------------------------------------------*/


PatherTests::PatherTests(QObject *parent) : QObject(parent)
{
    _logger = new Tools::StdoutLogger;
    _pathers << new NMicropather(_logger, NMicropather::Manhattan)
             << new NMicropather(_logger, NMicropather::Diagonal)
             << new NMicropather(_logger, NMicropather::Euclidean)
             << new NMicropather(_logger, NMicropather::None)
             ;
    _map = 0;

    _grid = new Tools::NGrid;

    _logger->noLogger(true);
}

PatherTests::~PatherTests()
{
    delete _logger;
    qDeleteAll(_pathers);
}

void PatherTests::resetMap()
{
    if (!_map)
        _map = new StrategyMap(QSizeF(2900, 1900), 100, 100);
    else
        _map->clear();
    _map->manageObjectOwnership(true);
}

int PatherTests::analyseTrajectory(Tools::Trajectory& t)
{
    int nbOfTurn = 0;

    if (!t.isEmpty())
    {
        Tools::Trajectory t2 = t;
#ifdef SMOOTHING
        Tools::trajectoryStraightLineSmoothing(t2);
        Tools::trajectoryDiagonalSmoothing(t2, 400);
#endif


        double currentAngle = 0;
        foreach(RPoint p, t2)
        {
            if (p.getTheta() != currentAngle)
                nbOfTurn++;
            currentAngle = p.getTheta();
        }

        qDebug() << "Result: " << t.count() << " moves with " << nbOfTurn << " turn";
    }
    else
        qDebug() << "No trajectory found!!!";

    return nbOfTurn;
}

void PatherTests::displayTrajectory(const Tools::Trajectory& t)
{
#ifdef DISPLAY_TRAJECTORIES
    foreach(RPoint p, t)
        qDebug() << "RPoint(" << p.getX() << p.getY() << p.getThetaDegree() << ")";
#endif
}

void PatherTests::preparePatherData()
{
    QTest::addColumn<int>("PATHER");
//    QTest::newRow("Micropather Manhattan") << 0;
//    QTest::newRow("Micropather Diagonal") << 1;
    QTest::newRow("Micropather Euclidean") << 2;
//    QTest::newRow("Micropather None") << 3;
}

void PatherTests::emptyTablePlan_data()
{
    preparePatherData();
}

void PatherTests::emptyTablePlan()
{
    //init test
    QFETCH(int, PATHER);
    Pather* p = _pathers.value(PATHER);

    //init environment
    _grid->clear();
    _grid->makeStandardGrid(QPointF(0,0), 200, 200,  QSizeF(2000,3000), NGrid::HeightConnections);
    resetMap();

    NGridNode* start = _grid->getNearestNode(QPointF(0,0));
    NGridNode* destination = _grid->getNearestNode(QPointF(2000,3000));

     _map->moveRobot(start->getPosition());

    //init pather
    p->init(_map, _grid, start);

    Tools::Trajectory result;

#ifdef RUN_BENCHMARK
    QBENCHMARK
#endif
    {       
        //replan
        p->prepareReplan(destination);
        p->replanTrajectory(result);

    }
    //p->beQuiet(false);

    //check result
    QVERIFY(result.count() >= 3000/200);
    QCOMPARE(result.first().toPoint(), RPoint(0, 0).toPoint());
    QCOMPARE(result.last().toPoint(), RPoint(2000, 3000).toPoint());

#ifndef RUN_BENCHMARK
    displayTrajectory(result);
    int turns = analyseTrajectory(result);
    QVERIFY(turns <= 2);
#endif
}

void PatherTests::obstaclePlan_data()
{
    preparePatherData();
}

void PatherTests::obstaclePlan()
{
    //init test
    QFETCH(int, PATHER);
    Pather* p = _pathers.value(PATHER);

    //init environment
    _grid->clear();
    _grid->makeStandardGrid(QPointF(0,0), 200, 200,  QSizeF(2000,3000), NGrid::HeightConnections);
    resetMap();
    _map->addObstacle(new CircleObject(400, QPointF(0, 1400)));

    NGridNode* start = _grid->getNearestNode(QPointF(0,0));
    NGridNode* destination = _grid->getNearestNode(QPointF(0,3000));

    //init pather
    p->init(_map, _grid, start);

     _map->moveRobot(start->getPosition());

    Tools::Trajectory result;
#ifdef RUN_BENCHMARK
    QBENCHMARK
#endif
    {
        //replan
        p->prepareReplan(destination);
        p->replanTrajectory(result);
    }

    //check result
    QVERIFY(result.count() >= 3000/200);
    QCOMPARE(result.first().toPoint(), RPoint(0, 0).toPoint());
    QCOMPARE(result.last().toPoint(), RPoint(0, 3000).toPoint());

#ifndef RUN_BENCHMARK
    displayTrajectory(result);
    int turns = analyseTrajectory(result);
    QVERIFY(turns <= 4);
#endif
}

void PatherTests::obstaclePlan2_data()
{
    preparePatherData();
}

void PatherTests::obstaclePlan2()
{
    //init test
    QFETCH(int, PATHER);
    Pather* p = _pathers.value(PATHER);

    //init environment
    _grid->clear();
    _grid->makeStandardGrid(QPointF(0,0), 200, 200,  QSizeF(2000,3000), NGrid::HeightConnections);
    resetMap();
    _map->addObstacle(new CircleObject(400, QPointF(1000, 1000)));
    _map->addObstacle(new CircleObject(400, QPointF(400, 1600)));
    _map->addObstacle(new CircleObject(400, QPointF(2000, 2000)));

    NGridNode* start = _grid->getNearestNode(QPointF(0,0));
    NGridNode* destination = _grid->getNearestNode(QPointF(2000,3000));

     _map->moveRobot(start->getPosition());

    //init pather
    p->init(_map, _grid, start);

    Tools::Trajectory result;
#ifdef RUN_BENCHMARK
    QBENCHMARK
#endif
    {
        //replan
        p->prepareReplan(destination);
        p->replanTrajectory(result);
    }

    //check result
    QVERIFY(result.count() >= 3000/200);
    QCOMPARE(result.first().toPoint(), RPoint(0, 0).toPoint());
    QCOMPARE(result.last().toPoint(), RPoint(2000, 3000).toPoint());

#ifndef RUN_BENCHMARK
    displayTrajectory(result);
    int turns = analyseTrajectory(result);
    QVERIFY(turns <= 7);
#endif
}

void PatherTests::multiPlan_data()
{
    preparePatherData();
}

void PatherTests::multiPlan()
{
    //init test
    QFETCH(int, PATHER);
    Pather* p = _pathers.value(PATHER);

    //init environment
    _grid->clear();
    _grid->makeStandardGrid(QPointF(0,0), 200, 200,  QSizeF(2000,3000), NGrid::HeightConnections);
    resetMap();
    _map->addObstacle(new CircleObject(400, QPointF(1000, 1000)));
    _map->addObstacle(new CircleObject(400, QPointF(400, 1600)));
    _map->addObstacle(new CircleObject(400, QPointF(2000, 2000)));

    NGridNode* start = _grid->getNearestNode(QPointF(0,0));
    NGridNode* destination1 = _grid->getNearestNode(QPointF(200,2200));
    NGridNode* destination2 = _grid->getNearestNode(QPointF(1800,2800));
    NGridNode* destination3 = _grid->getNearestNode(QPointF(2000,1200));
    NGridNode* destinationFinale = _grid->getNearestNode(QPointF(0,0));

    //init pather
    p->init(_map, _grid, start);

    Tools::Trajectory result;
    Tools::Trajectory total;

    //init position
     _map->moveRobot(start->getPosition());

    //replan
    p->prepareReplan(destination1);
    p->replanTrajectory(result);
    total.append(result);
    QVERIFY(result.count() > 0);

    p->prepareReplan(destination2);
    p->replanTrajectory(result);
    total.append(result);
    QVERIFY(result.count() > 0);


    p->prepareReplan(destination3);
    p->replanTrajectory(result);
    total.append(result);
    QVERIFY(result.count() > 0);


    p->prepareReplan(destinationFinale);
    p->replanTrajectory(result);
    total.append(result);
    QVERIFY(result.count() > 0);


    //check result
    QCOMPARE(total.first().toPoint(), RPoint(0, 0).toPoint());
    QCOMPARE(total.last().toPoint(), RPoint(0, 0).toPoint());

#ifndef RUN_BENCHMARK
    displayTrajectory(total);
    analyseTrajectory(total);
#endif
}

void PatherTests::unreachableDestination_data()
{
    preparePatherData();
}

void PatherTests::unreachableDestination()
{
    //init test
    QFETCH(int, PATHER);
    Pather* p = _pathers.value(PATHER);

    //init environment
    _grid->clear();
    _grid->makeStandardGrid(QPointF(0,0), 200, 200,  QSizeF(2000,3000), NGrid::HeightConnections);
    resetMap();
    _map->moveOpponent(QPointF(1000, 2000));

    NGridNode* start = _grid->getNearestNode(QPointF(0,0));
    NGridNode* destination = _grid->getNearestNode(QPointF(1000,2000));

     _map->moveRobot(start->getPosition());

    //init pather
    p->init(_map, _grid, start);

    //replan
    p->prepareReplan(destination);
    Tools::Trajectory result;
    p->replanTrajectory(result);

    //check result
    QCOMPARE(result.count(), 0);
    displayTrajectory(result);
}

void PatherTests::forceUnreachableDestination_data()
{
    preparePatherData();
}

void PatherTests::forceUnreachableDestination()
{
    //init test
    QFETCH(int, PATHER);
    Pather* p = _pathers.value(PATHER);

    //init environment
    _grid->clear();
    _grid->makeStandardGrid(QPointF(0,0), 200, 200,  QSizeF(2000,3000), NGrid::HeightConnections);
    resetMap();
    _map->moveOpponent(QPointF(1000, 2000));

    NGridNode* start = _grid->getNearestNode(QPointF(0,0));
    NGridNode* destination = _grid->getNearestNode(QPointF(1000,2000));

     _map->moveRobot(start->getPosition());

    //init pather
    p->init(_map, _grid, start);

    //replan
    p->prepareReplan(destination, QList<QRectF>(), true);
    Tools::Trajectory result;
    p->replanTrajectory(result);

    //check result
    QVERIFY(result.count() >= 2000/200);
    QCOMPARE(result.first().toPoint(), RPoint(0, 0).toPoint());
    QCOMPARE(result.last().toPoint(), RPoint(1000, 2000).toPoint());

#ifndef RUN_BENCHMARK
    int turns = analyseTrajectory(result);
    QVERIFY(turns <= 3);
    displayTrajectory(result);
#endif
}

void PatherTests::blockedWay_data()
{
    preparePatherData();
}

void PatherTests::blockedWay()
{
    //init test
    QFETCH(int, PATHER);
    Pather* p = _pathers.value(PATHER);

    //init environment
    _grid->clear();
    _grid->makeStandardGrid(QPointF(0,0), 200, 200,  QSizeF(2000,3000), NGrid::HeightConnections);
    resetMap();
	_map->addObstacle(new RectangleObject(QRectF(2000, 1200, -2000, -400)));

    NGridNode* start = _grid->getNearestNode(QPointF(0,0));
    NGridNode* destination = _grid->getNearestNode(QPointF(0,3000));

     _map->moveRobot(start->getPosition());

    //init pather
    p->init(_map, _grid, start);

    //replan
    p->prepareReplan(destination);
    Tools::Trajectory result;
    p->replanTrajectory(result);

    //check result
    displayTrajectory(result);
    QCOMPARE(result.count(), 0);
}

void PatherTests::forceBlockedWay_data()
{
    preparePatherData();
}

void PatherTests::forceBlockedWay()
{
    //init test
    QFETCH(int, PATHER);
    Pather* p = _pathers.value(PATHER);

    //init environment
    _grid->clear();
    _grid->makeStandardGrid(QPointF(0,0), 200, 200,  QSizeF(2000,3000), NGrid::HeightConnections);
    resetMap();
    _map->addObstacle(new RectangleObject(QRectF(0, 1200, 2000, 400)));

    NGridNode* start = _grid->getNearestNode(QPointF(0,0));
    NGridNode* destination = _grid->getNearestNode(QPointF(0,3000));

     _map->moveRobot(start->getPosition());

    //init pather
    p->init(_map, _grid, start);

    //replan
    p->prepareReplan(destination, QList<QRectF>(), true);
    Tools::Trajectory result;
    p->replanTrajectory(result);

    //check result
    QVERIFY(result.count() >= 3000/200);
    QCOMPARE(result.first().toPoint(), RPoint(0, 0).toPoint());
    QCOMPARE(result.last().toPoint(), RPoint(0, 3000).toPoint());

#ifndef RUN_BENCHMARK
    int turns = analyseTrajectory(result);
    QVERIFY(turns <= 1);
    displayTrajectory(result);
#endif
}

void PatherTests::startFromBlockedPosition_data()
{
    preparePatherData();
}

void PatherTests::startFromBlockedPosition()
{
    //init test
    QFETCH(int, PATHER);
    Pather* p = _pathers.value(PATHER);

    //init environment
    _grid->clear();
    _grid->makeStandardGrid(QPointF(0,0), 200, 200,  QSizeF(2000,3000), NGrid::HeightConnections);
    resetMap();
    _map->addObstacle(new CircleObject(200, QPointF(100, 200)));

    NGridNode* start = _grid->getNearestNode(QPointF(0,200));
    NGridNode* destination = _grid->getNearestNode(QPointF(0,3000));

     _map->moveRobot(start->getPosition());

    //init pather
    p->init(_map, _grid, start);

    //replan
    p->prepareReplan(destination);
    Tools::Trajectory result;
    p->replanTrajectory(result);

    //check result
    QCOMPARE(result.count(), 0);
    displayTrajectory(result);
}

void PatherTests::forceStartFromBlockedPosition_data()
{
    preparePatherData();
}

void PatherTests::forceStartFromBlockedPosition()
{
    //init test
    QFETCH(int, PATHER);
    Pather* p = _pathers.value(PATHER);

    //init environment
    _grid->clear();
    _grid->makeStandardGrid(QPointF(0,0), 200, 200,  QSizeF(2000,3000), NGrid::HeightConnections);
    resetMap();
    _map->addObstacle(new CircleObject(200, QPointF(100, 200)));

    NGridNode* start = _grid->getNearestNode(QPointF(0,200));
    NGridNode* destination = _grid->getNearestNode(QPointF(0,3000));

     _map->moveRobot(start->getPosition());

    //init pather
    p->init(_map, _grid, start);

    //replan
    p->prepareReplan(destination, QList<QRectF>(), true);
    Tools::Trajectory result;
    p->replanTrajectory(result);

    //check result
    QVERIFY(result.count() >= 3000/200);
    QCOMPARE(result.first().toPoint(), RPoint(0, 200).toPoint());
    QCOMPARE(result.last().toPoint(), RPoint(0, 3000).toPoint());

#ifndef RUN_BENCHMARK
    int turns = analyseTrajectory(result);
    QVERIFY(turns <= 1);
    displayTrajectory(result);
#endif
}
