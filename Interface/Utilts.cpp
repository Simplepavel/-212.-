#include "Utilts.hpp"

bool ValidateEmail(const std::string &value)
{
    return true;
}

Message CreateMessage(const QString mess, Owners ow, MessageType type, TextSize size)
{
    Message NewErrorMessage;
    NewErrorMessage.owner = ow;
    NewErrorMessage.type = type;
    NewErrorMessage.message = mess;
    NewErrorMessage.size = size;
    return NewErrorMessage;
}