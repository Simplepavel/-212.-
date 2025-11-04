#include "Card.hpp"

void Card::serialize(char *buffer)
{
    memcpy(buffer, &suit, 1);
    memcpy(buffer + 1, &rank, 1);
}

Card Card::deserialize(char *buffer)
{
    Card result;
    memcpy(&result.suit, buffer, 1);
    memcpy(&result.rank, buffer + 1, 1);
    return result;
}

std::ostream &operator<<(std::ostream &cout, const Card &card)
{
    switch (card.suit)
    {
    case (HEARTS):
        cout << "♥";
        break;
    case (DIAMONDS):
        cout << "♦";
        break;
    case (CLUBS):
        cout << "♣";
        break;
    case (SPADES):
        cout << "♠";
        break;
    }

    switch (card.rank)
    {
    case (JACK):
        std::cout << 'J';
        break;
    case (QUEEN):
        std::cout << 'Q';
        break;
    case (KING):
        std::cout << 'K';
        break;
    case (ACE):
        std::cout << 'A';
        break;
    default:
        std::cout << card.rank;
    }
    return cout;
}