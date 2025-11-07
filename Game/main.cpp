#include "Player/Player.hpp"
int main()
{
    Shop s1;
    Player pl1(s1, "User3289074239");
    Card new_card(DIAMONDS, SEVEN);
    pl1.add_card(new_card);
    std::cout << pl1 << '\n';
    char *pl1_serialize = pl1.serialize();
    Player pl2 = Player::deserialize(pl1_serialize);
    std::cout << pl2 << '\n';
    return 0;
}
