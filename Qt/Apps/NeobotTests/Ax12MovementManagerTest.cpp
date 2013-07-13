#include "Ax12MovementManagerTest.h"
#include "Ax12MovementManager.h"

using namespace Tools;


Ax12MovementManagerTest::Ax12MovementManagerTest(QObject *parent) :
	QObject(parent)
{
}

void Ax12MovementManagerTest::testGroups()
{
	Ax12MovementManager manager;

	QList<int> g1;
	g1 << 1 << 2 << 3 << 4;
	manager.setGroup("g1", g1);
	QList<int> g1bis = manager.getGroupIds("g1");
	QCOMPARE(g1.count(), g1bis.count());
	for(int i = 0; i < g1bis.count(); ++i)
		QCOMPARE(g1.value(i), g1bis.value(i));

	QList<int> g2;
	g2 << 42 << 24 << 12 << 253 << 223 << 444;
	manager.setGroup("g2", g2);
	QList<int> g2bis = manager.getGroupIds("g2");
	QCOMPARE(g2.count(), g2bis.count());
	for(int i = 0; i < g2bis.count(); ++i)
		QCOMPARE(g2.value(i), g2bis.value(i));

	QList<int> g3;
	manager.setGroup("g3", g3);
	QList<int> g3bis = manager.getGroupIds("g3");
	QCOMPARE(g3.count(), g3bis.count());

	manager.removeGroup("g2");
	QList<int> g2ter = manager.getGroupIds("g2");
	QCOMPARE(g2ter.count(), 0);
}

void Ax12MovementManagerTest::testMovements()
{
	Ax12MovementManager manager;

	QList<int> g1;
	g1 << 1 << 2 << 3 << 4;
	manager.setGroup("g1", g1);

	Ax12MovementManager::Ax12SingleGroupPosition m1p1;
	m1p1.positions << Ax12MovementManager::Ax12Position(30);
	m1p1.positions << Ax12MovementManager::Ax12Position(30);
	m1p1.positions << Ax12MovementManager::Ax12Position(30);
	m1p1.positions << Ax12MovementManager::Ax12Position(30);
	m1p1.info.torque = 72;
	m1p1.info.maxSpeed = 42.3;
	manager.addPositionToMovement("g1", "m1", m1p1);


	Ax12MovementManager::Ax12SingleGroupPosition m1p2;
	m1p2.positions << Ax12MovementManager::Ax12Position(60);
	m1p2.positions << Ax12MovementManager::Ax12Position(60);
	m1p2.positions << Ax12MovementManager::Ax12Position(60);
	m1p2.positions << Ax12MovementManager::Ax12Position(60);
	manager.addPositionToMovement("g1", "m1", m1p2);

	Ax12MovementManager::Ax12SingleMovement m1 = manager.getMovement("g1", "m1");
	QCOMPARE(m1.count(), 2);
	QCOMPARE(m1.value(0).positions.count(), 4);
	QCOMPARE(m1.value(0).info.torque, 72.f);
	QCOMPARE(m1.value(0).info.maxSpeed, 42.3f);
	QCOMPARE(m1.value(1).positions.count(), 4);
	QCOMPARE(m1.value(0).positions.value(0), 30.f);
	QCOMPARE(m1.value(1).positions.value(3), 60.f);

	QList<QPair<QMap<int, Ax12MovementManager::Ax12Position>, Ax12MovementManager::MovementInfo> > dm1 = manager.getDetailedMovement("g1", "m1");
	QCOMPARE(dm1.count(), 2);
	QCOMPARE(dm1.value(0).first.count(), 4);
	QCOMPARE(dm1.value(1).first.count(), 4);
	QCOMPARE(dm1.value(0).second.torque, 72.f);
	QCOMPARE(dm1.value(1).second.torque, 50.f);
	QCOMPARE(dm1.value(0).second.maxSpeed, 42.3f);
	QCOMPARE(dm1.value(1).second.maxSpeed, 60.f);
	QCOMPARE((dm1.value(0).first.begin()).key(), 1);
	QCOMPARE((dm1.value(0).first.begin()+1).key(), 2);
	QCOMPARE((dm1.value(0).first.begin()+2).key(), 3);
	QCOMPARE((dm1.value(0).first.begin()+3).key(), 4);
	QCOMPARE((dm1.value(1).first.begin()).key(), 1);
	QCOMPARE((dm1.value(1).first.begin()+1).key(), 2);
	QCOMPARE((dm1.value(1).first.begin()+2).key(), 3);
	QCOMPARE((dm1.value(1).first.begin()+3).key(), 4);
	QCOMPARE(*dm1.value(0).first.begin(), 30.f);
	QCOMPARE(*(dm1.value(1).first.begin()+3), 60.f);

	manager.addPositionToMovement("g1", "m2", m1p2);
	QCOMPARE(manager.getMovement("g1", "m2").count(), 1);
	manager.addPositionToMovement("g1", "m3", Ax12MovementManager::Ax12SingleGroupPosition());
	QCOMPARE(manager.getMovement("g1", "m3").count(), 1);

	manager.removeMovement("g1", "m1");
	Ax12MovementManager::Ax12SingleMovement m1Bis = manager.getMovement("g1", "m1");
	QCOMPARE(m1Bis.count(), 0);

	manager.removeMovements("g1");
	QCOMPARE(manager.getMovement("g1", "m2").count(), 0);
	QCOMPARE(manager.getMovement("g1", "m3").count(), 0);
}

