#include <iostream>
enum Ranks
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
enum Suits
{
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES
}; // Червы, Бубны, Трефы, Пики

struct Card
{
    Suits suit;
    Ranks rank;
    Card() {};
    Card(Suits s, Ranks r) : suit(s), rank(r) {}
};

#ifdef OPERATOR
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
#endif