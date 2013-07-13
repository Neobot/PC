#ifndef CONTACTOROBJECT_H
#define CONTACTOROBJECT_H

#include "DataObject.h"
#include <QVector2D>

class ContactorObject : public Tools::RectangleObject
{
public:
    ContactorObject(const QPointF& relativePosition, double relativeRotation, double height);
    ContactorObject(const ContactorObject& other);

    bool getStatus();
    void setStatus(const bool status);
    void setPositionAndRotation(const Tools::RPoint &robotPosition);

protected:
   const QVector2D _relativePosition;
   const double _relativeRotation;
   bool _status;
};

#endif // CONTACTOROBJECT_H
