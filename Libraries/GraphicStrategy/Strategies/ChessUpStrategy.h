#ifndef CHESSUPSTRATEGY_H
#define CHESSUPSTRATEGY_H

#include "StrategyInterface.h"

//class ChessUpStrategy : public StrategyInterface
//{
//public:
//    ChessUpStrategy();
//    virtual ~ChessUpStrategy() {}

//    enum RobotSide
//    {
//	Left,
//	Right
//    };

//protected:
//    virtual Tools::RPoint startPoint() const;
//    virtual void defaultObstaclePositions(QList<Tools::DataObject*>& objects) const;
//    virtual void avoidingSharps(QMap<int, Sharp*>& sharpList) const;
//    virtual void detectionSharps(QMap<int, Sharp*>& sharpList) const;
//    virtual void parameters(StrategyParameters& parameters) const;


//    AbstractAction* dropAtCoordinates(double x, double y);
//    AbstractAction* dropInSquare(int X, int Y);
//    AbstractAction* takePawnAtCoordinates(double x, double y);
//    AbstractAction* takeKingOrQueenAtCoordinates(double x, double y);
//    AbstractAction* takePawnInDispensingArea(int position, bool opponentArea);
//    AbstractAction* takeQueenOrKingInDispensingArea(int position, bool opponentArea);
//    AbstractAction* scanActions(RobotSide side, bool* isFigure, bool* smthDetected);

//    Tools::RPoint getDispansingAreaPosition(int position, bool opponentArea);

//    bool scan(RobotSide side, double d, bool* nothingDetected = 0);

//private:
//    bool _tmpSharpT1;
//    bool _tmpSharpT2;
//};

////--------------------------------------------------------------------------------------------------------------------------------------------

//class ChessUpHomologationStrategy : public ChessUpStrategy
//{
//public:
//    ChessUpHomologationStrategy();

//protected:

//   //Strategy
//    virtual void mainStrategy(QList<AbstractAction*>& actions);
//    virtual void actionAt(int index, bool lastActionResult);
//    void parameters(StrategyParameters& parameters) const;
//};

////--------------------------------------------------------------------------------------------------------------------------------------------

//class ChessUpTwinTowerStrategy : public ChessUpStrategy
//{
//public:
//    ChessUpTwinTowerStrategy();

//protected:

//   //Strategy
//    virtual void mainStrategy(QList<AbstractAction*>& actions);
//    virtual void actionAt(int index, bool lastActionResult);
//    void parameters(StrategyParameters& parameters) const;
//};

////--------------------------------------------------------------------------------------------------------------------------------------------

//class ChessUpBasicStrategy : public ChessUpStrategy
//{
//public:
//    ChessUpBasicStrategy();

//protected:

//   //Strategy
//    virtual void mainStrategy(QList<AbstractAction*>& actions);
//    virtual void actionAt(int index, bool lastActionResult);
//    void parameters(StrategyParameters& parameters) const;

//private:
//    int takePawn1;
//    int scan1;
//    int takePawn2;
//    int scan2;
//    int takePawn3;
//    int scan3;
//    int takePawn4;
//    int scan4;
//    int takePawn5;

//    bool firstPawnFound;

//    int scanningTemp;
//    int scanDistance;

//    QVector<bool> _queenKingPositions;
//};

#endif // CHESSUPSTRATEGY_H
