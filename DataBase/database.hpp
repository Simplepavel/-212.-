#include <pqxx/pqxx>
#include <iostream>
#include <string>
pqxx::connection *make_session();
bool delete_session(pqxx::connection *);