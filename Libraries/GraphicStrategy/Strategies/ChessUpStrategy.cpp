//#include "ChessUpStrategy.h"

//#include <SharpFactory.h>
//#include <StrategyManager.h>
//#include <StrategyMap.h>
//#include <ToolsLib.h>
//#include <vmath.h>
//#include <cmath>
//#include <QtDebug>

//using namespace Tools;

//const double TABLE_WIDTH = 3000.0;

//const int BOTTOM_LEFT_SHARP_INDEX = 9;
//const int TOP_LEFT_SHARP_INDEX = 8;
//const int BOTTOM_RIGHT_SHARP_INDEX = 7;
//const int TOP_RIGHT_SHARP_INDEX = 6;

//ChessUpStrategy::ChessUpStrategy()
//{
//}

//void ChessUpStrategy::parameters(StrategyParameters& parameters) const
//{
//    parameters.	loop = false;

//    parameters. replanInterval = 1000;			//ms
//    parameters. stopCircleRadius = 100;	    	//mm
//    parameters. tableSize = QSizeF(2100.0, 3000.0);	//mm
//    parameters. robotRadius = 270.0;			//mm
//    parameters. opponentRadius = 250.0;		//mm
//    parameters. noticeOfReceiptTimeOut = 300;		//ms

//    parameters. sharpObjectRadius = 100.0;		//mm
//    parameters. sharpDetectionOverlapRadius = 50.0;	//mm
//    parameters. sharpDetectionTime = 1500;		//ms

//    parameters. diagonalSmoothingMaxDistance = 400.0;	//mm
//    parameters. lostReplanInterval = 5000;  //ms
//}

//Tools::RPoint ChessUpStrategy::startPoint() const
//{
//    return autoMirror(RPoint(202.5, 135, pi/2));
//}

//void ChessUpStrategy::defaultObstaclePositions(QList<DataObject*>& objects) const
//{
//    objects << new CircleObject(10, QPointF(1850, 450)) << new CircleObject(10, QPointF(1850, 1135))
//            << new CircleObject(120, QPointF(2100, 800))

//            << new CircleObject(10, QPointF(1850, 1865)) << new CircleObject(10, QPointF(1850, 2550))
//            << new CircleObject(120, QPointF(2100, 2200));
//}

//void ChessUpStrategy::avoidingSharps(QMap<int, Sharp*>& sharpList) const
//{
//    //front
//    sharpList.insert( 0, SharpFactory::Sharp10_80(50, 50, QVector2D(172, -125), QVector2D(1, tan(0.19)), 0.0) );
//    sharpList.insert( 1, SharpFactory::Sharp20_150(85, 85, QVector2D(63, 0), QVector2D(1, 0), 0.0) );
//    sharpList.insert( 2, SharpFactory::Sharp10_80(50, 50, QVector2D(172, 125), QVector2D(1, tan(0.28)), 0.0) );

//    //back
//    sharpList.insert( 3, SharpFactory::Sharp10_80(50, 50, QVector2D(-115, 138), QVector2D(-1, tan(0.27)), 0.0) );
//    sharpList.insert( 4, SharpFactory::Sharp20_150(110, 110, QVector2D(-130, 0), QVector2D(-1, 0), 0.0) );
//    sharpList.insert( 5, SharpFactory::Sharp10_80(50, 50, QVector2D(-115, -138), QVector2D(-1, tan(0.23)), 0.0) );
//}

//void ChessUpStrategy::detectionSharps(QMap<int, Sharp*> &sharpList) const
//{
//    //left
//    sharpList.insert( BOTTOM_LEFT_SHARP_INDEX, SharpFactory::Sharp20_150(90, 90, QVector2D(15, 170), QVector2D(0, 1), 0.0) );
//    sharpList.insert( TOP_LEFT_SHARP_INDEX, SharpFactory::Sharp20_150(90, 90, QVector2D(63, 170), QVector2D(0, 1), 0.0) );

