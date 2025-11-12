// #include "Shop/Shop.hpp"

#include "Session/Session.hpp"
int main()
{
    Session s1("Markus", "Sven");
    s1.show_state();
    Session s2;
    std::cout << "\n\n";
    s2.show_state();

    std::cout << "\n\n";
    char *s1_buffer = s1.serialize();

    s2.self_deserialize(s1_buffer);

    s1.show_state();
    std::cout << "\n\n";
    s2.show_state();
    return 0;
}
