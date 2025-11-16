#include "Board.hpp"

Board::Board()
{
    self.resize(64);

    // Инициализируем все клетки пустыми фигурами
    for (int i = 0; i < 64; ++i)
    {
        self[i] = Figure();
    }

    // ЧЕРНЫЕ фигуры
    self[7 * 8 + 0] = Figure(FigureName::ROOK, false);   // a8
    self[7 * 8 + 1] = Figure(FigureName::KNIGHT, false); // b8
    self[7 * 8 + 2] = Figure(FigureName::BISHOP, false); // c8
    self[7 * 8 + 3] = Figure(FigureName::QUEEN, false);  // d8
    self[7 * 8 + 4] = Figure(FigureName::KING, false);   // e8
    self[7 * 8 + 5] = Figure(FigureName::BISHOP, false); // f8
    self[7 * 8 + 6] = Figure(FigureName::KNIGHT, false); // g8
    self[7 * 8 + 7] = Figure(FigureName::ROOK, false);   // h8

    // Черные пешки
    for (int i = 0; i < 8; ++i)
    {
        self[6 * 8 + i] = Figure(FigureName::PAWN, false); // линия 7
    }

    // БЕЛЫЕ фигуры
    self[0 * 8 + 0] = Figure(FigureName::ROOK, true);   // a1
    self[0 * 8 + 1] = Figure(FigureName::KNIGHT, true); // b1
    self[0 * 8 + 2] = Figure(FigureName::BISHOP, true); // c1
    self[0 * 8 + 3] = Figure(FigureName::QUEEN, true);  // d1
    self[0 * 8 + 4] = Figure(FigureName::KING, true);   // e1
    self[0 * 8 + 5] = Figure(FigureName::BISHOP, true); // f1
    self[0 * 8 + 6] = Figure(FigureName::KNIGHT, true); // g1
    self[0 * 8 + 7] = Figure(FigureName::ROOK, true);   // h1

    // Белые пешки
    for (int i = 0; i < 8; ++i)
    {
        self[1 * 8 + i] = Figure(FigureName::PAWN, true); // линия 2
    }
}

char *Board::serialize()
{
    char *result = new char[capacity()];
    for (int i = 0; i < 64; ++i)
    {
        char *figure_serialize = self[i].serialize();
        memcpy(result + i * 2, figure_serialize, 2);
        delete[] figure_serialize;
    }
    return result;
}

void Board::deserialize(char *buffer)
{
    Figure figure;
    for (int i = 0; i < 64; ++i)
    {
        figure.deserialize(buffer + i * 2);
        self[i] = figure;
    }
}

void Board::show() const
{
    for (int i = 0; i < 64; ++i)
    {
        Figure alfa = self[i];
        std::cout << alfa.to_string();
        if (i % 8 == 7)
        {
            std::cout << '\n';
        }
        else
        {
            std::cout << ' ';
        }
    }
}

const Figure &Board::operator[](int idx) const
{
    return self[idx];
}