#ifndef CHESSUPCONFIGURATION_H
#define CHESSUPCONFIGURATION_H

#include "ChessUpObject.h"

class ChessUpConfiguration
{
public:
    struct ChessUpConfigurationCard
    {
	ChessUpConfigurationCard(int kingPos, int queenPos);

	int kingPosition; //should be between 1 and 5. For card 2 and 3, 0 means that the object is not placed on the table
	int queenPosition; //should be between 1 and 5. Forcard 2 and 3, 0 means that the object is not placed on the table
    };

    /**
      * \brief Creates a random or an empty configuration.
      */
    ChessUpConfiguration(bool random = false);

    /**
      * \brief Creates a configuration from three cards.
      */
    ChessUpConfiguration(const ChessUpConfigurationCard& card1, const ChessUpConfigurationCard& card2, const ChessUpConfigurationCard& card3);

    const QList<PawnObject*> getPawns() const;
    const QList<QueenObject*> getQueens() const;
    const QList<KingObject*> getKings() const;

    /**
      * \brief This class doesn't manage itself the destruction of the object it creates.
      * This method delete every object contained in this configuration.
      */
    void deleteConfigurationObjects();

private:

    void initCards();
    void drawCards();
    void drawCards(const ChessUpConfigurationCard& card1, const ChessUpConfigurationCard& card2, const ChessUpConfigurationCard& card3);
    double randomPosition(bool redSide);

    QList<PawnObject*> _pawns;
    QList<QueenObject*> _queens;
    QList<KingObject*> _kings;

    QList<ChessUpConfigurationCard> _cards;
};

#endif // CHESSUPCONFIGURATION_H
