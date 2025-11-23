#include <string>
#include <iostream>
#include <cstdint>
class CurrentUser
{
private:
    uint32_t id;
    std::string username;
    std::string email;
    bool null;

public:
    CurrentUser();
    CurrentUser(uint32_t _id, const std::string &_username, const std::string &_email);
    CurrentUser &operator=(CurrentUser &&);
    uint32_t get_id() const;
    const std::string &get_username() const;
    const std::string &get_email() const;
    bool is_null();
    void to_null();
};

std::ostream &operator<<(std::ostream &cout, const CurrentUser &argv);
