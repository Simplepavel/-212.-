#include "Utilts.hpp"

bool ValidateEmail(const std::string &value)
{
    return true;
}

Message CreateMessage(const QString mess, Owners ow, MessageType type)
{
    Message NewErrorMessage;
    NewErrorMessage.owner = ow;
    NewErrorMessage.type = type;
    NewErrorMessage.message = mess;
    return NewErrorMessage;
}