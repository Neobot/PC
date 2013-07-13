#ifndef CURVETESTS_H
#define CURVETESTS_H

#include <QObject>

//#define DEBUG_MESSAGE;

class CurveTests : public QObject
{
    Q_OBJECT
public:
    CurveTests(QObject *parent);

private slots:
    void testConstruction();
    void testBezier();
    void testBezier_data();
    void testInterpolate();
    void testMinMax();
    void testSortPointsOnAbscissa();
    void testYValue();
    void testYValue2();
    void testXValue();
    void testToTrajectory();
    void testTrajectoryStraightLineSmoothing();
    void testTrajectoryDiagonalSmoothing();
    void testTrajectoryStopAtDistance();

};

#endif // CURVETESTS_H
