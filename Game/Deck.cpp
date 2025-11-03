#define OPERATOR
#include "Deck.hpp"
#undef OPERATOR

Deck::Deck()
{
    main.reserve(36);
    for (int i = SIX; i < ACE + 1; ++i)
    {
        for (int j = HEARTS; j < SPADES + 1; ++j)
        {
            main.emplace_back(static_cast<Suits>(j), static_cast<Ranks>(i));
        }
    }
    shuffle();
    for (int i = 0; i < 36; ++i)
    {
        std::cout << deck.top() << ' ';
        deck.pop();
    }
}

void Deck::shuffle()
{
    srand(time(nullptr));
    for (int i = 0; i < 100; ++i)
    {
        int idx1 = rand() % 36;
        int idx2 = rand() % 36;
        std::swap(main[idx1], main[idx2]);
    }
    for (auto i = main.begin(); i != main.end(); ++i)
    {
        deck.push(*i);
    }
}

Card Deck::get_card()
{
    Card res = deck.top();
    deck.pop();
    return res;
}

Card Deck::get_trump()
{
    return trump;
}

void Deck::set_trump()
{
    trump = deck.top();
}