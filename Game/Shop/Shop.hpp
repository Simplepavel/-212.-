#include "../Deck/Deck.hpp"
class Shop : public Deck
{
public:
    Shop();
    Card get_top();
    Card get_trump();
    Deck *create_empty() override;
};
