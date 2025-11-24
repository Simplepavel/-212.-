#include "DurakOnline.hpp"

std::string url_base = "postgresql://postgres:NiPWYEfWWdhjhkATtEeg-g7ZD@localhost:5432/durak";
char SERVER_IP[] = "127.0.0.1";
char SERVER_PORT[] = "6666";

DurakOnline::DurakOnline(int argc, char *argv[]) : app(argc, argv), session_id(0), FirstPosition(nullptr), SecondPosition(nullptr), board(nullptr) {}

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
    if (!client.is_ready())
    {
        bool flag = client.Client_Connect(SERVER_IP, SERVER_PORT);
        if (flag) // удачное подключение, пока только один поток
        {
            client.set_ready(true);
            std::thread listen_thread(&Durak_Client::Client_Listen, std::ref(client)); // ждем команд от сервера
            listen_thread.detach();
        }
    }
    if (client.is_ready())
    {
        Mark1 to_send;
        to_send.data = new char[4];
        uint32_t net_id = htonl(current_user.get_id());
        memcpy(to_send.data, &net_id, 4);
        to_send.length = 4;
        to_send.type = DataType::FIND_ENEMY;
        int bytes = client.Client_Send(to_send);
        window.wait();
        window.get_wait_Timer().start();
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
    QObject::connect(&window.get_play_StopBttn(), &QPushButton::clicked, this, &DurakOnline::Disconnect);
    QObject::connect(&window.get_play_NextBttn(), &QPushButton::clicked, this, &DurakOnline::Next);
    QObject::connect(&window.get_wait_StopBttn(), &QPushButton::clicked, this, &DurakOnline::StopFind);
    QObject::connect(&client, &Durak_Client::ServerSentData, this, &DurakOnline::play);
}

void DurakOnline::play() // Соперник уже найден
{
    Mark1 recv_data = Mark1::deserialize(client.GetData());
    if (recv_data.type == DataType::START)
    {
        window.get_wait_Timer().stop();
        uint32_t net_session_id;

        memcpy(&net_session_id, recv_data.data, 4);
        session_id = ntohl(net_session_id);

        memcpy(&IsMyTurn, recv_data.data + 4, 1);
        MyColor = IsMyTurn ? FigureColor::WHITE : FigureColor::BLACK;

        std::string opp_name;
        opp_name.resize(recv_data.length - 5);
        memcpy(&opp_name[0], recv_data.data + 5, recv_data.length - 5);

        window.get_play_EnemyName().setText(QString::fromStdString(opp_name));

        delete board;
        board = new Board(MyColor);
        std::vector<MyPushButton *> NewBttns = window.FillBoard(); // сюда передадим ссылку на Board
        for (auto i = NewBttns.begin(); i != NewBttns.end(); ++i)
        {
            QObject::connect(*i, &MyPushButton::clicked, this, &DurakOnline::MakeMove);
        }
        window.UpdateBoard(*board, MyColor);
        window.play(); // передать указатель на Board
    }

    else if (recv_data.type == DataType::BOARD)
    {
        board->DeserializeMove(recv_data.data + 4);
        board->replace();
        window.UpdateBoard(*board, MyColor);
        IsMyTurn = true;
    }
    else if (recv_data.type == DataType::LEAVE_ENEMY) // Соперник покинул игру. Текущий игрок снова добавляется в очередь!!!!
    {
        window.wait();
        window.get_wait_Timer().start();
    }
    else if (recv_data.type == DataType::SHUTDOWN)
    {
        client.Client_Disconnect();
        client.set_ready(false); // Больше не слушаем сервер
        window.main();
    }
}

void DurakOnline::MakeMove()
{
    if (IsMyTurn) // Сейчас мой ход
    {
        if (FirstPosition == nullptr)
        {
            FirstPosition = static_cast<MyPushButton *>(sender()); // просто клетка
            if (!FirstPosition->get_figure()->is_valid())
            {
                FirstPosition = nullptr;
                return;
            }
            if (FirstPosition->get_figure()->get_color() != MyColor) // не своя фигура
            {
                FirstPosition = nullptr;
                return;
            }
        }
        else // Первая клетка получена
        {
            SecondPosition = static_cast<MyPushButton *>(sender());
            int current_row = FirstPosition->get_row();
            int current_column = FirstPosition->get_column();

            int last_row = SecondPosition->get_row();
            int last_column = SecondPosition->get_column();

            if (board->move(current_row, current_column, last_row, last_column)) // вызывать метод move у доски. Вставить в MOVE изменения LastMove
            {
                board->replace(current_row, current_column, last_row, last_column); // при удачном move перерисовываем. Можно убрать после корректной реазлизации move
                window.UpdateBoard(*board, MyColor);
                char *new_board_serialize = board->SerializeMove();
                uint32_t net_session_id = htonl(session_id);
                char *data = new char[8];
                memcpy(data, &net_session_id, 4);
                memcpy(data + 4, new_board_serialize, 4);
                Mark1 to_send;
                to_send.type = DataType::BOARD;
                to_send.length = 8;
                to_send.data = data;

                int send_bytes = client.Client_Send(to_send);
                FirstPosition = nullptr;
                SecondPosition = nullptr;
                IsMyTurn = false;
            }
        }
    }
}

void DurakOnline::Disconnect()
{
    uint32_t net_session_id = htonl(session_id); // id сессии чтобы отключиться
    char *data = new char[4];
    memcpy(data, &net_session_id, 4);
    Mark1 to_send;
    to_send.type = DISCONNECT;
    to_send.length = 4;
    to_send.data = data;
    client.Client_Send(to_send);
}

void DurakOnline::Next()
{
    uint32_t net_session_id = htonl(session_id); // id сессии чтобы отключиться
    char *data = new char[4];
    memcpy(data, &net_session_id, 4);
    Mark1 to_send;
    to_send.type = DataType::NEXT_ENEMY;
    to_send.length = 4;
    to_send.data = data;
    client.Client_Send(to_send);
}

void DurakOnline::StopFind()
{
    uint32_t net_id = htonl(current_user.get_id()); // id сессии чтобы отключиться
    char *data = new char[4];
    memcpy(data, &net_id, 4);
    Mark1 to_send;
    to_send.type = DataType::STOP_FIND_ENEMY;
    to_send.length = 4;
    to_send.data = data;
    client.Client_Send(to_send);
}