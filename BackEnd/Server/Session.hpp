#include <cstdint>
#include "../Game/Board/Board.hpp"
struct Player
{
    uint32_t id;
    uint32_t fd;
};

struct Session
{
    uint32_t id;
    Player pl1;
    Player pl2;
    Board board;
};
