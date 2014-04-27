#ifndef PREHISTOBOTCOMMANDS_H
#define PREHISTOBOTCOMMANDS_H

#include "AbstractAICommand.h"
#include "Curve.h"

#include <QStringList>


#define START_NODE "Start"
#define FRESCO_NODE "Fresco"

#define FRUIT_1A_NODE "Fruit1A"
#define FRUIT_1B_NODE "Fruit1B"
#define FRUIT_2A_NODE "Fruit2A"
#define FRUIT_2B_NODE "Fruit2B"
#define FRUIT_3A_NODE "Fruit3A"
#define FRUIT_3B_NODE "Fruit3B"
#define FRUIT_4A_NODE "Fruit4A"
#define FRUIT_4B_NODE "Fruit4B"

#define FRUIT_DROP_AREA "FruitDrop"

#define NB_FRUIT_PICKUP "NbFruitPickup"

#define AVERAGE_SPEED 200.0 //mm/s

namespace Tools
{
	class NGrid;
}

enum RobotSide
{
	LeftSide,
	RightSide
};

class PBFrescoCommand : public AbstractAICommand
{
public:
	PBFrescoCommand(const QString& frescoAlias, double estimatedTimeInSeconds, StrategyManager* manager);

	double evaluate(GameState &state);
	void updateToFinalState(GameState &state) const;

	AbstractAction* getAction(const GameState& state) const;
private:
	QString _frescoAlias;
	double _estimatedTime; //seconds
};

class PBFruitPickupCommand : public AbstractAICommand
{
public:
	PBFruitPickupCommand(const QString& fruitAliasA, double angleA, RobotSide sideA,
						 const QString& fruitAliasB, double angleB,  RobotSide sideB,
						 double distance, double estimatedTimeInSeconds, StrategyManager* manager);

	double evaluate(GameState &state);
	void updateToFinalState(GameState &state) const;

	AbstractAction* getAction(const GameState& state) const;
private:
	QString _fruitAliasA;
	double _angleA;
	RobotSide _sideA;

	QString _fruitAliasB;
	double _angleB;
	RobotSide _sideB;

	double _distance;
	double _estimatedTime; //seconds

	void getOptions(double distanceToA, double distanceToB, QString& alias, double& angle, RobotSide& side) const;
};

class PBFruitDropCommand : public AbstractAICommand
{
public:
	PBFruitDropCommand(const QString& dropAreaAlias, double estimatedTimeInSeconds, StrategyManager* manager);

	double evaluate(GameState &state);
	void updateToFinalState(GameState &state) const;

	AbstractAction* getAction(const GameState& state) const;
private:
	QString _dropAreaAlias;
	double _estimatedTime; //seconds
};

#endif // PREHISTOBOTCOMMANDS_H