//    //right
//    sharpList.insert( BOTTOM_RIGHT_SHARP_INDEX, SharpFactory::Sharp20_150(90, 90, QVector2D(15, -170), QVector2D(0, -1), 0.0) );
//    sharpList.insert( TOP_RIGHT_SHARP_INDEX, SharpFactory::Sharp20_150(90, 90, QVector2D(63, -170), QVector2D(0, -1), 0.0) );

//    //front (for the pawns)
//    sharpList.insert( 10, SharpFactory::Sharp4_30(22, 22, QVector2D(42, 0), QVector2D(1, 0), 0.0) );
//}

//AbstractAction* ChessUpStrategy::scanActions(RobotSide side, bool* isFigure, bool* smthDetected)
//{
//    QList<AbstractAction*> scanActions;

//    scanActions << _manager->getActionFactory()->testSharpAction(side == Left ? BOTTOM_LEFT_SHARP_INDEX : BOTTOM_RIGHT_SHARP_INDEX, 0, smthDetected);
//    scanActions << _manager->getActionFactory()->testSharpAction(side == Left ? TOP_LEFT_SHARP_INDEX : TOP_RIGHT_SHARP_INDEX, 0, isFigure);

//    return _manager->getActionFactory()->actionList(scanActions);
//}

//bool ChessUpStrategy::scan(RobotSide side, double d, bool* nothingDetected)
//{
//    double db =_manager->getSharp(side == Left ? BOTTOM_LEFT_SHARP_INDEX : BOTTOM_RIGHT_SHARP_INDEX)->getDistance();
//    bool bottom = db >0 && db < d;
//    double dt = _manager->getSharp(side == Left ? TOP_LEFT_SHARP_INDEX : TOP_RIGHT_SHARP_INDEX)->getDistance();
//    bool top = dt > 0 && dt < d;

//    qDebug() << "SHARP DETECTION" << db << dt;

//    if (nothingDetected)
//        *nothingDetected = !bottom && !top;

//    return top;
//}

//AbstractAction* ChessUpStrategy::dropAtCoordinates(double x, double y)
//{
//    QList<AbstractAction*> actions;

////    actions << _manager->getActionFactory()->moveAction(autoMirror(RPoint(x, y)), 175.0, true)
////	    << _manager->getActionFactory()->clawAction(Comm::RELEASE)
////	    << _manager->getActionFactory()->manualBackwardMoveAction(200);

//    return _manager->getActionFactory()->actionList(actions);
//}

//AbstractAction* ChessUpStrategy::dropInSquare(int X, int Y)
//{
//    QList<AbstractAction*> actions;

//    if (X < 1 || X > 6 || Y < 1 || Y > 6)
//        return _manager->getActionFactory()->actionList(actions);

//    double x = -175 + 350*X;
//    double y = 275 + 350*Y;

//    if (Y == 1)
//        y = 650;
//    else if (Y == 6)
//        y = 2350;

////    if (X == 1)
////	actions << _manager->getActionFactory()->moveAction(autoMirror(RPoint(525, y)), 0.0, true);
////    else if (X == 6)
////    {
////	actions << _manager->getActionFactory()->moveAction(autoMirror(RPoint(1505, y)), 0.0, true)
////		<< _manager->getActionFactory()->manualMoveToPointAction(autoMirror(RPoint(1705, y)), true)
////		<< _manager->getActionFactory()->clawAction(Comm::RELEASE)
////		<< _manager->getActionFactory()->manualBackwardMoveAction(200);
////    }
////    else
////	actions << dropAtCoordinates(x, y);

//    return _manager->getActionFactory()->actionList(actions);
//}

//AbstractAction* ChessUpStrategy::takePawnAtCoordinates(double x, double y)
//{
//    QList<AbstractAction*> actions;

////    actions << _manager->getActionFactory()->moveAction(autoMirror(RPoint(x, y)), 200.0, true);
////    actions << _manager->getActionFactory()->clawAction(Comm::TAKE_PAWN, 50);

//    return _manager->getActionFactory()->actionList(actions);
//}

//AbstractAction* ChessUpStrategy::takeKingOrQueenAtCoordinates(double x, double y)
//{
//    QList<AbstractAction*> actions;

