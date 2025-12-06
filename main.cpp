#include "DataBase/database.hpp"
#include "DurakOnline.hpp"

int main(int argc, char* argv[]) {
  InitializeDatabase();
  DurakOnline app(argc, argv);
  return app.start();
}