void Ax12MovementManagerTest::testWrite()
{
	Ax12MovementManager manager;

	QList<int> g1;
	g1 << 1 << 2 << 3 << 4;
	manager.setGroup("g1", g1);
	QList<int> g2;
	g2 << 42 << 253 << 223 << 444;
	manager.setGroup("g2", g2);

	Ax12MovementManager::Ax12SingleGroupPosition m1p1;
	m1p1.positions << Ax12MovementManager::Ax12Position(30.3);
	m1p1.positions << Ax12MovementManager::Ax12Position(30);
	m1p1.positions << Ax12MovementManager::Ax12Position(30);
	m1p1.positions << Ax12MovementManager::Ax12Position(30);
	m1p1.info.torque = 72;
	m1p1.info.maxSpeed = 42.3;

	Ax12MovementManager::Ax12SingleGroupPosition m1p2;
	m1p2.positions << Ax12MovementManager::Ax12Position(60);
	m1p2.positions << Ax12MovementManager::Ax12Position(60);
	m1p2.positions << Ax12MovementManager::Ax12Position(60);
	m1p2.positions << Ax12MovementManager::Ax12Position(60);

	Ax12MovementManager::Ax12SingleGroupPosition m1p3;
	m1p3.positions << Ax12MovementManager::Ax12Position(60.42);
	m1p3.positions << Ax12MovementManager::Ax12Position(60);
	m1p3.positions << Ax12MovementManager::Ax12Position(60);
	m1p3.positions << Ax12MovementManager::Ax12Position(60);

	Ax12MovementManager::Ax12SingleGroupPosition m1p4;
	m1p4.positions << Ax12MovementManager::Ax12Position(32);
	m1p4.positions << Ax12MovementManager::Ax12Position(42);
	m1p4.positions << Ax12MovementManager::Ax12Position(52);
	m1p4.positions << Ax12MovementManager::Ax12Position(62);

	manager.addPositionToMovement("g1", "m1", m1p1);
	manager.addPositionToMovement("g1", "m1", m1p2);
	manager.addPositionToMovement("g1", "m1", m1p3);

	manager.addPositionToMovement("g2", "m1", m1p3);

	manager.addPositionToMovement("g2", "m2", m1p4);
	manager.addPositionToMovement("g2", "m2", m1p4);
	manager.addPositionToMovement("g2", "m2", m1p4);
	manager.addPositionToMovement("g2", "m2", m1p4);
	manager.addPositionToMovement("g2", "m2", m1p4);

	QFile file("test.nax12");
	file.remove();

	QVERIFY(manager.writeToFile("test.nax12"));

	QVERIFY(file.exists());
	QVERIFY(file.size() > 200); //Probably always greater than 0,2ko even if the format change
}

void Ax12MovementManagerTest::testRead()
{
	Ax12MovementManager manager;
	manager.readFromFile("test.nax12");

	Ax12MovementManager::Ax12SingleMovement g1m1 = manager.getMovement("g1", "m1");
	QCOMPARE(g1m1.count(), 3);
	QCOMPARE(g1m1.value(0).info.torque, 72.f);
	QCOMPARE(g1m1.value(0).info.maxSpeed, 42.3f);
	QCOMPARE(g1m1.value(0).positions.count(), 4);
	QCOMPARE(g1m1.value(1).positions.count(), 4);
	QCOMPARE(g1m1.value(2).positions.count(), 4);
	QCOMPARE(g1m1.value(0).positions.value(0), 30.3f);
	QCOMPARE(g1m1.value(2).positions.value(3), 60.f);

	Ax12MovementManager::Ax12SingleMovement g2m1 = manager.getMovement("g2", "m1");
	QCOMPARE(g2m1.count(), 1);
	QCOMPARE(g2m1.value(0).positions.count(), 4);

	Ax12MovementManager::Ax12SingleMovement g2m2 = manager.getMovement("g2", "m2");
	QCOMPARE(g2m2.count(), 5);
	QCOMPARE(g2m2.value(0).positions.count(), 4);
	QCOMPARE(g2m2.value(4).positions.count(), 4);
	QCOMPARE(g2m2.value(0).positions.value(0), 32.f);
	QCOMPARE(g2m2.value(2).positions.value(3), 62.f);
}
