#ifndef RMOVEMENT_H
#define RMOVEMENT_H

#include "RPoint.h"


namespace Tools
{
    /**
     * \brief This class represents a movement for the robot : it includes destination, movement type and isStopPoint.
     */

    enum Movement
    {
        NONE = 0,
        AVANT_XY = 1,
        ARRIERE_XY = 2,
        TOURNE_VERS_XY = 3,
        TOURNE_VERS_XY_AR = 4,
        TOURNE_RADIAN = 5,
        AVANCE_MM = 6,  //X
        ROTATE_TO_ABSOLUTE_ANGLE = 7,
        CIRC_AVANT = 8,
        CIRC_AR = 9,
        BEZIER_AV = 10,
        BEZIER_AR = 11,
        RECALAGE = 12,
        AUTO = 13
    };

	enum Deplacement
	{
		TURN_THEN_MOVE = 0,
		TURN_AND_MOVE = 1,
		TURN_ONLY = 2,
		MOVE_ONLY = 3
	};

    enum Direction {Backward = 0, Forward = 1, Unknown = 2};

    class RMovement
    {
    public:
		RMovement();
		RMovement(const RPoint& destination, int type, int deplacement, bool isStopPoint, int speed);
        RMovement(const RMovement& M);

        const RPoint& getDestination() const;
        int getType() const;
		int getDeplacement() const;
        bool isStopPoint() const;
		int getSpeed() const;

        void setDestination(RPoint& newDestination);
        void setType(int newType);
		void setDeplacement(int dep);
		void setSpeed(int speed);

		static bool isRelative(Movement m);
		static bool isPureRotation(Movement m);
        static Direction getMouvementDirection(Movement m);
        static Movement reverseMovement(Movement m);

    private:
        RPoint _destination;
        int _type;
		int _deplacement;
        bool _isStopPoint;
		int _speed; //percentage
    };
}

#endif // RMOVEMENT_H
