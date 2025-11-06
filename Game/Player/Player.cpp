#include "Player.hpp"

Player::Player(Shop &shop)
{
    for (int i = 0; i < 6; ++i)
    {
        add_card(shop.get_top());
    }
}
Card Player::GetLowestTrump()
{
    Card min_trump;
    for (int i = 0; i < count; ++i)
    {
        if (deck[i].is_trump && !min_trump.is_valid())
        {
            min_trump = deck[i];
        }
        if (deck[i].is_trump && min_trump.is_valid())
        {
            if (deck[i] < min_trump)
            {
                min_trump = deck[i];
            }
        }
    }
    return min_trump;
}
