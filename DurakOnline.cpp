#include "DurakOnline.hpp"

std::string url_base = "postgresql://postgres:NiPWYEfWWdhjhkATtEeg-g7ZD@localhost:5432/durak";

DurakOnline::DurakOnline(int argc, char *argv[]) : app(argc, argv) {}

bool DurakOnline::registration()
{
    auto session = make_session(url_base);
    pqxx::work tx(*session);
    std::string name = window.get_reg_Username().text().toStdString();
    std::string email = window.get_reg_Email().text().toStdString();
    std::string password = window.get_reg_Password().text().toStdString();
    std::string confirmPassword = window.get_reg_ConfirmPassword().text().toStdString();
    // Добавить больше валидации чтобы не вызывать базу данных просто так
    try
    {
        tx.exec("insert into users (username, email, password) values ($1, $2, $3)", pqxx::params{name, email, password});
        std::cout << "Succes authorization new user\n";
    }
    catch (std::exception &error)
    {
        delete_session(session);
        std::cout << "Something went wrong\n";
        return false;
    }
    tx.commit();
    delete_session(session);
    return true;
}

bool DurakOnline::login()
{
    std::string email = window.get_login_Email().text().toStdString();
    std::string password = window.get_login_Password().text().toStdString();
    // Валидация
    pqxx::connection *session = make_session(url_base);
    pqxx::work tx(*session);
    pqxx::result r = tx.exec("select id, username, email from users where email=$1 and password=$2 limit 1", pqxx::params{email, password});
    if (!r.empty())
    {
        pqxx::row user = r[0];
        CurrentUser NewUser(user[0].as<unsigned long>(), user[1].as<std::string>(), user[2].as<std::string>());
        current_user = std::move(NewUser);
        window.main();
    }
    else
    {
        std::cout << "Unavailable email or password\n";
        return false;
    }
    return true;
}

void DurakOnline::logout()
{
    current_user.to_null();
    std::cout << "hello!\n";
    window.login();
}

void DurakOnline::connect()
{
    QObject::connect(&window.get_reg_SubmitBttn(), &QPushButton::clicked, this, &DurakOnline::registration);
    QObject::connect(&window.get_login_LoginBttn(), &QPushButton::clicked, this, &DurakOnline::login);
    QObject::connect(&window.get_main_LogoutBttn(), &QPushButton::clicked, this, &DurakOnline::logout);
}

int DurakOnline::start()
{
    connect();
    window.setWindowTitle("Дурак онлайн");
    window.connect();
    window.showMaximized();
    int answer = app.exec();
    return answer;
}