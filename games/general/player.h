#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include <vector>

class Player
{
private:
    std::vector<Card> cards;

public:
    Player();

    void addCard(const Card &card);
    void removeCard(const Card &card);
    bool hasCard(const Card &card) const;
    std::vector<Card> getCards() const;
    void sortCards();
    int cardCount() const;
    void clear();
};

#endif
