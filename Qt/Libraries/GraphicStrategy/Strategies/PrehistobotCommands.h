#ifndef PREHISTOBOTCOMMANDS_H
#define PREHISTOBOTCOMMANDS_H

#include "AbstractAICommand.h"
#include "Curve.h"

#include <QStringList>
#include <QPointF>


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

#define TORCHE_1_NODE "Torche1"
#define TORCHE_2_NODE "Torche2"
#define TORCHE_3_NODE "Torche3"
#define TORCHE_4_NODE "Torche4"
#define TORCHE_5_NODE "Torche5"
#define TORCHE_6_NODE "Torche6"

#define EASYFIRE_1A_NODE "EasyFire1A"
#define EASYFIRE_1B_NODE "EasyFire1B"
#define EASYFIRE_2A_NODE "EasyFire2A"
#define EASYFIRE_2B_NODE "EasyFire2B"
#define EASYFIRE_3A_NODE "EasyFire3A"
#define EASYFIRE_3B_NODE "EasyFire3B"

#define HEARTH_1_NODE "Hearth1"

#define LEFT_HAND_HAS_FIRE "LeftHandHasFire"
#define RIGHT_HAND_HAS_FIRE "RightHandHasFire"

#define AVERAGE_SPEED 200.0 //mm/s

class PBActionFactory;

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

class PBSearchFiresCommand : public AbstractAICommand
{
public:
	PBSearchFiresCommand(const QList<QPointF> points, PBActionFactory* pbFactory, StrategyManager* manager);

	double evaluate(GameState &state);
	void updateToFinalState(GameState &state) const;

	AbstractAction* getAction(const GameState& state) const;
private:
	QList<QPointF> _points;
	PBActionFactory* _pbFactory;

	QList<QPointF> getSortedPointList(const GameState &state) const;
};

class PBTakeFixedTorcheCommand : public AbstractAICommand
{
public:
	PBTakeFixedTorcheCommand(const QString& torcheAlias, bool vertical, double estimatedTimeInSeconds, bool turnFire, PBActionFactory* pbFactory, StrategyManager* manager);

	double evaluate(GameState &state);
	void updateToFinalState(GameState &state) const;

	AbstractAction* getAction(const GameState& state) const;
private:
	PBActionFactory* _pbFactory;
	QString _torcheAlias;
	bool _vertical;
	double _estimatedTime; //seconds
	bool _turnFire;
	
	int getBestPump(const GameState &state) const;
	int getPump(const GameState &state, bool* isBest = 0) const;
};

class PBTakeMobileTorcheCommand : public AbstractAICommand
{
public:
	PBTakeMobileTorcheCommand(const QString& torcheAlias, double estimatedTimeInSeconds, PBActionFactory* pbFactory, StrategyManager* manager);

	double evaluate(GameState &state);
	void updateToFinalState(GameState &state) const;

	AbstractAction* getAction(const GameState& state) const;
private:
	PBActionFactory* _pbFactory;
	QString _torcheAlias;
	double _estimatedTime; //seconds
};

class PBEasyFireCommand : public AbstractAICommand
{
public:
	PBEasyFireCommand(const QString& aliasA, const QString& aliasB, double availableTimeToPerformAction, double estimatedTimeInSeconds, PBActionFactory* pbFactory, StrategyManager* manager);

	double evaluate(GameState &state);
	void updateToFinalState(GameState &state) const;

	AbstractAction* getAction(const GameState& state) const;
private:
	PBActionFactory* _pbFactory;
	QString _aliasA;
	QString _aliasB;
	double _availableTimeToPerformAction;
	double _estimatedTime; //seconds

	void getOptions(double distanceToA, double distanceToB, QString& firstAlias, QString& secondAlias) const;
};

class PBDropHeldFiresCommand : public AbstractAICommand
{
public:
	PBDropHeldFiresCommand(const QString& alias, bool onHearth, int maxFiresOnThisNode, double estimatedTimeInSeconds, PBActionFactory* pbFactory, StrategyManager* manager);

	double evaluate(GameState &state);
	void updateToFinalState(GameState &state) const;

	AbstractAction* getAction(const GameState& state) const;
private:
	PBActionFactory* _pbFactory;
	QString _alias;
	double _estimatedTime; //seconds
	bool _onHearth;
	int _maxFiresOnThisNode;
};


#endif // PREHISTOBOTCOMMANDS_H
