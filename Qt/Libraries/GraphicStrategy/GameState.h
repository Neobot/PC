#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QPointF>
#include <QHash>
#include <QVariant>

namespace Tools
{
    class NGridNode;
}

class GameState
{
public:
    GameState();

    int _remainingTime;
    Tools::NGridNode* _robotposition;
    QPointF _opponentPosition;

    QHash<QString, QVariant> _content;
};

#endif // GAMESTATE_H
