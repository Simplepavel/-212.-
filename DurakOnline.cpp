#include "DurakOnline.hpp"

std::string url_base = "postgresql://postgres:NiPWYEfWWdhjhkATtEeg-g7ZD@localhost:5432/durak";

DurakOnline::DurakOnline(int argc, char *argv[]) : app(argc, argv) {}

bool DurakOnline::registration()
{
    auto session = make_session(url_base);
    pqxx::work tx(*session);
    std::string name = window.get_reg_Username().text().toStdString();
    std::string email = window.get_reg_Username().text().toStdString();
    std::string password = window.get_reg_Username().text().toStdString();
    tx.exec("insert into users (username, email, password) values ($1, $2, $3)", pqxx::params{name, email, password});
    tx.commit();
    delete_session(session);
    return true;
}

void DurakOnline::connect()
{
    QObject::connect(&window.get_reg_SubmitBttn(), &QPushButton::clicked, this, &DurakOnline::registration);
}

int DurakOnline::start()
{
    window.setWindowTitle("Дурак онлайн");
    window.connect();
    window.show();
    int answer = app.exec();
    return answer;
}