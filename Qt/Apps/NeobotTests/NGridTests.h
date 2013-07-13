#ifndef NGRIDTESTS_H
#define NGRIDTESTS_H

#include <QObject>
#include <QtTest/QtTest>

class NGridTests : public QObject
{
     Q_OBJECT
public:
    NGridTests(QObject *parent);

private slots:
    void testNodeConnections();
    void testRemoveAllNodeConnections();
    void testRemoveNode();

    void testAlias();
    void testNearestNode();

    void testStandardGridHeightConnections();
    void testStandardGridFourConnections();
    void testStandardGridNoConnections();

    void testAreas();

    void testWrite();
    void testRead();
};

#endif // NGRIDTESTS_H
