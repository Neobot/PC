#include "NGridTests.h"
#include "NGrid.h"
#include "NGridArea.h"

using namespace Tools;

NGridTests::NGridTests(QObject *parent) : QObject(parent)
{
}

void NGridTests::testNodeConnections()
{
    NGrid grid;
    NGridNode* n1 = grid.addNode(QPointF(0, 0));
    NGridNode* n2 = grid.addNode(QPointF(0, 1));
    NGridNode* n3 = grid.addNode(QPointF(1, 1));
    NGridNode* n4 = grid.addNode(QPointF(1, 0));

    QCOMPARE(grid.getNodes().count(), 4);

    n3->addReachableNode(n4);
    QCOMPARE(n3->getReachableNodes().count(), 1);
    QCOMPARE(n3->getRevertedConnections().count(), 0);
    QCOMPARE(n4->getReachableNodes().count(), 0);
    QCOMPARE(n4->getRevertedConnections().count(), 1);

    grid.addBidirectionalLink(n1, n2);
    QCOMPARE(n1->getReachableNodes().count(), 1);
    QCOMPARE(n1->getRevertedConnections().count(), 1);
    QCOMPARE(n2->getReachableNodes().count(), 1);
    QCOMPARE(n2->getRevertedConnections().count(), 1);

    QCOMPARE(n1->getRevertedConnections()[0], n2);
    QCOMPARE(n1->getReachableNodes().begin().key(), n2);

    n1->addReachableNode(n3);
    n1->addReachableNode(n4);
    grid.addBidirectionalLink(n2, n4);

    QCOMPARE(n4->getReachableNodes().count(), 1);
    QCOMPARE(n4->getRevertedConnections().count(), 3);
    QCOMPARE(n2->getReachableNodes().count(), 2);
    QCOMPARE(n2->getRevertedConnections().count(), 2);

    QVERIFY(n4->getReachableNodes().contains(n2));
    QVERIFY(n4->getRevertedConnections().contains(n2));
    QVERIFY(n2->getReachableNodes().contains(n4));
    QVERIFY(n2->getRevertedConnections().contains(n4));

    grid.removeBidirectionalLink(n4, n2);

    QVERIFY(!n4->getReachableNodes().contains(n2));
    QVERIFY(!n4->getRevertedConnections().contains(n2));
    QVERIFY(!n2->getReachableNodes().contains(n4));
    QVERIFY(!n2->getRevertedConnections().contains(n4));

    QCOMPARE(n4->getReachableNodes().count(), 0);
    QCOMPARE(n4->getRevertedConnections().count(), 2);
    QCOMPARE(n2->getReachableNodes().count(), 1);
    QCOMPARE(n2->getRevertedConnections().count(), 1);
}

void NGridTests::testRemoveAllNodeConnections()
{
    NGrid grid;
    NGridNode* n1 = grid.addNode(QPointF(0, 0));
    NGridNode* n2 = grid.addNode(QPointF(0, 1));
    NGridNode* n3 = grid.addNode(QPointF(1, 1));
    NGridNode* n4 = grid.addNode(QPointF(1, 0));

    n3->addReachableNode(n4);
    grid.addBidirectionalLink(n1, n2);
    n1->addReachableNode(n3);
    n1->addReachableNode(n4);
    grid.addBidirectionalLink(n2, n4);

    n1->removeAllConnections();

    QCOMPARE(grid.getNodes().count(), 4);

    QCOMPARE(n2->getReachableNodes().count(), 1);
    QCOMPARE(n2->getRevertedConnections().count(), 1);

    QCOMPARE(n3->getReachableNodes().count(), 1);
    QCOMPARE(n3->getRevertedConnections().count(), 0);

    QCOMPARE(n4->getReachableNodes().count(), 1);
    QCOMPARE(n4->getRevertedConnections().count(), 2);
}

