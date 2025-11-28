#include <cstdint>
#include "../Game/Board/Board.hpp"
#include <stack>
#include <winsock2.h>
struct Player
{
    uint32_t id;
    uint32_t fd;
    Player(SOCKET i, SOCKET f) : id(i), fd(f) {}
    Player() : id(0), fd(0) {}
    bool operator==(const Player &);
};

struct Session
{
    uint32_t id;
    Player pl1;
    Player pl2;
    static uint32_t last_id;
    static std::stack<uint32_t> ids;
    bool IsFinished = false; // Закончена ли эта партия победой или ничьей(пат)
    Session(const Player &p1, const Player &p2);
    ~Session();
    const Player &Sender(int fd);
    const Player &Reciver(int fd);
};