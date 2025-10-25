#include "database.hpp"


pqxx::connection *make_session(const std::string &url)
{
    pqxx::connection *cx = nullptr;
    try
    {
        cx = new pqxx::connection{url}; // все будет хорошо
    }
    catch (std::exception &error)
    {
        std::cout << error.what();
        return nullptr;
    }
    return cx;
}
bool delete_session(pqxx::connection *cx)
{
    if (cx != nullptr)
    {
        delete cx;
        return true;
    }
    return false;
}