void NGridTests::testRemoveNode()
{
    NGrid grid;
    NGridNode* n1 = grid.addNode(QPointF(0, 0));
    NGridNode* n2 = grid.addNode(QPointF(0, 1));
    NGridNode* n3 = grid.addNode(QPointF(1, 1));
    NGridNode* n4 = grid.addNode(QPointF(1, 0));

    n3->addReachableNode(n4);
    grid.addBidirectionalLink(n1, n2);
    n1->addReachableNode(n3);
    n1->addReachableNode(n4);
    grid.addBidirectionalLink(n2, n4);

    grid.removeNode(n1);
    delete n1;

    QCOMPARE(grid.getNodes().count(), 3);

    QCOMPARE(n2->getReachableNodes().count(), 1);
    QCOMPARE(n2->getRevertedConnections().count(), 1);

    QCOMPARE(n3->getReachableNodes().count(), 1);
    QCOMPARE(n3->getRevertedConnections().count(), 0);

    QCOMPARE(n4->getReachableNodes().count(), 1);
    QCOMPARE(n4->getRevertedConnections().count(), 2);
}

void NGridTests::testAlias()
{
    NGrid grid;
    NGridNode* n1 = grid.addNode(QPointF(0, 0));
    NGridNode* n2 = grid.addNode(QPointF(0, 1));

    grid.addAlias(n1, "node1");
    grid.addAlias(n2, "node2");

    QVERIFY(grid.getNode("node1") == n1);
    QVERIFY(grid.getNode("node2") == n2);
    QVERIFY(grid.getNode("unknownNode") == 0);

    grid.removeNode(n1);
    delete n1;

    QVERIFY(grid.getNode("node1") == 0);
}

void NGridTests::testNearestNode()
{
    NGrid grid;
    NGridNode* n1 = grid.addNode(QPointF(100, 100));
    NGridNode* n2 = grid.addNode(QPointF(50, 100));
    NGridNode* n3 = grid.addNode(QPointF(300, 300));
    NGridNode* n4 = grid.addNode(QPointF(400, 50));

    QCOMPARE(grid.getNearestNode(QPointF(150, 100)), n1);
    QCOMPARE(grid.getNearestNode(QPointF(100, 100)), n1);
    QCOMPARE(grid.getNearestNode(QPointF(0, 0)), n2);

    QCOMPARE(grid.getNearestNode(QPointF(300, 100)), n4);

    NGridNode* r4 = grid.getNearestNode(QPointF(200, 200));
    QVERIFY(r4 == n1 || r4 == n3);

    QCOMPARE(grid.getNearestNode(QPointF(15000, 900)), n3);
    QCOMPARE(grid.getNearestNode(QPointF(15000, -500)), n4);

}

void NGridTests::testStandardGridHeightConnections()
{
    NGrid grid;
    grid.makeStandardGrid(QPointF(0,0), 50, 50, QSizeF(3005.5, 2005.2), NGrid::HeightConnections);

    QCOMPARE(grid.getNodes().count(), 61*41);

    NGridNode* middleNode = grid.getNearestNode(QPointF(1500, 1000));
    QCOMPARE(middleNode->getReachableNodes().count(), 8);
    QCOMPARE(middleNode->getRevertedConnections().count(), 8);

    NGridNode* borderNode = grid.getNearestNode(QPointF(1500, 0));
    QCOMPARE(borderNode->getReachableNodes().count(), 5);
    QCOMPARE(borderNode->getRevertedConnections().count(), 5);

    NGridNode* cornerNode = grid.getNearestNode(QPointF(0, 0));
    QCOMPARE(cornerNode->getReachableNodes().count(), 3);
    QCOMPARE(cornerNode->getRevertedConnections().count(), 3);
}

