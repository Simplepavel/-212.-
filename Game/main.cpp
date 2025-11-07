#include "Session/Session.hpp"
int main()
{
    Shop s1;

    char *serialize_deck = s1.serialize();
    std::cout << "Before: " << s1 << '\n';
    return 0;
}
