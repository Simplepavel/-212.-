#include <iostream>
#include <winsock2.h>
#include <cstdint>
enum Ranks : uint8_t
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
enum Suits : uint8_t
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
    void serialize(char *buffer);
    static Card deserialize(char *buffer);
};

std::ostream &operator<<(std::ostream &cout, const Card &card);