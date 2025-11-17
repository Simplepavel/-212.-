#pragma once
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

enum FigureColor : uint8_t
{
    NO_COLOR,
    BLACK,
    WHITE,
};

class Figure
{
private:
    FigureName name;
    FigureColor color;

public:
    Figure();
    Figure(FigureName _name, FigureColor _color);
    FigureName get_name() const { return name; }
    FigureColor get_color() const { return color; }
    bool is_valid() const { return !(name == EMPTY); }
    uint8_t capacity() { return 2; }
    char *serialize();
    void deserialize(char *buffer);
    std::string to_string() const;
    bool IsValidMove(int current_row, int current_column, int last_row, int last_column) const;
};
