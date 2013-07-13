#include "ChessUpConfiguration.h"
#include <QTime>

ChessUpConfiguration::ChessUpConfiguration(bool random)
{
    initCards();

    if (random)
	drawCards();
}

ChessUpConfiguration::ChessUpConfiguration(const ChessUpConfigurationCard& card1, const ChessUpConfigurationCard& card2, const ChessUpConfigurationCard& card3)
{
    initCards();
    drawCards(card1, card2, card3);
}

const QList<PawnObject *> ChessUpConfiguration::getPawns() const
{
    return _pawns;
}

void ChessUpConfiguration::drawCards()
{
    QList<ChessUpConfigurationCard> cards(_cards);  // copy the card list as we are going to remove cards

    _pawns.clear();
    _queens.clear();
    _kings.clear();

    //get 3 random card
    qsrand(QTime::currentTime().second());
    ChessUpConfigurationCard card1 = cards.takeAt(qrand() % cards.size());   // dispensing area configuration
    ChessUpConfigurationCard card2 = cards.takeAt(qrand() % cards.size());   // line 1 configuration
    ChessUpConfigurationCard card3 = cards.takeAt(qrand() % cards.size());   // line 2 configuration

    drawCards(card1, card2, card3);
}

void ChessUpConfiguration::drawCards(const ChessUpConfigurationCard& card1, const ChessUpConfigurationCard& card2, const ChessUpConfigurationCard& card3)
{
    //populate the lists
    _pawns.append(new PawnObject(QPointF(1050,1500)));

    if (card1.kingPosition > 0)
    {
	_kings.append(new KingObject(QPointF(410 + card1.kingPosition * 280, randomPosition(true))));
	_kings.append(new KingObject(QPointF(410 + card1.kingPosition * 280, randomPosition(false))));
    }
    if (card1.queenPosition > 0)
    {
	_queens.append(new QueenObject(QPointF(410 + card1.queenPosition * 280, randomPosition(true))));
	_queens.append(new QueenObject(QPointF(410 + card1.queenPosition * 280, randomPosition(false))));
    }

    for(int i = 1; i <= 5; i++)
        if (card1.kingPosition != i && card1.queenPosition != i)
        {
	    _pawns.append(new PawnObject(QPointF(410 + i * 280, randomPosition(true))));
	    _pawns.append(new PawnObject(QPointF(410 + i * 280, randomPosition(false))));
        };


    if (card2.kingPosition > 0)
    {
	_pawns.append(new PawnObject(QPointF(350 * card2.kingPosition,  800)));
	_pawns.append(new PawnObject(QPointF(350 * card2.kingPosition, 2200)));
    }
    if (card2.kingPosition > 0)
    {
	_pawns.append(new PawnObject(QPointF(350 * card2.queenPosition, 800)));
	_pawns.append(new PawnObject(QPointF(350 * card2.queenPosition,2200)));
    }

    if (card3.kingPosition > 0)
    {
	_pawns.append(new PawnObject(QPointF(350 * card3.kingPosition, 1150)));
	_pawns.append(new PawnObject(QPointF(350 * card3.kingPosition, 1850)));
    }

    if (card3.queenPosition > 0)
    {
	_pawns.append(new PawnObject(QPointF(350 * card3.queenPosition,1150)));
	_pawns.append(new PawnObject(QPointF(350 * card3.queenPosition,1850)));
    }
}

double ChessUpConfiguration::randomPosition(bool redSide)
{
    double res = qrand() % 200 + 100;

    if (!redSide)
	res = 3000 - res;

    return res;
}

ChessUpConfiguration::ChessUpConfigurationCard::ChessUpConfigurationCard(int kingPos, int queenPos)
{
    kingPosition = kingPos;
    queenPosition = queenPos;
}

const QList<QueenObject *> ChessUpConfiguration::getQueens() const
{
    return _queens;
}

const QList<KingObject *> ChessUpConfiguration::getKings() const
{
    return _kings;
}

void ChessUpConfiguration::initCards()
{
    _cards << ChessUpConfigurationCard(1, 2) << ChessUpConfigurationCard(1, 3) << ChessUpConfigurationCard(1, 4) << ChessUpConfigurationCard(1, 5)
	   << ChessUpConfigurationCard(2, 1) << ChessUpConfigurationCard(2, 3) << ChessUpConfigurationCard(2, 4) << ChessUpConfigurationCard(2, 5)
	   << ChessUpConfigurationCard(3, 1) << ChessUpConfigurationCard(3, 2) << ChessUpConfigurationCard(3, 4) << ChessUpConfigurationCard(3, 5)
	   << ChessUpConfigurationCard(4, 1) << ChessUpConfigurationCard(4, 2) << ChessUpConfigurationCard(4, 3) << ChessUpConfigurationCard(4, 5)
	   << ChessUpConfigurationCard(5, 1) << ChessUpConfigurationCard(5, 2) << ChessUpConfigurationCard(5, 3) << ChessUpConfigurationCard(5, 4);
}

void ChessUpConfiguration::deleteConfigurationObjects()
{
    qDeleteAll(_pawns);
    _pawns.clear();

    qDeleteAll(_queens);
    _queens.clear();

    qDeleteAll(_kings);
    _kings.clear();
}