////    actions << _manager->getActionFactory()->moveAction(autoMirror(RPoint(x, y)), 200.0, true);
////    actions << _manager->getActionFactory()->clawAction(Comm::TAKE_QUEEN_OR_KING, 50);

//    return _manager->getActionFactory()->actionList(actions);
//}

//Tools::RPoint ChessUpStrategy::getDispansingAreaPosition(int position, bool opponentArea)
//{
//    if (position < 1 || position > 4)
//        return RPoint();

//    double x = 410 + position * 280;
//    double y = opponentArea ? 2350 : 650;

//    return RPoint(x, y);
//}

//AbstractAction* ChessUpStrategy::takePawnInDispensingArea(int position, bool opponentArea)
//{
//    QList<AbstractAction*> actions;

//    if (position < 1 || position > 4)
//        return _manager->getActionFactory()->actionList(actions);

//    RPoint p = getDispansingAreaPosition(position, opponentArea);

////    actions << _manager->getActionFactory()->moveAction(autoMirror(p), 0.0, true)
////           << _manager->getActionFactory()->manualAbsoluteTurnMoveAction(opponentArea || _manager->isMirrored() ? pi/2 : -pi/2)
////            << _manager->getActionFactory()->clawAction(Comm::TAKE_PAWN, 450)
////            << _manager->getActionFactory()->manualMoveToPointAction(autoMirror(p), false);

//    return _manager->getActionFactory()->actionList(actions);
//}

//AbstractAction* ChessUpStrategy::takeQueenOrKingInDispensingArea(int position, bool opponentArea)
//{
//    QList<AbstractAction*> actions;

//    if (position < 1 || position > 4)
//        return _manager->getActionFactory()->actionList(actions);

//    RPoint p = getDispansingAreaPosition(position, opponentArea);

////    actions << _manager->getActionFactory()->moveAction(autoMirror(p), 0.0, true)
////	    << _manager->getActionFactory()->manualAbsoluteTurnMoveAction(opponentArea ? pi/2 : -pi/2)
////            << _manager->getActionFactory()->clawAction(Comm::TAKE_QUEEN_OR_KING, 450)
////            << _manager->getActionFactory()->manualMoveToPointAction(autoMirror(p), false);

//    return _manager->getActionFactory()->actionList(actions);
//}

////--------------------------------------------------------------------------------------------------------------------------------------------
//// HOMOLOGATION STRATEGY
////--------------------------------------------------------------------------------------------------------------------------------------------
//ChessUpHomologationStrategy::ChessUpHomologationStrategy()
//{
//}

//void ChessUpHomologationStrategy::parameters(StrategyParameters& parameters) const
//{
//    ChessUpStrategy::parameters(parameters);
//}

//void ChessUpHomologationStrategy::mainStrategy(QList<AbstractAction *>& actions)
//{
////    actions << _manager->getActionFactory()->moveAction(autoMirror(RPoint(202.5, 650)), 0.0, true)
////	    << takePawnInDispensingArea(1, false)
////	    << dropInSquare(6, 1)
////	    << _manager->getActionFactory()->moveAction(autoMirror(RPoint(202.5, 650)), 0.0, false)
////            << _manager->getActionFactory()->manualMoveToPointAction(autoMirror(RPoint(202.5, 140)), false);
//}

//void ChessUpHomologationStrategy::actionAt(int index, bool lastActionResult)
//{
//}

////--------------------------------------------------------------------------------------------------------------------------------------------
//// TWIN TOWER STRATEGY
////--------------------------------------------------------------------------------------------------------------------------------------------
//ChessUpTwinTowerStrategy::ChessUpTwinTowerStrategy()
//{
//}

//void ChessUpTwinTowerStrategy::parameters(StrategyParameters& parameters) const
//{
//    ChessUpStrategy::parameters(parameters);
//}

