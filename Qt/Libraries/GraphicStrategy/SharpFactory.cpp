#include "SharpFactory.h"

using namespace Tools;

const int voltage_factor = 255 / 5; //255 = 5V

Sharp* SharpFactory::TestSharp(const QVector2D& position, const QVector2D& direction, double height)
{
    Curve calibration;
    calibration << Point(0, 0) << Point(255, 1000);

    Sharp* s = new Sharp(position, direction, height, calibration);

    return s;
}

Sharp* SharpFactory::Sharp20_150(double t1, const QVector2D &position, const QVector2D& direction, double height)
{
    Curve calibration;
    //
    calibration << Point(50, 145)
		<< Point(150, 145)
		<< Point(200, 140)
		<< Point(300, 110)
		<< Point(350, 100)
		<< Point(450, 75)
		<< Point(500, 70)
		<< Point(550, 60)
		<< Point(700, 48)
	       << Point(10000, 2);

    Sharp* s = new Sharp(position, direction, height, calibration, t1);

    return s;
}

Sharp* SharpFactory::Sharp10_80(double t1, const QVector2D& position, const QVector2D& direction, double height)
{
    Curve calibration;
    //
    calibration << Point(50, 180)
		<< Point(150, 95)
		<< Point(200, 73)
		<< Point(300, 50)
		<< Point(450, 35)
		<< Point(600, 26)
		<< Point(700, 22)
		<< Point(10000, 2);

    Sharp* s = new Sharp(position, direction, height, calibration, t1);

    return s;
}

Sharp* SharpFactory::Sharp4_30(double t1, const QVector2D& position, const QVector2D& direction, double height)
{
    Curve calibration;
    calibration << Point(50, 110)
		<< Point(100, 65)
		<< Point(150, 50)
		<< Point(200, 36)
		<< Point(350, 18)
		<< Point(500, 10)
		<< Point(10000, 2);

    Sharp* s = new Sharp(position, direction, height, calibration, t1);

    return s;
}

Sharp* SharpFactory::MaxSonar_MB1240(double t1, const QVector2D& position, const QVector2D& direction, double height)
{
	Curve calibration;
		calibration << Point(0, 0)
			<< Point(100, 255);

	Sharp* s = new Sharp(position, direction, height, calibration, t1);

	return s;
}
