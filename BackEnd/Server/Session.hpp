#include <cstdint>
#include "../Game/Board/Board.hpp"
#include <stack>
#include <winsock2.h>
#include <RatingSystem/rating.hpp>

struct Player
{
    uint32_t id;
    uint32_t fd;
    Rating player_rating;
    Player(SOCKET i, SOCKET f, int init_rating) : id(i), fd(f), player_rating(init_rating) {}
    Player() : id(0), fd(0), player_rating(1200) {}
    Player(SOCKET i, SOCKET f) : id(i), fd(f), player_rating(1200) {}
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
    Player &Sender(int fd);
    Player &Reciver(int fd);
};