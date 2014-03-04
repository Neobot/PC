#ifndef SIMULATIONMAP_H
#define SIMULATIONMAP_H

#include "DataMap.h"
#include "ChessUpObject.h"
#include "ChessUpConfiguration.h"
#include "SharpObject.h"
#include "ColorSensorObject.h"
#include "ContactorObject.h"
#include "RPoint.h"

#include <QtDebug>
#include <QImage>

class SimulationMap : public Tools::DataMap
{
public:
	SimulationMap();
	~SimulationMap();

	enum CollisionGroup
	{
		PawnsGroup,
		QueensGroup,
		KingsGroup,
		AvoidingSharpGroup,
		OtherSharpGroup,
		ContactorGroup,
        ColorSensorGroup,
		OpponentGroup,
		RobotGroup,
		ObstacleGroup,
		TableObstacleGroup //For objects fixed on the table, not displayed
	};

	void setConfiguration(const ChessUpConfiguration& config);

	/**
	* \brief Moves the robot and its attached objects on the map (sharps, contactors)
	*/
	void moveRobot(const Tools::RPoint& robot);
	void takeChessUpObjects();
	void updateSensors();
	SharpObject* getSharpInClaw();

private:
    QImage _colorMap;

	void configureRobotSensors();
	double distanceTo(Tools::DataObject* o1, Tools::DataObject* o2);
	double distanceTo(Tools::LineObject* l1, Tools::CircleObject* c2);
	double distanceTo(Tools::LineObject* l1, Tools::RectangleObject* r2);
	double distanceTo(SharpObject* sharp, int group);
	double getObstacleDistanceToSharp(SharpObject* sharp);
};

#endif // SIMULATIONMAP_H