void NGridTests::testStandardGridFourConnections()
{
    NGrid grid;
    grid.makeStandardGrid(QPointF(0,0), 50, 50, QSizeF(3000, 2000), NGrid::FourConnections);

    QCOMPARE(grid.getNodes().count(), 61*41);

    NGridNode* middleNode = grid.getNearestNode(QPointF(1500, 1000));
    QCOMPARE(middleNode->getReachableNodes().count(), 4);
    QCOMPARE(middleNode->getRevertedConnections().count(), 4);

    NGridNode* borderNode = grid.getNearestNode(QPointF(1500, 0));
    QCOMPARE(borderNode->getReachableNodes().count(), 3);
    QCOMPARE(borderNode->getRevertedConnections().count(), 3);

    NGridNode* cornerNode = grid.getNearestNode(QPointF(0, 0));
    QCOMPARE(cornerNode->getReachableNodes().count(), 2);
    QCOMPARE(cornerNode->getRevertedConnections().count(), 2);
}

void NGridTests::testStandardGridNoConnections()
{
    NGrid grid;
    grid.makeStandardGrid(QPointF(0,0), 50, 50, QSizeF(2999.9, 1950.1), NGrid::NoConnection);

    QCOMPARE(grid.getNodes().count(), 60*40);

    NGridNode* middleNode = grid.getNearestNode(QPointF(1500, 1000));
    QCOMPARE(middleNode->getReachableNodes().count(), 0);
    QCOMPARE(middleNode->getRevertedConnections().count(), 0);

    NGridNode* borderNode = grid.getNearestNode(QPointF(1500, 0));
    QCOMPARE(borderNode->getReachableNodes().count(), 0);
    QCOMPARE(borderNode->getRevertedConnections().count(), 0);

    NGridNode* cornerNode = grid.getNearestNode(QPointF(0, 0));
    QCOMPARE(cornerNode->getReachableNodes().count(), 0);
    QCOMPARE(cornerNode->getRevertedConnections().count(), 0);
}


void NGridTests::testAreas()
{
    NGrid grid;
    grid.makeStandardGrid(QPointF(0,0), 50, 50, QSizeF(3000, 2000), NGrid::FourConnections);

    NGridNode* n1 = grid.getNearestNode(QPointF(0, 0));
    NGridNode* n2 = grid.getNearestNode(QPointF(200, 400));
    NGridNode* n3 = grid.getNearestNode(QPointF(300, 500));
    NGridNode* n4 = grid.getNearestNode(QPointF(300, 550));
    NGridNode* n5 = grid.getNearestNode(QPointF(700, 850));

    grid.addArea(QRectF(0, 0, 300, 500), "Area1");
    grid.addArea(QRectF(220, 420, 200, 300), "Area2");
    grid.addArea(QRectF(110, 110, 10, 20), "nullArea");

    QCOMPARE(grid.getAreas().count(), 3);
    QVERIFY(grid.getArea("Area1")->isValid());
    QVERIFY(grid.getArea("Area2")->isValid());
    QVERIFY(grid.getArea("nullArea")->isValid());
    QVERIFY(!grid.getArea("TOTO"));
    QVERIFY(!grid.getArea(QString()));

    QCOMPARE(grid.getArea("Area1")->getRect(), QRectF(0, 0, 300, 500));
    QCOMPARE(grid.getArea("Area1")->getNodes().count(), 7 * 11);
    QVERIFY(grid.getArea("Area1")->getNodes().contains(n1));
    QVERIFY(grid.getArea("Area1")->getNodes().contains(n2));
    QVERIFY(grid.getArea("Area1")->getNodes().contains(n3));
    QVERIFY(!grid.getArea("Area1")->getNodes().contains(n4));
    QVERIFY(!grid.getArea("Area1")->getNodes().contains(n5));

    QCOMPARE(grid.getArea("Area2")->getNodes().count(), 4 * 6);
    QVERIFY(!grid.getArea("Area2")->getNodes().contains(n1));
    QVERIFY(!grid.getArea("Area2")->getNodes().contains(n2));
    QVERIFY(grid.getArea("Area2")->getNodes().contains(n3));

    QCOMPARE(grid.getArea("nullArea")->getNodes().count(), 0);

    QCOMPARE(grid.getArea("Area1")->getNearestNode(QPointF(0, 0)), n1);
    QCOMPARE(grid.getArea("Area1")->getNearestNode(QPointF(200, 400)), n2);
    QCOMPARE(grid.getArea("Area1")->getNearestNode(QPointF(700, 850)), n3);

    grid.getArea("Area1")->setInternalCost(42);
    QCOMPARE(n1->getCostToGo(grid.getNearestNode(QPointF(0, 50))), 42);
    QCOMPARE(n3->getCostToGo(n4), 1); //Go out
    QCOMPARE(n4->getCostToGo(n3), 1); //Go in

    grid.getArea("Area1")->setCostToGoInside(78);
    QCOMPARE(n1->getCostToGo(grid.getNearestNode(QPointF(0, 50))), 42);
    QCOMPARE(n3->getCostToGo(n4), 1); //Go out
    QCOMPARE(n4->getCostToGo(n3), 78); //Go in

    grid.getArea("Area1")->setCostToGoOutside(12);
    QCOMPARE(n1->getCostToGo(grid.getNearestNode(QPointF(0, 50))), 42);
    QCOMPARE(n3->getCostToGo(n4), 12); //Go out
    QCOMPARE(n4->getCostToGo(n3), 78); //Go in

}

