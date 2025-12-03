#include <string>
#include <iostream>
#include <cstdint>
class CurrentUser
{
private:
    uint32_t id;
    std::string username;
    std::string email;
    uint32_t rating;
    bool null;

public:
    CurrentUser();
    CurrentUser(uint32_t _id, const std::string &_username, const std::string &_email, uint32_t _rating);

    CurrentUser &operator=(CurrentUser &&);
    uint32_t get_id() const;
    const std::string &get_username() const;
    const std::string &get_email() const;
    uint32_t get_rating() const;

    void set_username(const std::string &);
    void set_id(uint32_t);
    void set_email(const std::string &);
    void set_rating(uint32_t);
    void set_null(bool new_vl);
    bool is_null() const;
    void to_null();
};

std::ostream &operator<<(std::ostream &cout, const CurrentUser &argv);
CurrentUser &operator>>(std::istream &cout,CurrentUser &argv);
