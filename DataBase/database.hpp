#include <QString>
#include <pqxx/pqxx>
#include <iostream>

pqxx::connection *make_session(const QString &);
bool delete_session(pqxx::connection *);
bool execute(pqxx::connection *cx, const QString &query, pqxx::result &r);