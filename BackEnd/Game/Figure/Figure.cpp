#include "Figure.hpp"

Figure::Figure() : name(FigureName::EMPTY), white(false) {}

Figure::Figure(FigureName _name, bool _is_white) : name(_name), white(_is_white) {}

char *Figure::serialize()
{
    char *result = new char[capacity()];
    memcpy(result, &name, 1);
    result[1] = (white) ? 1 : 0;
    return result;
}

void Figure::deserialize(char *buffer)
{
    memcpy(&name, buffer, 1);
    white = (buffer[1] == 1);
}

std::string Figure::to_string() const
{
    if (is_valid())
    {
        if (is_white())
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
        else
        {
            switch (get_name())
            {
            case (KING):
                return "k";
            case (QUEEN):
                return "q";
            case (ROOK):
                return "r";
            case (BISHOP):
                return "b";
            case (KNIGHT):
                return "n";
            case (PAWN):
                return "p";
            }
        }
    }
    return "";
}