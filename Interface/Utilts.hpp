
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
    TXT,    // Просто текст
    SUCCES, // цвет зеленый
    ERR,    // цвет красный
};

enum TextSize : int
{
    SMALLEST = 8,
    SMALL = 16,
    BIG = 24,
    BIGGEST = 32,
};

struct Message
{
    QString message;
    Owners owner;
    MessageType type;
    TextSize size;
};

bool ValidateEmail(const std::string &value);
Message CreateMessage(const QString mess, Owners ow, MessageType type, TextSize size);