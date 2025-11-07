#include "Session.hpp"

Session::Session(const std::string &n1, const std::string &n2) : pl1(shop, n1), pl2(shop, n2)
{
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

    std::cout << "==============Start Game==================\n";
    std::cout << pl1 << '\n';
    std::cout << pl2 << '\n';
    std::cout << "Shop: " << shop << '\n';
    std::cout << ((pl1_attack) ? ("Player1 is attaker") : ("Player2 is attacker")) << '\n';
}