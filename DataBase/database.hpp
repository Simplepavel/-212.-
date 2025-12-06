#include <iostream>
#include <pqxx/pqxx>
#include <string>
pqxx::connection* make_session();
bool delete_session(pqxx::connection*);

void InitializeDatabase();