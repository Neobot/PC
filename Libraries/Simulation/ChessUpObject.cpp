#include "ChessUpObject.h"
#include "vmath.h"

using namespace Tools;

ChessUpObject::ChessUpObject(const QPointF position, double height) :
    CircleObject(100, position, pi/2, ZRange(0, height)),
    _level(1),
    _height(height)
{}

void ChessUpObject::setLevel(int level)
{
    _level = level;
}

int ChessUpObject::getLevel() const
{
    return _level;
}

double ChessUpObject::getHeight() const
{
    return _height;
}

void ChessUpObject::take()
{
    _collideable = false;
}

void ChessUpObject::drop()
{
    _collideable = true;
}

//----------------------------------------------

PawnObject::PawnObject(const QPointF &position) : ChessUpObject(position, 50)
{
}

int PawnObject::getPoints() const
{
    int l = getLevel();
    return l >= 1 && l <= 3 ? 10 : 0;
}

//----------------------------------------------

QueenObject::QueenObject(const QPointF &position) : ChessUpObject(position, 230)
{
}

int QueenObject::getPoints() const
{
    int l = getLevel();
    return l >= 1 && l <= 3 ? 20 * l : 0;
}


//----------------------------------------------

KingObject::KingObject(const QPointF &position) : ChessUpObject(position, 230)
{
}

int KingObject::getPoints() const
{
    int l = getLevel();
    return l >= 1 && l <= 3 ? 30 * l : 0;
}



