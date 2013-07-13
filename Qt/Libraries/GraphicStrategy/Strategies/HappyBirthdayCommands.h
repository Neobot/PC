#ifndef TREASUREISLANDCOMMANDS_H
#define TREASUREISLANDCOMMANDS_H

#include "AbstractAICommand.h"
#include "Curve.h"

#include <QStringList>


#define START_NODE "Start"
#define PRESENT1_NODE "Present1"
#define PRESENT2_NODE "Present2"
#define PRESENT3_NODE "Present3"
#define PRESENT4_NODE "Present4"

#define GLASS1A_NODE "Glass1A"
//#define GLASS1B_NODE "Glass1B"
#define GLASS2A_NODE "Glass2A"
#define GLASS3A_NODE "Glass3A"
#define GLASS4A_NODE "Glass4A"
#define GLASS5A_NODE "Glass5A"
#define GLASS6A_NODE "Glass6A"
#define GLASS7A_NODE "Glass7A"
#define GLASS8A_NODE "Glass8A"
#define GLASS9A_NODE "Glass9A"
#define GLASS10A_NODE "Glass10A"
#define GLASS11A_NODE "Glass11A"
#define GLASS12A_NODE "Glass12A"

#define GLASS1_AREA "Glass1Area"
#define GLASS2_AREA "Glass2Area"
#define GLASS3_AREA "Glass3Area"
#define GLASS4_AREA "Glass4Area"
#define GLASS5_AREA "Glass5Area"
#define GLASS6_AREA "Glass6Area"
#define GLASS7_AREA "Glass7Area"
#define GLASS8_AREA "Glass8Area"
#define GLASS9_AREA "Glass9Area"
#define GLASS10_AREA "Glass10Area"
#define GLASS11_AREA "Glass11Area"
#define GLASS12_AREA "Glass12Area"

#define CONSTRUCTION_AREA "ConstructionArea"

#define CANDLE1_NODE "Candle1"
#define CANDLE2_NODE "Candle2"
#define CANDLE3_NODE "Candle3"


#define AVERAGE_SPEED 200.0 //mm/s

#define ROBOT_CONTAINER_LEFT "RobotLeftContainer"
#define ROBOT_CONTAINER_RIGHT "RobotRightContainer"

#define LEFT_ARM_GROUP "BrasGauche"
#define RIGHT_ARM_GROUP "BrasDroit"
#define PRESENT_MOVEMENT "Cadeau"
#define INIT_MOVEMENT "GoToInit"


namespace Tools
{
	class NGrid;
}

class HBOpenPresentCommand : public AbstractAICommand
{
public:
	HBOpenPresentCommand(const QString& presentAlias, double estimatedTimePerPresent, int maxNumberOfGlassesPerContainer, StrategyManager* manager);

    double evaluate(GameState &state);
    void updateToFinalState(GameState &state) const;

	AbstractAction* getAction(const GameState& state) const;
private:
	QString _presentAlias;
	double _estimatedTimePerPresent;
	int _maxNumberOfGlassesPerContainer;
};

class HBAreaLocker
{
public:
	HBAreaLocker(Tools::NGrid* grid) : _grid(grid) {}
	void lockArea(const QString& area, int costToGoInside, int costToGoOutside, int internalCost);
	void unlockArea(const QString& area);

private:
	struct AreaLockingValues
	{
		int costToGoInside;
		int costToGoOutside;
		int internalCost;
	};

	Tools::NGrid* _grid;
	QHash<QString, AreaLockingValues> _lockedAreas;
	QStringList _areaList;

	void setAreaValues(const QString& area);
};

class HBTakeGlassCommand : public AbstractAICommand
{
public:
	enum Arm
	{
		Right,
		Left,
		Both
	};
	
	HBTakeGlassCommand(int glassNum, const QString& glassNodeAlias, const QString& glassAreaAlias, Arm arm,
					   int estimatedPointsPerGlass, double estimatedTimePerGlass,
					   int leftContainerMaxContent, int rightContainerMaxContent,
					   HBAreaLocker* areaLocker, StrategyManager* manager);

	double evaluate(GameState &state);
	void updateToFinalState(GameState &state) const;

	AbstractAction* getAction(const GameState& state) const;
	void end();
	
private:
	QString _glassId;
	QString _nodeAlias;
	QString _areaAlias;
	bool _doLeft;
	bool _doRight;
	HBAreaLocker* _areaLocker;

	int _estimatedPointsPerGlass;
	double _estimatedTimePerGlass;

	int _leftContainerMaxContent;
	int _rightContainerMaxContent;
};

class HBReleaseCommand : public AbstractAICommand
{
public:
   HBReleaseCommand(const QString& areaAlias, StrategyManager* manager);

   double evaluate(GameState &state);
   void updateToFinalState(GameState &state) const;

   AbstractAction* getAction(const GameState& state) const;
private:
   QString _areaAlias;
   int calculatePoint(int nbGlassTower) const;
};

class HBBlowCandleCommand : public AbstractAICommand
{
public:
	HBBlowCandleCommand(const QString& candleAlias, double estimatedTimePerCandle, int maxNumberOfGlassesPerContainer, StrategyManager* manager);

	double evaluate(GameState &state);
	void updateToFinalState(GameState &state) const;

	AbstractAction* getAction(const GameState& state) const;
private:
	QString _candleAlias;
	double _estimatedTimePerCandle;
	int _maxNumberOfGlassesPerContainer;
};


#endif // TREASUREISLANDCOMMANDS_H
