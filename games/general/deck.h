#ifndef DECK_H
#define DECK_H

#include "card.h"
#include <vector>

class Deck
{
private:
    std::vector<Card> cards;

public:
    Deck();
    void shuffle();
    Card drawCard();
    bool isEmpty() const;
    int cardsLeft() const;
    void reset();
};

#endif