void NGridTests::testWrite()
{
    NGrid grid;
    grid.makeStandardGrid(QPointF(0,0), 50, 50, QSizeF(3000, 2000), NGrid::HeightConnections);

    NGridNode* a = grid.addNode(QPointF(5000, 1000));
    a->addReachableNode(grid.getNearestNode(QPointF(0,0)));
    a->addReachableNode(grid.getNearestNode(QPointF(3000,0)));
    a->addReachableNode(grid.getNearestNode(QPointF(0,2000)));
    a->addReachableNode(grid.getNearestNode(QPointF(3000,2000)));

    grid.addAlias(a, "ANode");

    QCOMPARE(grid.getNodes().count(), 61*41 + 1);

    grid.addArea(QRectF(0, 0, 300, 500), "Area1");
    grid.addArea(QRectF(220, 420, 200, 300), "Area2");
    grid.addArea(QRectF(110, 110, 10, 20), "nullArea");

    QFile file("test.ngrid");
    file.remove();

    QVERIFY(grid.writeToFile("test.ngrid"));

    QVERIFY(file.exists());
    QVERIFY(file.size() > 5000); //Probably always greater than 5ko even if the format change
}

void NGridTests::testRead()
{
    NGrid grid;
    QVERIFY(grid.readFromFile("test.ngrid"));

    QCOMPARE(grid.getNodes().count(), 61*41 + 1);

    NGridNode* cornerNode = grid.getNearestNode(QPointF(3000, 2000));
    QCOMPARE(cornerNode->getPosition(), QPointF(3000, 2000));
    QCOMPARE(cornerNode->getReachableNodes().count(), 3);
    QCOMPARE(cornerNode->getRevertedConnections().count(), 3 + 1);


    NGridNode* a = grid.getNearestNode(QPointF(5000, 1000));
    QCOMPARE(a->getReachableNodes().count(), 4);
    QCOMPARE(a->getRevertedConnections().count(), 0);

    QVERIFY(a->getReachableNodes().contains(grid.getNearestNode(QPointF(0,0))));
    QVERIFY(a->getReachableNodes().contains(grid.getNearestNode(QPointF(3000,0))));
    QVERIFY(a->getReachableNodes().contains(grid.getNearestNode(QPointF(0,2000))));
    QVERIFY(a->getReachableNodes().contains(grid.getNearestNode(QPointF(3000,2000))));

    QCOMPARE(grid.getNode("ANode"), a);

    QCOMPARE(grid.getAreas().count(), 3);
    QVERIFY(grid.getArea("Area1")->isValid());
    QCOMPARE(grid.getArea("Area1")->getNodes().count(), 7 * 11);
    QVERIFY(grid.getArea("Area2")->isValid());
    QVERIFY(grid.getArea("nullArea")->isValid());
}
