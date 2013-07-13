#include "SharpView.h"
#include <QPainter>

SharpView::SharpView(QWidget *parent) :
	QWidget(parent), _margin(10)
{
}

void SharpView::addPositionAcquisition(int t, const PositionSample &sample, int ct, const CommandSample &csample)
{
	Q_UNUSED(t);
	Q_UNUSED(sample);
	Q_UNUSED(ct);
	Q_UNUSED(csample);
}

void SharpView::addCommandAcquisition(int t, const CommandSample &sample)
{
	Q_UNUSED(t);
	Q_UNUSED(sample);
}

void SharpView::addAvoidingSharpAcquisition(int t, const SharpSample &sample)
{
	_values = sample._values;
	update();
}

void SharpView::setAcquisition(const Acquisition *acquisition)
{
	if (!acquisition->getAvoidingSharpSamples().isEmpty())
	{
		const SharpSample& s = *(acquisition->getAvoidingSharpSamples().end() - 1);
		_values = s._values;
	}
	update();
}

void SharpView::clear()
{
	_values.clear();
	update();
}

void SharpView::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	QPainter painter(this);

	int nbSharp = _values.count();
	if (nbSharp > 0)
	{
		int h = height();
		int w = width() / nbSharp;
		int maxValue = *(std::max_element(_values.begin(), _values.end()));
		for(int i = 0; i < nbSharp; ++i)
		{
			int value = _values[i];
			int hv = maxValue == 0 ? 0 : (value * h / maxValue);

			QRect r(i*w, height(), w, -hv);
			painter.fillRect(r, Qt::lightGray);

			QPen p(Qt::black);
			p.setWidth(3);
			painter.setPen(p);
			painter.drawRect(r);

			QRect rt(i*w, 10, w, height() - 20);
			QTextOption o(Qt::AlignBottom | Qt::AlignHCenter);
			painter.drawText(rt, QString::number(value), o);
		}
	}
}
