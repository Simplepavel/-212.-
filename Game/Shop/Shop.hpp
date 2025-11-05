#include "../Deck/Deck.hpp"
class Shop: public Deck
{
public:
    Shop();
    Card get_top();
    Card get_trump();
    // friend std::ostream &operator<<(std::ostream &cout, const Shop &shop);
};

// std::ostream &operator<<(std::ostream &cout, const Shop &shop);