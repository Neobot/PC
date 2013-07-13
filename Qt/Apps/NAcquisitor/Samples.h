#ifndef SAMPLES_H
#define SAMPLES_H

#include <QtGlobal>
#include <QList>

class PositionSample
{
public:
	PositionSample();
	PositionSample(qint16 x, qint16 y, double theta);

	PositionSample &operator=(const PositionSample &other);

	void clear();
	void set(qint16 x, qint16 y, double theta);

	bool operator==(const PositionSample &other) const;

	qint16 _x;
	qint16 _y;
	double _theta;
};

class CommandSample
{
public:
	CommandSample();
	CommandSample(qint16 cx, qint16 cy, double ctheta);

	CommandSample& operator=(const CommandSample& other);

	void clear();
	void set(qint16 cx, qint16 cy, double ctheta);

	bool operator==(const CommandSample& other) const;

	qint16 _cx;
	qint16 _cy;
	double _ctheta;
};

class SharpSample
{
public:
	SharpSample();
	SharpSample(const QList<quint8>& values);

	SharpSample &operator=(const SharpSample &other);

	void clear();
	void set(const QList<quint8>& values);

	bool operator==(const SharpSample& other) const;

	QList<quint8> _values;
};

#endif // SAMPLES_H
