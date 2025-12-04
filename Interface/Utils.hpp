#include <regex>
#include <string>
#include <QString>
#include <QPixmap>
#include <QBuffer>
#include <QByteArray>

enum Owners : uint8_t // кому предназначена ифномарция
{
    LOGIN, // полю main
    MAIN,  // полю main
    PLAY,  // полю play
    CHECKIN,
    PROFILE

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

bool isValidEmail(const std::string &email);
Message CreateMessage(const QString mess, Owners ow, MessageType type, TextSize size);
QPixmap CutPixMap(const QPixmap &argv, int height, int width);
char *pixmapToCharArray(const QPixmap &pixmap, int &outSize);
