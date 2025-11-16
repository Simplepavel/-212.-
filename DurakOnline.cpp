#include "DurakOnline.hpp"

std::string url_base = "postgresql://postgres:NiPWYEfWWdhjhkATtEeg-g7ZD@localhost:5432/durak";
char SERVER_IP[] = "127.0.0.1";
char SERVER_PORT[] = "6666";

DurakOnline::DurakOnline(int argc, char *argv[]) : app(argc, argv) {}

bool DurakOnline::registration()
{
    pqxx::connection *session = make_session(url_base);
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
    window.login();
}

void DurakOnline::FindEnemy()
{
    bool flag = client.Client_Connect(SERVER_IP, SERVER_PORT);
    if (flag) // удачное подключение, пока только один поток
    {
        Mark1 to_send;
        to_send.data = new char[4];
        uint32_t net_id = htonl(current_user.get_id());
        memcpy(to_send.data, &net_id, 4);
        to_send.length = 4;
        to_send.type = DataType::FIND_ENEMY;
        int bytes = client.Client_Send(to_send);
        client.set_ready(true);
        std::thread listen_thread(&Durak_Client::Client_Listen, std::ref(client));
        listen_thread.detach();
    }
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

void DurakOnline::connect()
{
    QObject::connect(&window.get_reg_SubmitBttn(), &QPushButton::clicked, this, &DurakOnline::registration);
    QObject::connect(&window.get_login_LoginBttn(), &QPushButton::clicked, this, &DurakOnline::login);
    QObject::connect(&window.get_main_LogoutBttn(), &QPushButton::clicked, this, &DurakOnline::logout);
    QObject::connect(&window.get_main_PlayBttn(), &QPushButton::clicked, this, &DurakOnline::FindEnemy);
    QObject::connect(&client, &Durak_Client::ServerSentData, this, &DurakOnline::play);
}

void DurakOnline::play()
{
    std::cout << "Read from buffer and draw the board depence of the data type\n";
    Mark1 recv_data = Mark1::deserialize(client.GetData());
    if (recv_data.type == DataType::START)
    {
        window.play();
    }
    // с этого момента можно читать данные из buffer сокета и в зависимости от их типа данных вызывать те или иные фунции
}
