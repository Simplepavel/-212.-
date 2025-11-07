#include <vector>
#include <stack>
#include "../Card/Card.hpp"
#include <iostream>
class Deck
{
protected:
    std::vector<Card> deck;
    unsigned int count;

public:
    Deck();
    void add_card(const Card &new_vl_);
    const Card &get_card(unsigned int idx) const;
    void remove_card(unsigned int idx);

    virtual Deck *create_empty();

    unsigned int get_count() const { return count; }
    void shuffle();
    void set_trumps(Suits trump_suit);
    char *serialize();
    static Deck *deserialize(char *buffer);
};

std::ostream &operator<<(std::ostream &cout, const Deck &deck);