#include <vector>
#include <stack>
#include "Card.hpp"
class Deck
{
private:
    std::vector<Card> main;
    std::stack<Card> deck;
    Card trump;
    void shuffle();

public:
    Deck();
    Card get_card();
    Card get_trump();
    void set_trump();
};
