#include "NPlotWidget.h"

using namespace Tools;

NPlotWidget::NPlotWidget(QWidget *parent) :
	QCustomPlot(parent), _autoResize(true)
{
	_defaultColors << Qt::blue << Qt::red << Qt::green << Qt::yellow << Qt::darkBlue << Qt::darkRed << Qt::darkGreen << Qt::darkYellow;
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

void NPlotWidget::rescale()
{
	if (_autoResize)
		rescaleAxes();
	replot();
}

//------------


MultiCurveWidget::MultiCurveWidget(QWidget *parent) : NPlotWidget(parent), _dataNumberLimit(-1), _time(0)
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

	if (_dataNumberLimit > 0)
	{
		c->setData(_defaultTimeValues, QVector<double>(_dataNumberLimit, 0.0));
	}

	return c;
}

QCPCurve *MultiCurveWidget::addCurve(const QString &name)
{
	return addCurve(name, QPen(_defaultColors.value(_curves.count())));
}

void MultiCurveWidget::addValue(int curveIndex, double x, double y)
{
	QCPCurve* c = getCurve(curveIndex);
	if (c)
	{
		c->addData(x, y);
		if (_dataNumberLimit > 0 && c->data()->count() > _dataNumberLimit)
			c->removeData(c->data()->begin().key());

		rescale();
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

	rescale();
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

	rescale();
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

	rescale();
}

void MultiCurveWidget::addTimeSample(const QList<double> &values)
{
	if (!_samplesPlotIndexesReceived.isEmpty())
		++_time;

	addValues(_time, values);
	++_time;
	_samplesPlotIndexesReceived.clear();
}

void MultiCurveWidget::addSingleTimeSample(int plotIndex, double value)
{
	if (_samplesPlotIndexesReceived.contains(plotIndex))
	{
		++_time;
		_samplesPlotIndexesReceived.clear();
	}

	addValue(plotIndex, _time, value);
	_samplesPlotIndexesReceived.insert(plotIndex);
}

void MultiCurveWidget::setValues(int curveIndex, const QVector<double> &x, const QVector<double> &y)
{
	QCPCurve* c = getCurve(curveIndex);
	if (c)
	{
		c->setData(x, y);
		if (_dataNumberLimit > 0 && c->data()->count() > _dataNumberLimit)
			c->removeData(c->data()->begin().key(), c->data()->begin().key() + c->data()->count() - _dataNumberLimit - 1);

		rescale();
	}
}

void MultiCurveWidget::clear()
{
	foreach(QCPCurve* c, _curves)
	{
		c->clearData();
		if (_dataNumberLimit > 0)
		{
			c->setData(_defaultTimeValues, QVector<double>(_dataNumberLimit, 0.0));
		}
	}

	_time = _dataNumberLimit;
	replot();
}

QCPCurve *MultiCurveWidget::getCurve(int i) const
{
	return _curves.value(i, 0);
}

void MultiCurveWidget::setDataNumberLimit(int limit)
{
	_dataNumberLimit = limit;

	if (_dataNumberLimit > 0)
	{
		_defaultTimeValues.clear();
		_defaultTimeValues.resize(_dataNumberLimit);
		for(int i = 0; i < _dataNumberLimit; ++i)
			_defaultTimeValues[i] = i;

		foreach(QCPCurve* c, _curves)
		{
			c->setData(_defaultTimeValues, QVector<double>(_dataNumberLimit, 0.0));
		}
	}

	_time += _dataNumberLimit;
}

void MultiCurveWidget::rescale()
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

	replot();
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

QCPBars *MultiBarWidget::addBar(const QString &name)
{
	return addBar(name, QPen(Qt::black), QBrush(_defaultColors.value(_bars.count())));
}

void MultiBarWidget::setValues(const QList<double> &values)
{
	int i = 0;
	foreach(QCPBars* b, _bars)
	{
		b->clearData();
		b->addData(i + 1, values.value(i));
		i++;
	}

	rescale();
}

void MultiBarWidget::addTimeSample(const QList<double> &values)
{
	setValues(values);
}

void MultiBarWidget::addSingleTimeSample(int plotIndex, double value)
{
	QCPBars* b = _bars.value(plotIndex, nullptr);
	if (b)
	{
		b->clearData();
		b->addData(plotIndex + 1, value);
	}

	rescale();
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


