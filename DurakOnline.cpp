#include "DurakOnline.hpp"

std::string url_base = "postgresql://postgres:NiPWYEfWWdhjhkATtEeg-g7ZD@localhost:5432/durak";
char SERVER_IP[] = "127.0.0.1";
char SERVER_PORT[] = "6666";

DurakOnline::DurakOnline(int argc, char *argv[]) : app(argc, argv), session_id(0), FirstPosition(nullptr), SecondPosition(nullptr) {}

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
    Mark1 recv_data = Mark1::deserialize(client.GetData());
    if (recv_data.type == DataType::START)
    {
        uint32_t net_session_id;

        memcpy(&net_session_id, recv_data.data, 4);
        session_id = ntohl(net_session_id);

        memcpy(&IsMyTurn, recv_data.data + 4, 1);
        MyColor = IsMyTurn ? FigureColor::WHITE : FigureColor::BLACK;
        std::cout << (MyColor == FigureColor::WHITE ? "WHITE" : "BLACK") << '\n';

        std::string opp_name;
        opp_name.resize(recv_data.length - 5);
        memcpy(&opp_name[0], recv_data.data + 5, recv_data.length - 5);

        window.get_play_EnemyName().setText(QString::fromStdString(opp_name));

        board = new Board(MyColor);
        std::vector<MyPushButton *> NewBttns = window.FillBoard(); // сюда передадим ссылку на Board
        for (auto i = NewBttns.begin(); i != NewBttns.end(); ++i)
        {
            QObject::connect(*i, &MyPushButton::clicked, this, &DurakOnline::MakeMove);
        }
        window.UpdateBoard(*board, MyColor);
        window.play(); // передать указатель на Board
    }

    if (recv_data.type == DataType::BOARD)
    {
        board->DeserializeMove(recv_data.data + 4);
        board->replace();

        window.UpdateBoard(*board, MyColor);
        IsMyTurn = true;
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

            const Figure *current_figure = FirstPosition->get_figure();
            if (current_figure->IsValidMove(current_row, current_column, last_row, last_column)) // теперь данных метод отвечает за ВСЮ логику игры
            {
                board->replace(current_row, current_column, last_row, last_column);
                window.UpdateBoard(*board, MyColor);
                char *new_board_serialize = board->SerializeMove();
                std::cout << "new board serialize\n";
                uint32_t net_session_id = htonl(session_id);
                std::cout << "net session id\n";
                char *data = new char[8];
                memcpy(data, &net_session_id, 4);
                std::cout << "memcpy one\n";
                memcpy(data + 4, new_board_serialize, 4);
                std::cout << "memcpy two\n";
                Mark1 to_send;
                to_send.type = DataType::BOARD;
                to_send.length = 8;
                to_send.data = data;

                int send_bytes = client.Client_Send(to_send);
                std::cout << "SEND: " << send_bytes << '\n';
                FirstPosition = nullptr;
                SecondPosition = nullptr;
                IsMyTurn = false;
            }
        }
    }
}
