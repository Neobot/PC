#ifndef ARROWWIDGET_H
#define ARROWWIDGET_H

#include <QWidget>

class ArrowWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(Direction direction READ getDirection WRITE setDirection)
public:
    enum Direction
    {
        LeftToRight,
        RightToLeft,
        None
    };

    explicit ArrowWidget(QWidget *parent = 0);

    Direction getDirection() const;
    void setDirection(Direction direction);

protected:
    void paintEvent(QPaintEvent *);

private:
    Direction _direction;

};

#endif // ARROWWIDGET_H
