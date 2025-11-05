#include "Shop.hpp"

Shop::Shop() : Deck()
{
    for (uint8_t i = 0; i < 4; ++i)
    {
        for (uint8_t j = 6; j < 15; ++j)
        {
            add_card(Card(static_cast<Suits>(i), static_cast<Ranks>(j)));
        }
    }
    shuffle();
}

Card Shop::get_top()
{
    Card result = get_card(count - 1);
    remove_card(count - 1);
    return result;
}

Card Shop::get_trump()
{
    Card result = get_card(count - 1);
    deck.insert(deck.begin(), result);
    return result;
}

// std::ostream &operator<<(std::ostream &cout, const Shop &shop)
// {
//     cout << shop;
//     return cout;
// }