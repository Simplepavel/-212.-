#include <cstdint>
#include <winsock2.h>
enum DataType : uint8_t // что передали какой тип данный содержится в данных
{
    Int,
    Text,
    Player
};

struct Mark1
{
    DataType type;
    char *data;
    uint32_t length;
    char *serialize();
    static Mark1 deserialize(char *buffer);
};