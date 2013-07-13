#ifndef GRIDYVIEW_H
#define GRIDYVIEW_H

#include <QGraphicsView>
#include <QMainWindow>
#include <QWheelEvent>
#include "SceneTools.h"

class GridScene;

class GridView : public QGraphicsView
{
    Q_OBJECT

    #define ZOOM_STEP 1.2
    #define ZOOM_LOWER_LIMIT 0.05
    #define ZOOM_UPPER_LIMIT 10

public:
    explicit GridView(QWidget *parent = 0);

    void zoomIn();
    void zoomOut();
    void scaleViewBy(qreal scaleFactor);
    void scaleViewTo(qreal absoluteScaleFactor);

    void setMode(SceneTools::Mode mode);

	qreal getScaleFactor() const;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    qreal _zoomScale;
    SceneTools::Mode _mode;

    bool _areaInConstruction;
    bool _areaConstructionAsked;
    QPointF _areaConstructionOrigin;

    void wheelEvent(QWheelEvent *event);
    qreal calculateScaleFactor(const QMatrix& matrix);

signals:
    void cancelMode();
};

#endif // GRIDYVIEW_H
