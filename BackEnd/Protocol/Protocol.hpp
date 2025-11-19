#include <cstdint>
#include <winsock2.h>
#include <iostream>
enum DataType : uint8_t // что передали какой тип данный содержится в данных
{
    NONE,
    FIND_ENEMY,
    BOARD, // ХОД
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

    Mark1();
    Mark1(const Mark1 &);
    Mark1 &operator=(const Mark1 &);
    Mark1(Mark1 &&);
    Mark1 &operator=(Mark1 &&);
    ~Mark1();
};