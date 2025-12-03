#include <cstdint>
#include <winsock2.h>
#include <iostream>
enum DataType : uint8_t // что передали какой тип данный содержится в данных
{
    NONE,
    FIND_ENEMY,
    LEAVE_ENEMY,     // Соперник покинул матч
    NEXT_ENEMY,      // Найти нового соперника
    STOP_FIND_ENEMY, // Прекратить поиск соперника
    MOVE,            // ХОД
    CHECKMATE,
    CLASTING,
    START,
    DISCONNECT,
    SHUTDOWN, // Разрешение выполнить отключение от сервера
    DOWLOAD_
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