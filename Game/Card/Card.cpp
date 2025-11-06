#include "Card.hpp"

char *Card::serialize()
{
    char *buffer = new char[3];
    memcpy(buffer, &suit, 1);
    memcpy(buffer + 1, &rank, 1);
    buffer[2] = is_trump ? 1 : 0;
    return buffer;
}

Card Card::deserialize(char *buffer)
{
    Card result;
    memcpy(&result.suit, buffer, 1);
    memcpy(&result.rank, buffer + 1, 1);
    result.is_trump = buffer[2] == 1;
    return result;
}

bool Card::operator==(const Card &argv)
{
    return (suit == argv.suit) && (rank == argv.rank);
}

bool Card::operator<(const Card &argv)
{

    if (!is_trump)
    {
        if (argv.is_trump)
            return true;
        if (suit != argv.suit)
            return false;
        if (rank < argv.rank)
            return true;
        if (rank > argv.rank)
            return false;
    }
    if (argv.is_trump)
    {
        if (rank < argv.rank)
            return true;
        else
            return false;
    }
    return false;
}

bool Card::is_valid()
{
    return suit != SUIT && rank != RANK;
}

std::ostream &operator<<(std::ostream &cout, const Card &card)
{
    switch (card.suit)
    {
    case (HEARTS):
        cout << "H";
        break;
    case (DIAMONDS):
        cout << "D";
        break;
    case (CLUBS):
        cout << "C";
        break;
    case (SPADES):
        cout << "S";
        break;
    }

    switch (card.rank)
    {
    case (JACK):
        cout << 'J';
        break;
    case (QUEEN):
        cout << 'Q';
        break;
    case (KING):
        cout << 'K';
        break;
    case (ACE):
        cout << 'A';
        break;
    default:
        cout << static_cast<uint32_t>(card.rank);
    }
    return cout;
}