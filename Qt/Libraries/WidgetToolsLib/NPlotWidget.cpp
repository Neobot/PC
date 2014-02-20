#include "NPlotWidget.h"

using namespace Tools;

NPlotWidget::NPlotWidget(QWidget *parent) :
	QCustomPlot(parent), _autoResize(true)
{
    setBackground(QBrush(Qt::white));

	legend->setVisible(true);
	QFont legendFont = font();  // start out with MainWindow's font..
	legendFont.setPointSize(9); // and make a bit smaller for legend
	legend->setFont(legendFont);
    axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);

   setDefaultInteractionEnabled(true);
}

NPlotWidget::~NPlotWidget()
{
}


void NPlotWidget::setLegendVisible(bool value)
{
	legend->setVisible(value);
}

void NPlotWidget::setAutoResize(bool value)
{
	_autoResize = value;
}

void NPlotWidget::setAxisScale(double xMin, double xMax, double yMin, double yMax)
{
	xAxis->setRange(xMin, xMax);
	yAxis->setRange(yMin, yMax);
}

void NPlotWidget::setAxisValuesVisible(bool x, bool y)
{
	xAxis->setTickLabels(x);
	yAxis->setTickLabels(y);
}

void NPlotWidget::setDefaultInteractionEnabled(bool value)
{
    if (value)
	{
        setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	}
	else
	{
        setInteractions(0);
    }
}

void NPlotWidget::setTitle(const QString &title)
{
    plotLayout()->insertRow(0);
    plotLayout()->addElement(0, 0, new QCPPlotTitle(this, title));
}

//------------


MultiCurveWidget::MultiCurveWidget(QWidget *parent) : NPlotWidget(parent), _dataNumberLimit(-1)
{
}

MultiCurveWidget::~MultiCurveWidget()
{
}

QCPCurve* MultiCurveWidget::addCurve(const QString &name, const QPen &pen)
{
	QCPCurve* c = new QCPCurve(xAxis, yAxis);
	c->setPen(pen);
	c->setName(name);
	addPlottable(c);
	_curves << c;

	return c;
}

void MultiCurveWidget::addValue(int curveIndex, double x, double y)
{
	QCPCurve* c = getCurve(curveIndex);
	if (c)
	{
		c->addData(x, y);
		if (_dataNumberLimit > 0 && c->data()->count() > _dataNumberLimit)
			c->removeData(c->data()->begin().key());

		customRescale();
		replot();
	}
}

void MultiCurveWidget::addValues(double x, const QList<double>& y)
{
	QList<double>::const_iterator itValues = y.begin();
	foreach(QCPCurve* c, _curves)
	{
		if (itValues != y.end())
		{
			c->addData(x, *itValues);
			if (_dataNumberLimit > 0 && c->data()->count() > _dataNumberLimit)
				c->removeData(c->data()->begin().key());
		}

		++itValues;
	}

	customRescale();
	replot();
}

void MultiCurveWidget::addValues(const QList<double> &x, double y)
{
	QList<double>::const_iterator itValues = x.begin();
	foreach(QCPCurve* c, _curves)
	{
		if (itValues != x.end())
		{
			c->addData(*itValues, y);
			if (_dataNumberLimit > 0 && c->data()->count() > _dataNumberLimit)
				c->removeData(c->data()->begin().key());
		}

		++itValues;
	}

	customRescale();
	replot();
}

void MultiCurveWidget::addValues(const QList<double> &x, const QList<double> &y)
{
	QList<double>::const_iterator itXValues = x.begin();
	QList<double>::const_iterator itYValues = y.begin();
	foreach(QCPCurve* c, _curves)
	{
		if (itXValues != x.end() && itYValues != y.end())
		{
			c->addData(*itXValues, *itYValues);
			if (_dataNumberLimit > 0 && c->data()->count() > _dataNumberLimit)
				c->removeData(c->data()->begin().key());
		}

		++itXValues;
		++itYValues;
	}

	customRescale();
	replot();
}

void MultiCurveWidget::setValues(int curveIndex, const QVector<double> &x, const QVector<double> &y)
{
	QCPCurve* c = getCurve(curveIndex);
	if (c)
	{
		c->setData(x, y);
		if (_dataNumberLimit > 0 && c->data()->count() > _dataNumberLimit)
			c->removeData(c->data()->begin().key(), c->data()->begin().key() + c->data()->count() - _dataNumberLimit - 1);

		customRescale();
		replot();
	}
}

void MultiCurveWidget::clear()
{
	foreach(QCPCurve* c, _curves)
		c->clearData();

	replot();
}

QCPCurve *MultiCurveWidget::getCurve(int i) const
{
	return _curves.value(i, 0);
}


void MultiCurveWidget::setDataNumberLimit(int limit)
{
	_dataNumberLimit = limit;
}

void MultiCurveWidget::customRescale()
{
	if (_autoResize)
		rescaleAxes();
	else if (_dataNumberLimit > 0)
	{
		foreach(QCPCurve* c, _curves)
		{
			if (c->data()->count() >= _dataNumberLimit)
				c->rescaleKeyAxis();
		}
	}
}

//-----------------------

MultiBarWidget::MultiBarWidget(QWidget *parent) :
	NPlotWidget(parent)
{
    //setRangeZoom(Qt::Horizontal | Qt::Vertical);
    //setInteraction(QCustomPlot::iSelectPlottables);

     setBackground(QBrush(Qt::white));

	legend->setVisible(true);
	QFont legendFont = font();  // start out with MainWindow's font..
	legendFont.setPointSize(9); // and make a bit smaller for legend
	legend->setFont(legendFont);
    axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);
}

MultiBarWidget::~MultiBarWidget()
{
}

QCPBars *MultiBarWidget::addBar(const QString &name, const QPen &pen, const QBrush &brush)
{
	QCPBars* b = new QCPBars(xAxis, yAxis);
	b->setPen(pen);
	b->setBrush(brush);
	b->setName(name);
	addPlottable(b);
	_bars << b;

	return b;
}

void MultiBarWidget::setValues(const QList<double> &values)
{
	int i = 0;
	foreach(QCPBars* b, _bars)
	{
		b->clearData();
		b->addData(i + 1, values.value(i++));
	}

	if (_autoResize)
		rescaleAxes();
	replot();
}

void MultiBarWidget::clear()
{
	foreach(QCPBars* b, _bars)
	{
		b->clearData();
	}

	replot();
}

QCPBars *MultiBarWidget::getBar(int i) const
{
	return _bars.value(i, 0);
}


