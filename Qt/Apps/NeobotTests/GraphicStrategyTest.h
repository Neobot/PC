#ifndef GRAPHICSTRATEGYTESTS_H
#define GRAPHICSTRATEGYTESTS_H

#include <QObject>
#include <QBuffer>
#include "AbstractLogger.h"
#include "RobotProtocol.h"
#include "RobotCommInterface.h"

class TableMap;

class GraphicStrategyTests : public QObject
{
Q_OBJECT

public:
    GraphicStrategyTests(QObject *parent = 0);

private:
    Tools::StdoutLogger*    _logger;
    QBuffer                 _buffer;
    Comm::RobotProtocol*         _protocol;
    Comm::RobotCommInterface*            _robot;

private slots:
    void cleanupTestCase();
    void firstPlan();
    void staticTest();
    void unreachableTest();

};

#endif // GRAPHICSTRATEGYTESTS_H
