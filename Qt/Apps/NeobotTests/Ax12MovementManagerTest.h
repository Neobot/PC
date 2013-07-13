#ifndef AX12MOVEMENTMANAGERTEST_H
#define AX12MOVEMENTMANAGERTEST_H

#include <QObject>
#include <QtTest/QtTest>

class Ax12MovementManagerTest : public QObject
{
	Q_OBJECT
public:
	explicit Ax12MovementManagerTest(QObject *parent = 0);

private slots:
	void testGroups();
	void testMovements();

	void testWrite();
	void testRead();
};

#endif // AX12MOVEMENTMANAGERTEST_H
