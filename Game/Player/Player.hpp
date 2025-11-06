#include "../Shop/Shop.hpp"

class Player : public Deck
{
public:
    Player(Shop &);
    Card GetLowestTrump();
};
