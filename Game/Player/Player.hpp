#include "../Shop/Shop.hpp"

class Player
{
private:
    Deck deck;
    std::string username;

public:
    Player(Shop &, const std::string &);
    Player(const Deck &d, const std::string &n);
    Card GetLowestTrump();
    void add_card(const Card &);
    char *serialize();
    static Player deserialize(char *);
    // void set_trumps(Suits);
    friend std::ostream &operator<<(std::ostream &cout, const Player &pl);
};

std::ostream &operator<<(std::ostream &cout, const Player &pl);
