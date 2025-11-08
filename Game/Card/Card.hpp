#include <iostream>
#include <winsock2.h>
#include <cstdint>
enum Ranks : uint8_t
{
    RANK,
    SIX = 6,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    ACE,
};
enum Suits : uint8_t
{
    SUIT,
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES,
}; // Червы, Бубны, Трефы, Пики

struct Card
{
    Suits suit;
    Ranks rank;
    bool is_trump = false;
    Card() : suit(SUIT), rank(RANK) {};
    Card(Suits s, Ranks r, bool it) : suit(s), rank(r), is_trump(it) {}
    char *serialize();
    static Card deserialize(char *buffer);
    bool is_valid();
    bool operator==(const Card &argv);
    bool operator<(const Card &argv);
    uint8_t GetTotalByteSize();
};

std::ostream &operator<<(std::ostream &cout, const Card &card);