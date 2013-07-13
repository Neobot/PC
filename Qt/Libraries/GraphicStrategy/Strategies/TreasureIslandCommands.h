#ifndef TREASUREISLANDCOMMANDS_H
#define TREASUREISLANDCOMMANDS_H

#include "AbstractAICommand.h"
#include "Curve.h"

#define START_NODE "Start"
#define BOTTLE1_NODE "Bottle1"
#define BOTTLE2_NODE "Bottle2"
#define DECK_NODE "Deck"
#define MAP_NODE "Map"

#define TOTEM1_NORTH_NODE "Totem1InN"
#define TOTEM1_WEST_NODE "Totem1OutW"
#define TOTEM1_SOUTH_NODE "Totem1InS"
#define TOTEM2_NORTH_NODE "Totem2OutN"
#define TOTEM2_EAST_NODE "Totem2InE"
#define TOTEM2_SOUTH_NODE "Totem2OutS"

#define OPP_START_NODE "OpponentStart"
#define OPP_DECK_NODE "OpponentDeck"

#define CAPTAIN_BEDROOM_AREA "CaptainBedroom"
#define LOADING_DECK_AREA "LoadingDeck"
#define OPPONENT_BEDROOM_AREA "OpponentBedroom"
#define OPPONENT_DECK_AREA "OpponentDeck"
#define SOUTH_SEA_AREA "SouthSea"

#define AVERAGE_SPEED 200.0 //mm/s

#define ROBOT_CONTENT "RobotContent"
#define MAX_ROBOT_CONTENT 3


class TSBottleCommand : public AbstractAICommand
{
public:
    TSBottleCommand(const QString& bottleAlias, StrategyManager* manager);

    double evaluate(GameState &state);
    void updateToFinalState(GameState &state) const;

    AbstractAction* getAction() const;
private:
    QString _bottleAlias;
};

class TSMapCommand : public AbstractAICommand
{
public:
    TSMapCommand(const QString& mapAlias, StrategyManager* manager);

    double evaluate(GameState &state);
    void updateToFinalState(GameState &state) const;

    AbstractAction* getAction() const;
private:
    QString _maplias;
};

class TSGatherSouthSeaCommand : public AbstractAICommand
{
public:
    TSGatherSouthSeaCommand(const QString& areaAlias, const Tools::Curve& curve, StrategyManager* manager);

    double evaluate(GameState &state);
    void updateToFinalState(GameState &state) const;

    AbstractAction* getAction() const;
    void end();

private:
    QString _areaAlias;
    Tools::Curve _probabilityCurve;
};

class TSGatherTotemCommand : public AbstractAICommand
{
public:
    TSGatherTotemCommand(const QString& nodeInAlias, const QString& nodeOutAlias, const QString& mainNodeAlias, const Tools::Curve& curve, StrategyManager* manager);

    double evaluate(GameState &state);
    void updateToFinalState(GameState &state) const;

    AbstractAction* getAction() const;

private:
    QString _nodeInAlias;
    QString _nodeOutAlias;
    QString _mainNodeAlias;
    Tools::Curve _probabilityCurve;
};

class TSReleaseCommand : public AbstractAICommand
{
public:
    TSReleaseCommand(const QString& areaAlias, int maxPointsInside, StrategyManager* manager);

    double evaluate(GameState &state);
    void updateToFinalState(GameState &state) const;

    AbstractAction* getAction() const;
private:
    QString _areaAlias;
    int _maxPoints;
};

class TSPushBarCommand : public AbstractAICommand
{
public:
    TSPushBarCommand(const QString& nodeAlias, StrategyManager* manager);

    double evaluate(GameState &state);
    void updateToFinalState(GameState &state) const;

    AbstractAction* getAction() const;
private:
    QString _nodeAlias;
};



#endif // TREASUREISLANDCOMMANDS_H
