#include "Samples.h"


PositionSample::PositionSample()
{
	clear();
}

PositionSample::PositionSample(qint16 x, qint16 y, double theta)
{
	set(x, y, theta);
}

void PositionSample::set(qint16 x, qint16 y, double theta)
{
	_x = x;
	_y = y;
	_theta = theta;
}

void PositionSample::clear()
{
	_x = 0;
	_y = 0;
	_theta = 0.0;
}

bool PositionSample::operator==(const PositionSample &other) const
{
	return _x == other._x && _y == other._y && _theta == other._theta;
}

PositionSample &PositionSample::operator=(const PositionSample &other)
{
	if (this == &other)
		return *this;

	_x = other._x;
	_y = other._y;
	_theta = other._theta;

	return *this;
}

CommandSample::CommandSample()
{
	clear();
}

CommandSample::CommandSample(qint16 x, qint16 y, double theta)
{
	set(x, y, theta);
}

CommandSample &CommandSample::operator=(const CommandSample &other)
{
	if (this == &other)
		return *this;

	_cx = other._cx;
	_cy = other._cy;
	_ctheta = other._ctheta;

	return *this;
}

void CommandSample::set(qint16 cx, qint16 cy, double ctheta)
{
	_cx = cx;
	_cy = cy;
	_ctheta = ctheta;
}

bool CommandSample::operator==(const CommandSample &other) const
{
	return _cx == other._cx && _cy == other._cy && _ctheta == other._ctheta;
}

void CommandSample::clear()
{
	_cx = 0;
	_cy = 0;
	_ctheta = 0.0;
}

SharpSample::SharpSample()
{
	clear();
}

SharpSample::SharpSample(const QList<quint8> &values)
{
	set(values);
}

SharpSample &SharpSample::operator=(const SharpSample &other)
{
	if (this == &other)
		return *this;

	_values = other._values;

	return *this;
}

void SharpSample::set(const QList<quint8> &values)
{
	_values = values;
}

bool SharpSample::operator==(const SharpSample &other) const
{
	return _values == other._values;
}

void SharpSample::clear()
{
	_values.clear();
}
