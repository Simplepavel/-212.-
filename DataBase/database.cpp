#include "database.hpp"

pqxx::connection *make_session(const QString &url)
{
    pqxx::connection *cx = nullptr;
    try
    {
        cx = new pqxx::connection{url.toStdString()}; // все будет хорошо
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

bool execute(pqxx::connection *cx, const QString &query, pqxx::result &r)
{
    if (cx == nullptr)
        return false;
    try
    {
        pqxx::work tx(*cx);
        r = tx.exec(query.toStdString()); // записали данные
        tx.commit();
    }
    catch (std::exception &error)
    {
        std::cout << error.what();
        return false;
    }
    return true;
}