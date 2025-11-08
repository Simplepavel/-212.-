#include "../Player/Player.hpp"
class Session
{
private:
    Shop shop;
    Player pl1;
    Player pl2;
    Card stol; // карта на столе в текущий момент времени
    bool pl1_attack;

public:
    Session(const std::string &, const std::string &);
    char *serialize();
    static Session deserialize(char *buffer); // Дописать десериализацию
};