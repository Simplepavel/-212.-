#include "Shop.hpp"

Shop::Shop() : Deck()
{
    for (uint8_t i = 1; i < 5; ++i)
    {
        for (uint8_t j = 6; j < 15; ++j)
        {
            add_card(Card(static_cast<Suits>(i), static_cast<Ranks>(j), false));
        }
    }
    shuffle();
    Card trump = get_trump();
    set_trump(trump.suit);
}

Card Shop::get_top()
{
    Card result = get_card(count - 1);
    remove_card(count - 1);
    return result;
}

Card Shop::get_trump()
{
    Card result = get_card(23);
    deck.erase(deck.begin() + 23);
    deck.insert(deck.begin(), result);
    return result;
}

void Shop::set_trump(Suits trump_suit)
{
    for (auto i = deck.begin(); i != deck.end(); ++i)
    {
        bool is_trump = (i->suit == trump_suit);
        i->is_trump = is_trump;
    }
}
Deck *Shop::create_empty()
{
    return new Shop();
}
