#ifndef DOUBLECURVEVIEW_H
#define DOUBLECURVEVIEW_H

#include <qcustomplot.h>

#include <QVector>

#include "ViewInterface.h"

class DoubleCurveView : public QCustomPlot, public ViewInterface
{
	Q_OBJECT
public:
	explicit DoubleCurveView(QWidget *parent = 0);
	virtual ~DoubleCurveView();

	void setLabels(const QString& title, const QString& hAxis, const QString& vAxis);

	void clear();

protected:
	void addFirst(double x1, double y1);
	void addSecond(double x2, double y2);
	void add(double x1, double y1, double x2, double y2);

	void setFirst(const QVector<double>& x1, const QVector<double>& y1);
	void setSecond(const QVector<double>& x2, const QVector<double>& y2);
	void set(const QVector<double>& x1, const QVector<double>& y1, const QVector<double>& x2, const QVector<double>& y2);

protected:
	QCPCurve* _c1;
	QCPCurve* _c2;
};

class XYCurveView : public DoubleCurveView
{
public:
	explicit XYCurveView(QWidget *parent = 0);

	void addPositionAcquisition(int t, const PositionSample &sample, int ct, const CommandSample &csample);
	void addAvoidingSharpAcquisition(int t, const SharpSample &sample);
	void setAcquisition(const Acquisition* acquisition);
};


#endif // DOUBLECURVEVIEW_H
