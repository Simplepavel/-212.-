#include "../Player/Player.hpp"
class Session
{
private:
    Shop shop;
    Player pl1;
    Player pl2;
    Card stol;  // карта на столе в текущий момент времени
    Card trump; //  козырь
    bool pl1_attack;

public:
    Session();
};