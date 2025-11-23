#include "Figure.hpp"

Figure::Figure() : name(FigureName::EMPTY), color(FigureColor::NO_COLOR) {}

Figure::Figure(FigureName _name, FigureColor _color) : name(_name), color(_color) {}

char *Figure::serialize()
{
    char *result = new char[capacity()];
    memcpy(result, &name, 1);
    memcpy(result + 1, &color, 1);
    return result;
}

void Figure::deserialize(char *buffer)
{
    memcpy(&name, buffer, 1);
    memcpy(&color, buffer + 1, 1);
}

std::string Figure::to_string() const
{
    if (is_valid())
    {
        switch (get_name())
        {
        case (KING):
            return "K";
        case (QUEEN):
            return "Q";
        case (ROOK):
            return "R";
        case (BISHOP):
            return "B";
        case (KNIGHT):
            return "N";
        case (PAWN):
            return "P";
        }
    }
    return " ";
}

bool Figure::IsValidMove(int current_row, int current_column, int last_row, int last_column) const
{
    return true; // попросить кого то реализовать
}