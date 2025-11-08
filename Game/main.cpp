// #include "Shop/Shop.hpp"
#include "Session/Session.hpp"
// #include "Player/Player.hpp"
int main()
{
    // Shop sh1;
    // Player pl1(sh1, "Name1");

    // char* pl1_serialize = pl1.serialize();

    // Player pl1_copy = Player::deserialize(pl1_serialize);
    // std::cout << pl1_copy;

    Session s1("User1", "User2");
    char * p = s1.serialize();
    std::cout << "\n\n";
    Session::deserialize(p);
    return 0;
}
