#ifndef AREAINSPECTORWIDGET_H
#define AREAINSPECTORWIDGET_H

#include <QWidget>

class AreaItem;

namespace Ui {
    class AreaInspectorWidget;
}

class AreaInspector : public QWidget
{
    Q_OBJECT
    
public:
    explicit AreaInspector(QWidget *parent = 0);
    ~AreaInspector();

    void clearInfo();
    void showInfo(AreaItem* area);

    void update();
    
private:
    Ui::AreaInspectorWidget *ui;
    AreaItem* _displayedArea;

    void setReadOnly(bool value);

private slots:
    void nameChanged();
    void positionChanged();

signals:
    void areaChanged(AreaItem* item, const QPointF& oldPoint, const QSizeF& oldSize, const QPointF& newPoint, const QSizeF& newSize);
    void nameChanged(AreaItem* item, const QString& newName);
};

#endif // AREAINSPECTORWIDGET_H
