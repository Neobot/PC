#include "DoubleCurveView.h"

DoubleCurveView::DoubleCurveView(QWidget *parent) :
	QCustomPlot(parent)
{
	_c1 = new QCPCurve(xAxis, yAxis);
	_c1->setPen(QPen(Qt::red));
	_c1->setName("Robot");

	_c2 = new QCPCurve(xAxis, yAxis);
	_c2->setPen(QPen(Qt::blue));
	_c2->setName("Consigne");

	setRangeZoom(Qt::Horizontal | Qt::Vertical);
	setInteraction(QCustomPlot::iSelectPlottables);

	addPlottable(_c1);
	addPlottable(_c2);

	setColor(Qt::white);

	legend->setVisible(true);
	QFont legendFont = font();  // start out with MainWindow's font..
	legendFont.setPointSize(9); // and make a bit smaller for legend
	legend->setFont(legendFont);
	legend->setPositionStyle(QCPLegend::psBottomRight);
	//legend->setBrush(QBrush(QColor(255,255,255,230)));

}

DoubleCurveView::~DoubleCurveView()
{
}

void DoubleCurveView::setLabels(const QString &title, const QString &hAxis, const QString &vAxis)
{
	setTitle(title);
//	setAxisTitle(xBottom, hAxis);
	//	setAxisTitle(yLeft, vAxis);
}

void DoubleCurveView::addFirst(double x1, double y1)
{
	_c1->addData(x1, y1);
	rescaleAxes();
	replot();
}

void DoubleCurveView::addSecond(double x2, double y2)
{
	_c2->addData(x2, y2);
	rescaleAxes();
	replot();
}

void DoubleCurveView::add(double x1, double y1, double x2, double y2)
{
	_c1->addData(x1, y1);
	_c2->addData(x2, y2);
	rescaleAxes();
	replot();
}

void DoubleCurveView::setFirst(const QVector<double> &x1, const QVector<double> &y1)
{
	_c1->setData(x1, y1);
	rescaleAxes();
	replot();
}

void DoubleCurveView::setSecond(const QVector<double> &x2, const QVector<double> &y2)
{
	_c2->setData(x2, y2);
	rescaleAxes();
	replot();
}

void DoubleCurveView::set(const QVector<double> &x1, const QVector<double> &y1, const QVector<double> &x2, const QVector<double> &y2)
{
	_c1->setData(x1, y1);
	_c2->setData(x2, y2);
	rescaleAxes();
	replot();
}

void DoubleCurveView::clear()
{
	_c1->clearData();
	_c2->clearData();
	replot();
}

//------------

XYCurveView::XYCurveView(QWidget *parent) : DoubleCurveView(parent)
{
	_c2->setScatterStyle(QCP::ssCircle);
	_c2->setScatterSize(6);
	_c2->setLineStyle(QCPCurve::lsNone);
}

void XYCurveView::addPositionAcquisition(int t, const PositionSample &sample, int ct, const CommandSample &csample)
{
	Q_UNUSED(t);
	Q_UNUSED(ct);
	add(sample._x, sample._y, csample._cx, csample._cy);
}

void XYCurveView::addAvoidingSharpAcquisition(int t, const SharpSample &sample)
{
	Q_UNUSED(t);
	Q_UNUSED(sample);
}

void XYCurveView::setAcquisition(const Acquisition *acquisition)
{
	clear();

	QVector<double> x1, y1;
	const PositionSamples& samples = acquisition->getPositionSamples();
	foreach(const PositionSample& s, samples)
	{
		x1 << s._x;
		y1 << s._y;
	}

	QVector<double> x2, y2;
	const CommandSamples& csamples = acquisition->getCammandSamples();
	foreach(const CommandSample& s, csamples)
	{
		x2 << s._cx;
		y2 << s._cy;
	}

	set(x1, y1, x2, y2);
}


