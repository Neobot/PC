#ifndef CHESSUPOBJECT_H
#define CHESSUPOBJECT_H

#include "DataObject.h"

class ChessUpObject : public Tools::CircleObject
{
public:
    ChessUpObject(const QPointF position, double height);

    virtual int getPoints() const {return 0;}

    void setLevel(int level);
    int getLevel() const; //level of the object in a tower

    double getHeight() const;
    void take();
    void drop();

private:
    int _level;
    double _height;
};


class PawnObject : public ChessUpObject
{
public:
    PawnObject(const QPointF& position);
    int getPoints() const;
};

class QueenObject : public ChessUpObject
{
public:
    QueenObject(const QPointF& position);
    int getPoints() const;
};


class KingObject : public ChessUpObject
{
public:
    KingObject(const QPointF& position);
    int getPoints() const;
};


#endif // CHESSUPOBJECT_H
