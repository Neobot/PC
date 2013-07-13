#ifndef PATHERTESTS_H
#define PATHERTESTS_H

#include <QtTest/QtTest>
#include <QObject>
#include <Pather.h>
#include <AbstractLogger.h>
#include <NGrid.h>

class PatherTests : public QObject
{
    Q_OBJECT

public:
    PatherTests(QObject *parent = 0);
    ~PatherTests();

private:
    QList<Pather*> _pathers;
    Tools::AbstractLogger* _logger;
    StrategyMap* _map;
    Tools::NGrid* _grid;

    int analyseTrajectory(Tools::Trajectory& t); //Smooth the trajectory and compte the number of turn
    void displayTrajectory(const Tools::Trajectory& t);
    void resetMap();
    void preparePatherData();

    void initRobotPosition(Pather* p, Tools::NGridNode* node);

private slots:
    void emptyTablePlan_data();
    void emptyTablePlan();

    void obstaclePlan_data();
    void obstaclePlan();

    void obstaclePlan2_data();
    void obstaclePlan2();

    void multiPlan_data();
    void multiPlan();

    void unreachableDestination_data();
    void unreachableDestination();
    void forceUnreachableDestination_data();
    void forceUnreachableDestination();

    void blockedWay_data();
    void blockedWay();
    void forceBlockedWay_data();
    void forceBlockedWay();

    void startFromBlockedPosition_data();
    void startFromBlockedPosition();
    void forceStartFromBlockedPosition_data();
    void forceStartFromBlockedPosition();
};

#endif // PATHERTESTS_H
