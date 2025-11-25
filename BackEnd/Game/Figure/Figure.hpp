#pragma once
#include <cstdint>
#include <memory>
#include <iostream>
enum FigureName : uint8_t
{
    KING,   // Король
    QUEEN,  // Ферщь
    ROOK,   // Ладья
    BISHOP, // Bishop
    KNIGHT, // Конь
    PAWN,   // Пешка
    EMPTY,
};

enum FigureColor : uint8_t
{
    BLACK = 0,
    WHITE = 6,
    NO_COLOR,
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
    uint8_t GetImgNum() const; // Порядковый номер готового изображения
};
