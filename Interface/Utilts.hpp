
#include <QString>
enum Owners : uint8_t // кому предназначена ифномарция
{
    LOGIN, // полю main
    MAIN,  // полю main
    PLAY,  // полю play
    CHECKIN,

};

enum MessageType : uint8_t
{
    SUCCES, // цвет зеленый
    ERR,    // цвет красный
};

struct Message
{
    QString message;
    Owners owner;
    MessageType type;
};

bool ValidateEmail(const std::string &value);
Message CreateMessage(const QString mess, Owners ow, MessageType type);