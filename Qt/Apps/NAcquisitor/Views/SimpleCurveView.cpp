#include "SimpleCurveView.h"


//SimpleCurveView::SimpleCurveView(QWidget *parent) :
//	QCustomPlot(parent), _lastT(0)
//{
//	_robot = new QCPCurve(xAxis, yAxis);
//	_robot->setPen(QPen(Qt::red));
//	_robot->setName("Robot");

//	_consigne = new QCPCurve(xAxis, yAxis);
//	_consigne->setPen(QPen(Qt::blue));
//	_consigne->setName("Consigne");

//	addPlottable(_robot);
//	addPlottable(_consigne);

//	setColor(Qt::white);

//	legend->setVisible(true);
//	QFont legendFont = font();  // start out with MainWindow's font..
//	legendFont.setPointSize(9); // and make a bit smaller for legend
//	legend->setFont(legendFont);
//	legend->setPositionStyle(QCPLegend::psBottomRight);
//	//legend->setBrush(QBrush(QColor(255,255,255,230)));
//}


XCurveView::XCurveView(QWidget *parent) : DoubleCurveView(parent)
{
}

void XCurveView::addPositionAcquisition(int t, const PositionSample &sample, int ct, const CommandSample &csample)
{
	add(t, sample._x, ct, csample._cx);
}

void XCurveView::addAvoidingSharpAcquisition(int t, const SharpSample &sample)
{
	Q_UNUSED(t);
	Q_UNUSED(sample);
}

void XCurveView::setAcquisition(const Acquisition *acquisition)
{
	clear();

	QVector<double> t1, x1;
	const PositionSamples& samples = acquisition->getPositionSamples();
	for(PositionSamples::const_iterator it = samples.constBegin(); it != samples.constEnd(); ++it)
	{
		t1 << it.key();
		x1 << it->_x;
	}

	QVector<double> t2, x2;
	const CommandSamples& csamples = acquisition->getCammandSamples();
	for(CommandSamples::const_iterator it = csamples.constBegin(); it != csamples.constEnd(); ++it)
	{
		t2 << it.key();
		x2 << it->_cx;
	}

	set(t1, x1, t2, x2);
}


//------------

YCurveView::YCurveView(QWidget *parent) : DoubleCurveView(parent)
{
}

void YCurveView::addPositionAcquisition(int t, const PositionSample &sample, int ct, const CommandSample &csample)
{
	add(t, sample._y, ct, csample._cy);
}

void YCurveView::addAvoidingSharpAcquisition(int t, const SharpSample &sample)
{
	Q_UNUSED(t);
	Q_UNUSED(sample);
}

void YCurveView::setAcquisition(const Acquisition* acquisition)
{
	clear();

	QVector<double> t1, y1;
	const PositionSamples& samples = acquisition->getPositionSamples();
	for(PositionSamples::const_iterator it = samples.constBegin(); it != samples.constEnd(); ++it)
	{
		t1 << it.key();
		y1 << it->_y;
	}

	QVector<double> t2, y2;
	const CommandSamples& csamples = acquisition->getCammandSamples();
	for(CommandSamples::const_iterator it = csamples.constBegin(); it != csamples.constEnd(); ++it)
	{
		t2 << it.key();
		y2 << it->_cy;
	}

	set(t1, y1, t2, y2);
}

//------------

ThetaCurveView::ThetaCurveView(QWidget *parent) : DoubleCurveView(parent)
{
}

void ThetaCurveView::addPositionAcquisition(int t, const PositionSample &sample, int ct, const CommandSample &csample)
{
	add(t, sample._theta, ct, csample._ctheta);
}

void ThetaCurveView::addAvoidingSharpAcquisition(int t, const SharpSample &sample)
{
	Q_UNUSED(t);
	Q_UNUSED(sample);
}

void ThetaCurveView::setAcquisition(const Acquisition *acquisition)
{
	clear();

	QVector<double> t1, theta1;
	const PositionSamples& samples = acquisition->getPositionSamples();
	for(PositionSamples::const_iterator it = samples.constBegin(); it != samples.constEnd(); ++it)
	{
		t1 << it.key();
		theta1 << it->_theta;
	}

	QVector<double> t2, theta2;
	const CommandSamples& csamples = acquisition->getCammandSamples();
	for(CommandSamples::const_iterator it = csamples.constBegin(); it != csamples.constEnd(); ++it)
	{
		t2 << it.key();
		theta2 << it->_ctheta;
	}

	set(t1, theta1, t2, theta2);
}
