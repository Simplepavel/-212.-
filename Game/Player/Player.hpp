#include "../Shop/Shop.hpp"

class Player
{
private:
    Deck deck;
    std::string username;

public:
    Player();
    Player(const Player &);
    Player(const Player &&);
    Player &operator=(const Player &);
    Player &operator=(const Player &&);
    Player(Shop &, const std::string &); // длину имени обязательно ограничить 100 символами
    Player(const Deck &d, const std::string &n);
    
    Card GetLowestTrump();
    void add_card(const Card &);
    char *serialize();
    static Player deserialize(char *);
    void self_deserialize(char *);
    uint8_t GetTotalByteSize();
    friend std::ostream &operator<<(std::ostream &cout, const Player &pl);
};

std::ostream &operator<<(std::ostream &cout, const Player &pl);
