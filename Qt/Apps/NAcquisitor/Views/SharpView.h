#ifndef SHARPVIEW_H
#define SHARPVIEW_H

#include <QWidget>

#include "ViewInterface.h"

class SharpView : public QWidget, public ViewInterface
{
	Q_OBJECT
public:
	explicit SharpView(QWidget *parent = 0);

	void addPositionAcquisition(int t, const PositionSample &sample, int ct, const CommandSample &csample);
	void addCommandAcquisition(int t, const CommandSample &sample);
	void addAvoidingSharpAcquisition(int t, const SharpSample &sample);
	void setAcquisition(const Acquisition* acquisition);
	void clear();

	void paintEvent(QPaintEvent *event);
	
private:
	QList<quint8> _values;
	int _margin;
	
};

#endif // SHARPVIEW_H
