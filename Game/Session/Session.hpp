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
    Session();
    
    Session(const Session &);
    Session(const Session &&);

    Session &operator=(const Session &);
    Session &operator=(const Session &&);

    Session(const std::string &, const std::string &);
    Session(const Shop &, const Player &, const Player &, const Card &, bool);
    char *serialize();
    static Session deserialize(char *buffer);
    void self_deserialize(char *buffer);
    void show_state();
};