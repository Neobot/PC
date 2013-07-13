#include "ContactorObject.h"
#include <math.h>

using namespace Tools;

ContactorObject::ContactorObject(const QPointF& relativePosition, double relativeRotation, double height)
    : RectangleObject(QRectF(relativePosition + QPointF(-4, 8), QSizeF(8, 16)), ZRange(height - 3, height + 3)),
      _relativePosition(relativePosition + QPointF(-4, 8)),
      _relativeRotation(relativeRotation),
      _status(false)
{
    setRotation(relativeRotation);
}

bool ContactorObject::getStatus()
{
    return _status;
}

void ContactorObject::setStatus(const bool status)
{
    _status = status;
}

void ContactorObject::setPositionAndRotation(const RPoint &robotPosition)
{
    double x, y, d, theta;
    d = _relativePosition.length();

    theta = robotPosition.getTheta() + atan2(_relativePosition.y(), _relativePosition.x());
    x = robotPosition.getX() + d*cos(theta);
    y = robotPosition.getY() + d*sin(theta);

    RectangleObject::setPosition(QPointF(x, y));
    RectangleObject::setRotation(_relativeRotation + robotPosition.getTheta());
}
