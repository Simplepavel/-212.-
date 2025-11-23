#include <vector>
#include "../Figure/Figure.hpp"
class Board
{
private:
    std::vector<Figure> self;
    std::vector<uint8_t> LastMoves; // последний ход

public:
    Board(FigureColor);
    char *serialize();
    void deserialize(char *);
    uint16_t capacity() { return 128; }
    void show() const;
    Figure &operator[](int idx);
    void replace(int c_row, int c_column, int l_row, int l_column);
    void replace();
    char *SerializeMove();
    void DeserializeMove(char *);
};
