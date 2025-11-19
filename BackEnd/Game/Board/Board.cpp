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
    self[7 * 8 + 0] = Figure(FigureName::ROOK, FigureColor::BLACK);   // a8
    self[7 * 8 + 1] = Figure(FigureName::KNIGHT, FigureColor::BLACK); // b8
    self[7 * 8 + 2] = Figure(FigureName::BISHOP, FigureColor::BLACK); // c8
    self[7 * 8 + 3] = Figure(FigureName::QUEEN, FigureColor::BLACK);  // d8
    self[7 * 8 + 4] = Figure(FigureName::KING, FigureColor::BLACK);   // e8
    self[7 * 8 + 5] = Figure(FigureName::BISHOP, FigureColor::BLACK); // f8
    self[7 * 8 + 6] = Figure(FigureName::KNIGHT, FigureColor::BLACK); // g8
    self[7 * 8 + 7] = Figure(FigureName::ROOK, FigureColor::BLACK);   // h8

    // Черные пешки
    for (int i = 0; i < 8; ++i)
    {
        self[6 * 8 + i] = Figure(FigureName::PAWN, FigureColor::BLACK); // линия 7
    }

    // БЕЛЫЕ фигуры
    self[0 * 8 + 0] = Figure(FigureName::ROOK, FigureColor::WHITE);   // a1
    self[0 * 8 + 1] = Figure(FigureName::KNIGHT, FigureColor::WHITE); // b1
    self[0 * 8 + 2] = Figure(FigureName::BISHOP, FigureColor::WHITE); // c1
    self[0 * 8 + 3] = Figure(FigureName::QUEEN, FigureColor::WHITE);  // d1
    self[0 * 8 + 4] = Figure(FigureName::KING, FigureColor::WHITE);   // e1
    self[0 * 8 + 5] = Figure(FigureName::BISHOP, FigureColor::WHITE); // f1
    self[0 * 8 + 6] = Figure(FigureName::KNIGHT, FigureColor::WHITE); // g1
    self[0 * 8 + 7] = Figure(FigureName::ROOK, FigureColor::WHITE);   // h1

    // Белые пешки
    for (int i = 0; i < 8; ++i)
    {
        self[1 * 8 + i] = Figure(FigureName::PAWN, FigureColor::WHITE); // линия 2
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

Figure &Board::operator[](int idx)
{
    return self[idx];
}

void Board::replace(int c_row, int c_column, int l_row, int l_column)
{
    int idx1 = c_row * 8 + c_column;
    int idx2 = l_row * 8 + l_column;
    self[idx2] = self[idx1];
    self[idx1] = Figure();
}