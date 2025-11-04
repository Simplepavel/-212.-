#include <cstdint>
#include "../../Game/Card.hpp"
enum MOVES : uint8_t
{
    ATTACK,  // Атаковать картой
    DEFEND,  // Защититься картой
    TAKE,    // Взять карты (сдаться) или добрать карту
    END_TURN // Закончить ход/атаку
};

struct Protocol
{
    uint32_t id;
    uint32_t opp_id;
    MOVES move;
    Card card; // с какой картой происходит действие move
    void serialize(char *buffer);
    static Protocol deserialize(char *buffer);
};