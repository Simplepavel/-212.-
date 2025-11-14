#include <cstdint>
#include <winsock2.h>
enum DataType : uint8_t // что передали какой тип данный содержится в данных
{
    FIND_ENEMY,
    MOVE,
    START,
};

struct Mark1
{
    DataType type;   // тип данных
    char *data;      // данные
    uint32_t length; // длина данных

    char *serialize() const;
    uint32_t capacity() const;
    static Mark1 deserialize(char *buffer);
};