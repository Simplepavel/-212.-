#include "CurrentUser.hpp"

CurrentUser::CurrentUser() : id(-1), username(""), email(""), null(true) {}
CurrentUser::CurrentUser(unsigned long _id, const std::string &_username, const std::string &_email) : id(_id), username(_username), email(_email), null(false) {}
CurrentUser &CurrentUser::operator=(CurrentUser &&value)
{
    id = value.id;
    username = std::move(value.username);
    email = std::move(value.email);
    null = value.null;
    return *this;
}

unsigned long CurrentUser::get_id() const { return id; }
const std::string &CurrentUser::get_email() const { return email; }
const std::string &CurrentUser::get_username() const { return username; }
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