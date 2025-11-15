#include <cstdint>
#include "../Game/Board/Board.hpp"
#include <stack>
struct Player
{
    uint32_t id;
    uint32_t fd;
    Player(uint32_t i, uint32_t f) : id(i), fd(f) {}
    Player() : id(0), fd(0) {}
};

struct Session
{
    uint32_t id;
    Player pl1;
    Player pl2;
    Board board;
    static uint32_t last_id;
    static std::stack<uint32_t> ids;
    Session(const Player &p1, const Player &p2);
    ~Session();
};
