#include <pqxx/pqxx>
#include <iostream>
#include <string>

pqxx::connection *make_session(const std::string &);
bool delete_session(pqxx::connection *);