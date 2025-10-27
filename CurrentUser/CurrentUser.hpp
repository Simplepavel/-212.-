#include <string>
#include <iostream>
class CurrentUser
{
private:
    unsigned long id;
    std::string username;
    std::string email;
    bool null;

public:
    CurrentUser();
    CurrentUser(unsigned long _id, const std::string &_username, const std::string &_email);
    CurrentUser &operator=(CurrentUser &&);
    unsigned long get_id() const;
    const std::string &get_username() const;
    const std::string &get_email() const;
    bool is_null();
    void to_null();
};

std::ostream &operator<<(std::ostream &cout, const CurrentUser &argv);
