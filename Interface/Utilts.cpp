#include "Utilts.hpp"


Message CreateMessage(const QString mess, Owners ow, MessageType type, TextSize size)
{
    Message NewErrorMessage;
    NewErrorMessage.owner = ow;
    NewErrorMessage.type = type;
    NewErrorMessage.message = mess;
    NewErrorMessage.size = size;
    return NewErrorMessage;
}




bool isValidEmail(const std::string& email) {
    if (email.empty()) return false;
    

    if (email.length() > 254 || email.length() < 3) return false; //длина
    

    size_t at_pos = email.find('@');
    if (at_pos == std::string::npos || at_pos == 0) return false; // собачка
    

    std::string local_part = email.substr(0, at_pos);
    if (local_part.length() > 64) return false;
    std::string domain_part = email.substr(at_pos + 1);
    if (domain_part.empty()) return false;
    

    size_t dot_pos = domain_part.find('.');
    if (dot_pos == std::string::npos || dot_pos == 0) return false; // точка в домене(.ru и тд)
    
  
    size_t last_dot = domain_part.rfind('.');
    if (last_dot == std::string::npos || 
        domain_part.length() - last_dot < 3) return false; // после точки должно быть >1 буквы
    

    const std::regex pattern(R"(^[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+@[a-zA-Z0-9-]+(?:\.[a-zA-Z0-9-]+)*$)"); // общая проверка символов
    if (!std::regex_match(email, pattern)) return false;
    
    return true;
}