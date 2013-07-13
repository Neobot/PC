#ifndef SHARPFACTORY_H
#define SHARPFACTORY_H

#include "Sharp.h"

class SharpFactory
{
public:
    static Sharp* TestSharp(const QVector2D& position, const QVector2D& direction, double height);
    static Sharp* Sharp20_150(double t1, const QVector2D& position, const QVector2D& direction, double height);
    static Sharp* Sharp10_80(double t1, const QVector2D &position, const QVector2D& direction, double height);
    static Sharp* Sharp4_30(double t1, const QVector2D &position, const QVector2D& direction, double height);
    static Sharp* MaxSonar_MB1240(double t1, const QVector2D &position, const QVector2D& direction, double height);
};

#endif // SHARPFACTORY_H
