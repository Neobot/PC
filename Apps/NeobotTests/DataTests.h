#ifndef DATATESTS_H
#define DATATESTS_H

#include <QObject>

class DataTests : public QObject
{
    Q_OBJECT
public:
    DataTests(QObject *parent = 0);

private slots:
    void LittleEndianTest();
    void BigEndianTest();
    void SimpleBooleanTest();
    void ComplexBooleanTest();
    void intToBitArrayTest();
    void bitArrayToIntTest();
	void floatTest();
};

#endif // DATATESTS_H
