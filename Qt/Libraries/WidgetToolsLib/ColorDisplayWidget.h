#ifndef COLORDISPLAYWIDGET_H
#define COLORDISPLAYWIDGET_H

#include <QWidget>
#include <QHBoxLayout>

namespace Tools
{
    class ColorDisplayWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit ColorDisplayWidget(QWidget *parent = 0);

        void clear();
        void setColors(const QList<QColor>& colors);

    private:
        QHBoxLayout* _layout;
        QList<QWidget*> _colorWidgets;
        QList<QSpacerItem*> _spacers;

        void addColor(const QColor& color);
        void removeLastColor();
        void addSpacer();
        void setWidgetColor(QWidget* w, const QColor& color) const;

    };
}

#endif // COLORDISPLAYWIDGET_H
