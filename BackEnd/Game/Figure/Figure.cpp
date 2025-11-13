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

std::ostream &operator<<(std::ostream &cout, const Figure &fig)
{
    if (fig.is_valid())
    {
        char ans[3]{};
        switch (fig.get_name())
        {
        case KING:
            ans[0] = 'K';
            break;
        case QUEEN:
            ans[0] = 'Q';
            break;
        case ROOK:
            ans[0] = 'R';
            break;
        case BISHOP:
            ans[0] = 'B';
            break;
        case KNIGHT:
            ans[0] = 'N';
            break;
        case PAWN:
            ans[0] = 'P';
            break;
        default:
            ans[0] = ' ';
            break;
        }
        ans[1] = fig.is_white() ? '0' : '1';
        cout << ans;
    }
    else
    {
        cout << "XX";
    }
    return cout;
}