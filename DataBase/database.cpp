#include "database.hpp"

std::string db_user = "postgres";
std::string db_pass = "gfhjkm123";
std::string db_host = "localhost";
std::string db_port = "5432";
std::string db_name = "chess";

std::string GetConnectionString() {
  return "postgresql://" + db_user + ":" + db_pass + "@" + db_host + ":" +
         db_port + "/" + db_name;
}

pqxx::connection* make_session() {
  pqxx::connection* cx = nullptr;
  try {
    cx = new pqxx::connection{GetConnectionString()};  // все будет хорошо
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

void InitializeDatabase() {
  try {
    pqxx::connection C(GetConnectionString());

    if (!C.is_open()) {
      std::cerr << "Can't open database" << std::endl;
      return;
    }

    pqxx::work W(C);

    std::string sql =
        "CREATE TABLE IF NOT EXISTS users ("
        "id SERIAL PRIMARY KEY, "
        "username VARCHAR(50) UNIQUE NOT NULL, "
        "email VARCHAR(100) UNIQUE NOT NULL, "
        "password VARCHAR(100) NOT NULL, "
        "rating INT DEFAULT 1000, "
        "status INT DEFAULT 0);";

    W.exec(sql);
    W.commit();

    std::cout << "Database tables initialized successfully!" << std::endl;

  } catch (const std::exception& e) {
    std::cerr << "Database initialization failed: " << e.what() << std::endl;
  }
}
