#ifndef ZRANGETESTS_H
#define ZRANGETESTS_H

#include <QObject>
#include <QtTest/QtTest>


class ZRangeTests : public QObject
{
    Q_OBJECT
public:
    ZRangeTests(QObject *parent = 0);

private slots:
    void heightTest();
    void isInRangeTest();
    void equalTest();
    void intersectionTest();
    void intersectedTest();

};

#endif // ZRANGETESTS_H
