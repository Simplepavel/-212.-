#include "../Deck/Deck.hpp"
class Shop : public Deck
{
private:
    Card get_trump();
    void set_trump(Suits trump);

public:
    Shop();
    Card get_top();
    Deck *create_empty() override;
};
