#ifndef SHARPOBJECT_H
#define SHARPOBJECT_H

#include "DataObject.h"
#include "Curve.h"
#include <QVector2D>

class SharpObject : public Tools::LineObject
{
public:
    SharpObject(double range, const QPointF& relativePosition, double relativeRotation, const ZRange& zrange, const Tools::Curve& calibrationCurve = Tools::Curve());
    SharpObject(const SharpObject& other);

    double getValue();
    void setPositionAndRotation(const Tools::RPoint &robotPosition);
    void setDistance(const double distance);

    double getRange() const;
    double getDistance() const;

protected:
   Tools::Curve _calibrationCurve; //Curve: Volt on Y axis and Distance on X axis
   const QVector2D _relativePosition;
   const double _relativeRotation;
   double _distance;
   double _range;
};

class Sharp4_30 : public SharpObject
{
public:
    Sharp4_30(const QPointF& relativePosition, double relativeRotation, double height);
    Sharp4_30(const Sharp4_30& other);
};

class Sharp10_80 : public SharpObject
{
public:
    Sharp10_80(const QPointF& relativePosition, double relativeRotation, double height);
    Sharp10_80(const Sharp10_80& other);
};

class Sharp20_150 : public SharpObject
{
public:
    Sharp20_150(const QPointF& relativePosition, double relativeRotation, double height);
    Sharp20_150(const Sharp20_150& other);
};

class MaxSonar_MB1240 : public SharpObject
{
public:
        MaxSonar_MB1240(const QPointF& relativePosition, double relativeRotation, double height);
	MaxSonar_MB1240(const MaxSonar_MB1240& other);
};

#endif // SHARPOBJECT_H
