#include "CurrentUser.hpp"

CurrentUser::CurrentUser() : id(-1), username(""), email(""), null(true) {}
CurrentUser::CurrentUser(uint32_t _id, const std::string &_username, const std::string &_email, uint32_t _rating) : id(_id), username(_username), email(_email), rating(_rating), null(false) {}
CurrentUser &CurrentUser::operator=(CurrentUser &&value)
{
    id = value.id;
    username = std::move(value.username);
    email = std::move(value.email);
    rating = value.rating;
    null = value.null;
    return *this;
}

uint32_t CurrentUser::get_id() const { return id; }
const std::string &CurrentUser::get_email() const { return email; }
const std::string &CurrentUser::get_username() const { return username; }
uint32_t CurrentUser::get_rating() const { return rating; }
bool CurrentUser::is_null() { return null; }
void CurrentUser::to_null()
{
    id = -1;
    email.clear();
    username.clear();
    null = true;
}

std::ostream &operator<<(std::ostream &cout, const CurrentUser &argv)
{
    cout << "ID " << argv.get_id() << " UserName " << argv.get_username() << " Email " << argv.get_email();
    return cout;
}


void CurrentUser::set_username(const std::string& new_value)
{
    username = new_value;
}