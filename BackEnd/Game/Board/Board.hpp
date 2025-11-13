#include <vector>
#include "Figure.hpp"
class Board
{
private:
    std::vector<Figure> self;

public:
    Board();
    char *serialize();
    void deserialize(char *);
    uint16_t capacity() { return 128; }
    void show() const;
};
