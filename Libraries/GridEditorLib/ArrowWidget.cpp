#include "ArrowWidget.h"
#include <QPainter>

ArrowWidget::ArrowWidget(QWidget *parent) :
    QWidget(parent), _direction(LeftToRight)
{
}

void ArrowWidget::paintEvent(QPaintEvent *)
{


    QPainter painter(this);

    QPen pen;
    pen.setWidth(2);
    if (isEnabled())
    {
        painter.setBrush(Qt::black);
        pen.setColor(Qt::black);
    }
    else
    {
         painter.setBrush(Qt::lightGray);
        pen.setColor(Qt::lightGray);
    }
    painter.setPen(pen);

    QPointF left(0, height()/2);
    QPointF right( width(), height()/2);

    painter.drawLine(left, right);

    int arrowX = 10;
    int arrowY = 3;

    QPointF topPoint(arrowX, arrowY);
    QPointF downPoint(arrowX, -arrowY);

    QPolygonF arrow;
    if (_direction == LeftToRight)
        arrow << right << right - topPoint << right - downPoint;
    else if (_direction == RightToLeft)
        arrow << left << left + topPoint << left + downPoint;

    painter.drawPolygon(arrow);
}

ArrowWidget::Direction ArrowWidget::getDirection() const
{
    return _direction;
}

void ArrowWidget::setDirection(ArrowWidget::Direction direction)
{
    _direction = direction;
}
