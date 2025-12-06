#include <cstdint>
#include <winsock2.h>
#include <iostream>
enum DataType : uint8_t // что передали какой тип данный содержится в данных
{
    NONE,
    // Игровые команды
    START,
    FIND_ENEMY,
    LEAVE_ENEMY,
    NEXT_ENEMY,
    STOP_FIND_ENEMY,
    MOVE,
    CHECKMATE,
    CLASTING,
    GAMEOVER,

    ID,
    INVITE,
    CREATE_GAME, // создать игровую сессия со мной и с отпралвенным id(мой id и id соперника)
    UPLOAD_PHOTO,
    DOWNLOAD_PHOTO,

};

struct Mark1
{
    DataType type;   // тип данных
    char *data;      // данные
    uint32_t length; // длина полезных данных

    char *serialize() const;
    uint32_t capacity() const;
    uint32_t fullsize() const;
    static Mark1 deserialize(char *buffer);

    Mark1();
    Mark1(const Mark1 &);
    Mark1 &operator=(const Mark1 &);
    Mark1(Mark1 &&);
    Mark1 &operator=(Mark1 &&);
    ~Mark1();
};