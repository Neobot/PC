#ifndef SIMPLECURVEVIEW_H
#define SIMPLECURVEVIEW_H

#include <qcustomplot.h>
#include <QVector>

#include "DoubleCurveView.h"

class XCurveView : public DoubleCurveView
{
public:
	explicit XCurveView(QWidget *parent = 0);

	void addPositionAcquisition(int t, const PositionSample &sample, int ct, const CommandSample &csample);
	void addAvoidingSharpAcquisition(int t, const SharpSample &sample);
	void setAcquisition(const Acquisition* acquisition);
};

class YCurveView : public DoubleCurveView
{
public:
	explicit YCurveView(QWidget *parent = 0);

	void addPositionAcquisition(int t, const PositionSample &sample, int ct, const CommandSample &csample);
	void addAvoidingSharpAcquisition(int t, const SharpSample &sample);
	void setAcquisition(const Acquisition* acquisition);
};

class ThetaCurveView : public DoubleCurveView
{
public:
	explicit ThetaCurveView(QWidget *parent = 0);

	void addPositionAcquisition(int t, const PositionSample &sample, int ct, const CommandSample &csample);
	void addAvoidingSharpAcquisition(int t, const SharpSample &sample);
	void setAcquisition(const Acquisition* acquisition);
};

#endif // SIMPLECURVEVIEW_H
