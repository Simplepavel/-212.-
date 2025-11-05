#include "Session.hpp"

Session::Session() : pl1(shop), pl2(shop), stol()
{
    trump = shop.get_trump();
    pl1.set_trumps(trump.suit);
    pl2.set_trumps(trump.suit);
    shop.set_trumps(trump.suit);

    Card lowest1 = pl1.GetLowestTrump();
    Card lowest2 = pl2.GetLowestTrump();
    if (lowest1.is_valid() && lowest2.is_valid())
    {
        pl1_attack = lowest1 < lowest2;
    }
    if (lowest1.is_valid() && !lowest2.is_valid())
    {
        pl1_attack = true;
    }
    if (!lowest1.is_valid() && lowest2.is_valid())
    {
        pl1_attack = false;
    }
    if (!lowest1.is_valid() && !lowest2.is_valid())
    {
        pl1_attack = (rand() % 2 == 0);
    }
}