#include "GraphicStrategyTest.h"
#include <QTest>
#include <QtDebug>

using namespace Tools;

//#define SHOW_INFO

GraphicStrategyTests::GraphicStrategyTests(QObject *parent) :
    QObject(parent)
{
//#ifdef SHOW_INFO
//    _logger = new StdoutLogger;
//#else
//    _logger = 0;
//#endif
//    _buffer.open(QIODevice::ReadWrite);

//    _protocol = new Comm::Protocol(&_buffer, _logger, true);
//    _protocol->beQuiet(true);
//    _robot = new Comm::Robot(_protocol, _logger);
//    _robot->beQuiet(true);

//    //_dummyStrategy = new DstarStrategyDummy(0);
//    //_strategy = new DstarPather(_robot, _dummyStrategy, _logger);

//#ifndef SHOW_INFO
//    //_strategy->beQuiet(true);
//#endif
}

void GraphicStrategyTests::cleanupTestCase()
{
    //delete _strategy;
    //delete _dummyStrategy;
//    delete _robot;
//    delete _protocol;
//    delete _logger;
}

void GraphicStrategyTests::firstPlan()
{
    //_strategy->init();

//    QCOMPARE(_strategy->_globalStrategy.count(), 5 - 1); //The first trajectory is already calculated
//    QCOMPARE(_strategy->_initDone, true);
//    QCOMPARE(_strategy->_map->checkRobotPositionAt(Tools::RPoint(1500, 800)), true);
//    QCOMPARE(_strategy->_map->checkRobotPositionAt(Tools::RPoint(600, 1000)), false);
//    QCOMPARE(_strategy->_map->checkRobotPositionAt(_dummyStrategy->getStartPoint()), true);

//    QVERIFY(_strategy->_currentTrajectory.count() > 0);
//    for (int i = 0; i < _strategy->_currentTrajectory.count(); ++i)
//    {
//        QVERIFY(_strategy->_currentTrajectory.value(i).getX() != 0);
//        QVERIFY(_strategy->_currentTrajectory.value(i).getY() != 0);
//#ifdef SHOW_INFO
//        qDebug() << _strategy->_currentTrajectory.value(i).getX() << " " << _strategy->_currentTrajectory.value(i).getY();
//        qDebug() << (_strategy->_currentTrajectory.value(i).getX() - 150.0)/225.0 << " " << _strategy->_currentTrajectory.value(i).getY()/250;
//#endif
//        QCOMPARE(_strategy->_map->checkRobotPositionAt(_strategy->_currentTrajectory.value(i)), true);
//    }
}

void GraphicStrategyTests::staticTest()
{
//    int startPointCount = _strategy->_currentTrajectory.count();
//
//    _robot->getListener()->go();
//    QCOMPARE(_strategy->_currentTrajectory.count(), startPointCount - 1);
//
//    Trajectory tr = _strategy->_currentTrajectory;
//    Trajectory gl = _strategy->_globalStrategy;
//
//    for (int j = 0; j < gl.count() + 1; ++j)
//    {
//        RPoint g = _strategy->_currentGlobalPoint;
//        for (int i = 0; i < tr.count() + 1; ++i)
//        {
//            RPoint p = _strategy->_currentTrajPoint;
//#ifdef SHOW_INFO
//            qDebug() << p.getX() << " " << p.getY();
//#endif
//            _robot->getListener()->coordinates(p.getX(), p.getY(), p.getTheta(), true);
//            if (i < tr.count())
//            {
//                QCOMPARE(_strategy->_currentTrajectory.count(), tr.count() - i - 1);
//                if (!_strategy->_currentTrajectory.isEmpty())
//                    QCOMPARE(_strategy->_currentTrajectory.first(), tr.value(i + 1));
//            }
//            else
//            {
//#ifdef SHOW_INFO
//                if (_strategy->_currentTrajectory.count() > 0)
//                    qDebug() << "New trajectory of " << _strategy->_currentTrajectory.count() + 1 << " Points";
//#endif
//            }
//
//            QCOMPARE(_strategy->_map->getRobotPosition(), p);
//
//            if (_dummyStrategy->_type == 1 && i == tr.count())
//                QCOMPARE(_strategy->_map->checkRobotPositionAt(p), false);
//            else
//                QCOMPARE(_strategy->_map->checkRobotPositionAt(p), true);
//        }
//        tr = _strategy->_currentTrajectory;
//
//        if (j < gl.count())
//            QCOMPARE(_strategy->_globalStrategy.count(), gl.count() - j - 1);
//
//        QCOMPARE(_strategy->_map->getRobotPosition(), g);
//
//    }
//    QCOMPARE(_strategy->_globalStrategy.count(), 0);
//    QCOMPARE(_strategy->_currentTrajectory.count(), 0);

}


void GraphicStrategyTests::unreachableTest()
{
//    delete _strategy;
//    delete _dummyStrategy;
//    _dummyStrategy = new DstarStrategyDummy(1);
//    _strategy = new DstarPather(_robot, _dummyStrategy, _logger);
//    _strategy->init();
//    staticTest();
}
