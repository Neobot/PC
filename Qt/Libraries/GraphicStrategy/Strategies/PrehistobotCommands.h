#ifndef PREHISTOBOTCOMMANDS_H
#define PREHISTOBOTCOMMANDS_H

#include "AbstractAICommand.h"
#include "Curve.h"

#include <QStringList>


#define START_NODE "Start"
#define FRESCO_NODE "Fresco"
//#define GLASS1_AREA "Glass1Area"

#define AVERAGE_SPEED 200.0 //mm/s

namespace Tools
{
	class NGrid;
}

class PBFrescoCommand : public AbstractAICommand
{
public:
	PBFrescoCommand(const QString& frescoAlias, double estimatedTime, StrategyManager* manager);

	double evaluate(GameState &state);
	void updateToFinalState(GameState &state) const;

	AbstractAction* getAction(const GameState& state) const;
private:
	QString _frescoAlias;
	double _estimatedTime;
};

//class HBAreaLocker
//{
//public:
//	HBAreaLocker(Tools::NGrid* grid) : _grid(grid) {}
//	void lockArea(const QString& area, int costToGoInside, int costToGoOutside, int internalCost);
//	void unlockArea(const QString& area);

//private:
//	struct AreaLockingValues
//	{
//		int costToGoInside;
//		int costToGoOutside;
//		int internalCost;
//	};

//	Tools::NGrid* _grid;
//	QHash<QString, AreaLockingValues> _lockedAreas;
//	QStringList _areaList;

//	void setAreaValues(const QString& area);
//};

//class HBTakeGlassCommand : public AbstractAICommand
//{
//public:
//	enum Arm
//	{
//		Right,
//		Left,
//		Both
//	};
	
//	HBTakeGlassCommand(int glassNum, const QString& glassNodeAlias, const QString& glassAreaAlias, Arm arm,
//					   int estimatedPointsPerGlass, double estimatedTimePerGlass,
//					   int leftContainerMaxContent, int rightContainerMaxContent,
//					   HBAreaLocker* areaLocker, StrategyManager* manager);

//	double evaluate(GameState &state);
//	void updateToFinalState(GameState &state) const;

//	AbstractAction* getAction(const GameState& state) const;
//	void end();
	
//private:
//	QString _glassId;
//	QString _nodeAlias;
//	QString _areaAlias;
//	bool _doLeft;
//	bool _doRight;
//	HBAreaLocker* _areaLocker;

//	int _estimatedPointsPerGlass;
//	double _estimatedTimePerGlass;

//	int _leftContainerMaxContent;
//	int _rightContainerMaxContent;
//};

//class HBReleaseCommand : public AbstractAICommand
//{
//public:
//   HBReleaseCommand(const QString& areaAlias, StrategyManager* manager);

//   double evaluate(GameState &state);
//   void updateToFinalState(GameState &state) const;

//   AbstractAction* getAction(const GameState& state) const;
//private:
//   QString _areaAlias;
//   int calculatePoint(int nbGlassTower) const;
//};

//class HBBlowCandleCommand : public AbstractAICommand
//{
//public:
//	HBBlowCandleCommand(const QString& candleAlias, double estimatedTimePerCandle, int maxNumberOfGlassesPerContainer, StrategyManager* manager);

//	double evaluate(GameState &state);
//	void updateToFinalState(GameState &state) const;

//	AbstractAction* getAction(const GameState& state) const;
//private:
//	QString _candleAlias;
//	double _estimatedTimePerCandle;
//	int _maxNumberOfGlassesPerContainer;
//};


#endif // PREHISTOBOTCOMMANDS_H
