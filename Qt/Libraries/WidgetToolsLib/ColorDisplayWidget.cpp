#include "ColorDisplayWidget.h"
#include <QLabel>

using namespace Tools;

ColorDisplayWidget::ColorDisplayWidget(QWidget *parent) :
    QWidget(parent), _layout(0)
{
    _layout = new QHBoxLayout(this);
    _layout->setContentsMargins(0,0,0,0);

    setWidgetColor(this, Qt::white);
}

void ColorDisplayWidget::clear()
{
    foreach(QWidget* cw, _colorWidgets)
    {
        _layout->removeWidget(cw);
        delete cw;
    }

    _colorWidgets.clear();

    foreach(QSpacerItem* s, _spacers)
    {
        _layout->removeItem(s);
        delete s;
    }

    _spacers.clear();
}

void ColorDisplayWidget::setColors(const QList<QColor> &colors)
{
    if (_colorWidgets.count() <= colors.count())
    {
        for(int i = 0; i < _colorWidgets.count(); ++i)
        {
            setWidgetColor(_colorWidgets[i], colors[i]);
        }

        for(int i = _colorWidgets.count(); i < colors.count(); ++i)
        {
            addColor(colors[i]);
        }
    }
    else
    {
        for(int i = 0; i < colors.count(); ++i)
        {
            setWidgetColor(_colorWidgets[i], colors[i]);
        }

        int nbWidgetToRemove = _colorWidgets.count() - colors.count();
        for(int i = 0; i < nbWidgetToRemove; ++i)
        {
            removeLastColor();
        }
    }
}

void ColorDisplayWidget::addColor(const QColor &color)
{
    if (_colorWidgets.isEmpty())
    {
        addSpacer();
    }

    QLabel* widget = new QLabel("TEST", this);
    setWidgetColor(widget, color);
    _colorWidgets << widget;
    _layout->addWidget(widget);

    addSpacer();
}

void ColorDisplayWidget::removeLastColor()
{
    if (!_spacers.isEmpty())
    {
        QSpacerItem* s = _spacers.takeLast();
        _layout->removeItem(s);
        delete s;
    }

    if (!_colorWidgets.isEmpty())
    {
        QWidget* w = _colorWidgets.takeLast();
        _layout->removeWidget(w);
        delete w;
    }

    if (_spacers.count() == 1)
    {
        //Removes the last spacer
        QSpacerItem* s = _spacers.takeLast();
        _layout->removeItem(s);
        delete s;
    }
}

void ColorDisplayWidget::addSpacer()
{
    QSpacerItem* spacer = new QSpacerItem(0,0, QSizePolicy::Expanding);
    _layout->addSpacerItem(spacer);

    _spacers << spacer;
}

void ColorDisplayWidget::setWidgetColor(QWidget *w, const QColor &color) const
{
    w->setStyleSheet(QString("background-color:%1; border:4px solid black").arg(color.name()));
}