//void ChessUpTwinTowerStrategy::mainStrategy(QList<AbstractAction *>& actions)
//{
////    //Sort de la zone de départ
////    actions << _manager->getActionFactory()->moveAction(autoMirror(RPoint(202.5, 550)), 0.0, true)
////	    << _manager->getActionFactory()->moveAction(autoMirror(RPoint(350, 800)), 0.0, true);

////    //scanActions(actions, factory, manager, Right, true, reversed, 0, 3);

////    actions << _manager->getActionFactory()->moveAction(autoMirror(RPoint(350, 800)), 0.0, true) //scan
////            << _manager->getActionFactory()->manualMoveToPointAction(autoMirror(RPoint(1800, 700)), true) //entrée zone sécu
////            << _manager->getActionFactory()->manualMoveToPointAction(autoMirror(RPoint(1700, 800)), false) //sortie zone sécu
////            << _manager->getActionFactory()->moveAction(autoMirror(RPoint(690, 625)), 0.0, true)
////            //<< _manager->getActionFactory()->manualAbsoluteTurnMoveAction(Tools::pi) //
////            //lacher pion
////            << _manager->getActionFactory()->moveAction(autoMirror(RPoint(970, 625)), 0.0, false)
////            << _manager->getActionFactory()->waitAction(5000) //Prendre figure
////            //<< _manager->getActionFactory()->moveAction(autoMirror(RPoint(970, 625)), 0.0, true)
////            << _manager->getActionFactory()->moveAction(autoMirror(RPoint(670, 625)), 0.0, true)
////            << _manager->getActionFactory()->moveAction(autoMirror(RPoint(690, 625)), 0.0, true)
////            << _manager->getActionFactory()->waitAction(5000) //Prendre pion
////            << _manager->getActionFactory()->moveAction(autoMirror(RPoint(1800, 1400)), 0.0, true)
////               //ouvrir pince
////            << _manager->getActionFactory()->manualTurnToAction(autoMirror(RPoint(2000, 1250)))
////            //<< _manager->getActionFactory()->manualMoveToPointAction(autoMirror(RPoint(1900, 1400)), true)
////            << _manager->getActionFactory()->manualMoveToPointAction(autoMirror(RPoint(1600, 1550)), false)
//               ;
//}

//void ChessUpTwinTowerStrategy::actionAt(int index, bool lastActionResult)
//{
//        switch(index)
//        {
//        case 0:
//            break;
//        }
//}

////--------------------------------------------------------------------------------------------------------------------------------------------
//// BASIC STRATEGY - One tower + 2 pawns or 1 semi-tower
////--------------------------------------------------------------------------------------------------------------------------------------------
//ChessUpBasicStrategy::ChessUpBasicStrategy() : _queenKingPositions(5)
//{
//    firstPawnFound = false;
//    takePawn1 = -1;
//    takePawn2 = -1;
//    takePawn3 = -1;
//    takePawn4 = -1;
//    takePawn5 = -1;
//    scan1 = -1;
//    scan2 = -1;
//    scan3 = -1;
//    scan4 = -1;

//    scanningTemp = 200;
//    scanDistance = 750;
//}

//void ChessUpBasicStrategy::parameters(StrategyParameters& parameters) const
//{
//    ChessUpStrategy::parameters(parameters);
//}

//void ChessUpBasicStrategy::mainStrategy(QList<AbstractAction *>& actions)
//{
//    //Sort de la zone de départ
////    actions << _manager->getActionFactory()->manualMoveToPointAction(autoMirror(RPoint(202.5, 530)), true)
////            << _manager->getActionFactory()->manualTurnToAction(autoMirror(RPoint(350, 800)))
////            << _manager->getActionFactory()->clawAction(Comm::TAKE_PAWN, 290);
//    takePawn1 = actions.count() - 1;
//}

//void ChessUpBasicStrategy::actionAt(int index, bool lastActionResult)
//{
////    if (index == takePawn1)
////    {
////        if (lastActionResult)
////        {
////            firstPawnFound = true;
////            _manager->addAtion(_manager->getActionFactory()->clawAction(Comm::RAISE, 1));
////        }

