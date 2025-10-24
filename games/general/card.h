#ifndef CARD_H
#define CARD_H

#include <string>

enum Suit
{
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES
};

enum Rank
{
    SIX = 6,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    ACE
};

class Card
{
private:
    Suit suit;
    Rank rank;

public:
    Card();
    Card(Suit suit, Rank rank);

    Suit getSuit() const;
    Rank getRank() const;
    std::string toString() const;
    std::string getImagePath() const;

    bool operator<(const Card &other) const;
    bool operator==(const Card &other) const;
};

#endif
