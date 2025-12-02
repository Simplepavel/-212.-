#include "database.hpp"

std::string url_base = "postgresql://postgres:gfhjkm123@localhost:5432/chess";

pqxx::connection* make_session() {
  pqxx::connection* cx = nullptr;
  try {
    cx = new pqxx::connection{url_base};  // все будет хорошо
  } catch (std::exception& error) {
    std::cout << error.what();
    return nullptr;
  }
  return cx;
}
bool delete_session(pqxx::connection* cx) {
  if (cx != nullptr) {
    delete cx;
    return true;
  }
  return false;
}