////         _manager->addAtion(_manager->getActionFactory()->manualMoveToPointAction(autoMirror(RPoint(690, 800)), true));
////         _manager->addAtion(_manager->getActionFactory()->manualAbsoluteTurnMoveAction(0));
////         _manager->addAtion(_manager->getActionFactory()->waitAction(scanningTemp));
////         scan1 = _manager->getActionCount() - 1;
////         takePawn1 = -1;
////    }
////    else if (index == scan1)
////    {
////        _queenKingPositions[0] = scan(_manager->isMirrored() ? Left : Right, scanDistance);
////        if (!firstPawnFound)
////            _manager->addAtion(_manager->getActionFactory()->clawAction(Comm::TAKE_PAWN, 200));


////        takePawn2 = _manager->getActionCount() - 1;
////        scan1 = -1;
////        if (firstPawnFound)
////            actionAt(takePawn2, true);
////    }

////    else if (index == takePawn2)
////    {
////        if (lastActionResult && !firstPawnFound)
////        {
////            firstPawnFound = true;
////            _manager->addAtion(_manager->getActionFactory()->clawAction(Comm::RAISE, 1));
////        }

////        _manager->addAtion(_manager->getActionFactory()->manualMoveToPointAction(autoMirror(RPoint(970, 800)), true));
////        _manager->addAtion(_manager->getActionFactory()->manualAbsoluteTurnMoveAction(0));
////        _manager->addAtion(_manager->getActionFactory()->waitAction(scanningTemp));
////        scan2 = _manager->getActionCount() - 1;
////        takePawn2 = -1;
////    }
////    else if (index == scan2)
////    {
////        _queenKingPositions[1] = scan(_manager->isMirrored() ? Left : Right, scanDistance);
////        if (!firstPawnFound)
////            _manager->addAtion(_manager->getActionFactory()->clawAction(Comm::TAKE_PAWN, 200));
////        takePawn3 = _manager->getActionCount() - 1;
////        scan2 = -1;

////        if (firstPawnFound)
////            actionAt(takePawn3, true);
////    }

////    else if (index == takePawn3)
////    {
////        if (lastActionResult && !firstPawnFound)
////        {
////            firstPawnFound = true;
////            _manager->addAtion(_manager->getActionFactory()->clawAction(Comm::RAISE, 1));
////        }

////        _manager->addAtion(_manager->getActionFactory()->manualMoveToPointAction(autoMirror(RPoint(1250, 800)), true));
////        _manager->addAtion(_manager->getActionFactory()->manualAbsoluteTurnMoveAction(0));
////        _manager->addAtion(_manager->getActionFactory()->waitAction(scanningTemp));
////        scan3 = _manager->getActionCount() - 1;
////        takePawn3 = -1;
////    }
////    else if (index == scan3)
////    {
////        _queenKingPositions[2] = scan(_manager->isMirrored() ? Left : Right, scanDistance);
////        if (!firstPawnFound)
////            _manager->addAtion(_manager->getActionFactory()->clawAction(Comm::TAKE_PAWN, 200));
////        takePawn4 = _manager->getActionCount() - 1;
////        scan3 = -1;

////        if (firstPawnFound)
////            actionAt(takePawn4, true);
////    }

////    else if (index == takePawn4)
////    {
////        if (lastActionResult && !firstPawnFound)
////        {
////            firstPawnFound = true;
////            _manager->addAtion(_manager->getActionFactory()->clawAction(Comm::RAISE, 1));
////            takePawn4 = -1;
////        }

////        _manager->addAtion(_manager->getActionFactory()->manualMoveToPointAction(autoMirror(RPoint(1530, 800)), true));
////        _manager->addAtion(_manager->getActionFactory()->manualAbsoluteTurnMoveAction(0));
////        _manager->addAtion(_manager->getActionFactory()->waitAction(scanningTemp));
////        scan4 = _manager->getActionCount() - 1;
////    }
////    else if (index == scan4)
////    {
////        _queenKingPositions[3] = scan(_manager->isMirrored() ? Left : Right, scanDistance);

