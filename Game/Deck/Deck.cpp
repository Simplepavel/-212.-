#include "Deck.hpp"

Deck::Deck() : count(0) {}

void Deck::add_card(const Card &new_vl_)
{
    deck.push_back(new_vl_);
    ++count;
}

const Card &Deck::get_card(unsigned int idx) const
{
    return deck[idx];
}

void Deck::remove_card(unsigned int idx)
{
    if (idx >= 0 && idx < count)
    {
        deck.erase(deck.begin() + idx);
        --count;
    }
}

void Deck::shuffle()
{
    srand(time(nullptr));
    for (int i = 0; i < 100; ++i)
    {
        int idx1 = rand() % count;
        int idx2 = rand() % count;
        std::swap(deck[idx1], deck[idx2]);
    }
}

std::ostream &operator<<(std::ostream &cout, const Deck &deck)
{
    for (int i = 0; i < deck.get_count(); ++i)
    {
        cout << deck.get_card(i) << ' ';
    }
    return cout;
}

void Deck::set_trumps(Suits trump_suit)
{
    for (int i = 0; i < count; ++i)
    {
        deck[i].is_trump = (deck[i].suit == trump_suit);
    }
}