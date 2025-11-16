#include <cstdint>
#include <memory>
#include <iostream>
enum FigureName : uint8_t
{
    EMPTY,
    KING,   // Король
    QUEEN,  // Ферщь
    ROOK,   // Ладья
    BISHOP, // Bishop
    KNIGHT, // Конь
    PAWN,   // Пешка
};

class Figure
{
private:
    FigureName name;
    bool white;

public:
    Figure();
    Figure(FigureName _name, bool _is_white);
    FigureName get_name() const { return name; }
    bool is_white() const { return white; }
    bool is_valid() const { return !(name == EMPTY); }
    uint8_t capacity() { return 2; }
    char *serialize();
    void deserialize(char *buffer);
    std::string to_string() const;
};
