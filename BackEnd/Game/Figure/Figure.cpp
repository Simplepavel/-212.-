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

uint8_t Figure::GetImgNum() const
{
    if (is_valid())
    {
        return color + name;
    }
    return 255;
}