////        //Zone sécu
////        _manager->addAtion(_manager->getActionFactory()->manualMoveToPointAction(autoMirror(RPoint(1650, 800))));
////        _manager->addAtion(_manager->getActionFactory()->manualAbsoluteTurnMoveAction(autoMirror(RPoint(0, 0, -pi/4)).getTheta()));
////        //_manager->addAtion(_manager->getActionFactory()->manualMoveToPointAction(autoMirror(RPoint(1750, 750)), true));
////        _manager->addAtion(_manager->getActionFactory()->manualBackwardMoveAction(200));
////        _manager->addAtion(_manager->getActionFactory()->clawAction(Comm::RAISE, 0));

////        //Depose zone bonus

////        _manager->addAtion(_manager->getActionFactory()->moveAction(autoMirror(RPoint(1400, 1500)), 0.0, true));
////        _manager->addAtion(_manager->getActionFactory()->manualMoveToPointAction(autoMirror(RPoint(1750, 1500)), true));
////        _manager->addAtion(_manager->getActionFactory()->manualAbsoluteTurnMoveAction(autoMirror(RPoint(0, 0, -pi/4)).getTheta()));
////        _manager->addAtion(_manager->getActionFactory()->clawAction(Comm::RELEASE));
////        _manager->addAtion(_manager->getActionFactory()->manualBackwardMoveAction(200));

////        scan4 = -1;
////        qDebug() << "SCAN RESULTS" << _queenKingPositions[0]  << _queenKingPositions[1]  << _queenKingPositions[2]  << _queenKingPositions[3] ;

////        int firstPawnIndex = _queenKingPositions.indexOf(false) + 1;
////        if (firstPawnIndex == 0) //Not found
////            firstPawnIndex = 1;
////        qDebug() << "First pawn detected in" << firstPawnIndex;
////        _manager->addAtion(takePawnInDispensingArea(firstPawnIndex, false));



////        int firstQueenOrKing = _queenKingPositions.indexOf(true) + 1;
////        if (firstQueenOrKing == 0) //No found
////            firstQueenOrKing = firstPawnIndex > 1 ? 1 : 2;
////        RPoint firstQueenOrKingPosition = getDispansingAreaPosition(firstQueenOrKing, false);
////        RPoint firstQueenOrKingPositionMinus200 = firstQueenOrKingPosition - RPoint(200, 0);

////        qDebug() << "First king or queen detected in" << firstQueenOrKing;

////        _manager->addAtion(_manager->getActionFactory()->moveAction(autoMirror(firstQueenOrKingPositionMinus200), 0.0, true));
////        _manager->addAtion(_manager->getActionFactory()->manualAbsoluteTurnMoveAction(pi));

////        _manager->addAtion(_manager->getActionFactory()->clawAction(Comm::RELEASE));
////        _manager->addAtion(_manager->getActionFactory()->manualBackwardMoveAction(200));
////        //_manager->addAtion(_manager->getActionFactory()->manualMoveToPointAction(autoMirror(firstQueenOrKingPosition)));
////        _manager->addAtion(takeQueenOrKingInDispensingArea(firstQueenOrKing, false));
////        _manager->addAtion(_manager->getActionFactory()->manualAbsoluteTurnMoveAction(pi));
////        _manager->addAtion(_manager->getActionFactory()->clawAction(Comm::TAKE_PAWN, 300));

////        //Build tower

////        _manager->addAtion(_manager->getActionFactory()->moveAction(autoMirror(RPoint(1550, 1700)), 0.0, true));
////        _manager->addAtion(_manager->getActionFactory()->clawAction(Comm::RAISE, 1));
////        _manager->addAtion(_manager->getActionFactory()->manualTurnToAction(autoMirror(RPoint(1750, 1500))));
////        _manager->addAtion(_manager->getActionFactory()->manualMoveToPointAction(autoMirror(RPoint(1750, 1500)), true));
////        _manager->addAtion(_manager->getActionFactory()->clawAction(Comm::RELEASE));
////        _manager->addAtion(_manager->getActionFactory()->manualBackwardMoveAction(200));




////    }
//}
