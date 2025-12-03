#include "DurakOnline.hpp"

char SERVER_IP[] = "127.0.0.1";
char SERVER_PORT[] = "6666";

DurakOnline::DurakOnline(int argc, char *argv[]) : app(argc, argv), session_id(0), FirstPosition(nullptr), SecondPosition(nullptr), board(nullptr)
{
    LeaderBoardUpdateTimer.setInterval(std::chrono::seconds(60));
    UpdateLeaderBoard();
    LeaderBoardUpdateTimer.start();
}

void DurakOnline::registration()
{

    std::string name = window.get_reg_Username().text().toStdString();
    std::string email = window.get_reg_Email().text().toStdString();
    std::string password = window.get_reg_Password().text().toStdString();
    std::string confirmPassword = window.get_reg_ConfirmPassword().text().toStdString();

    bool flag = false; // Были или нет ошибки

    if (name.empty())
    {
        flag = true;
        window.AddStateMessage(CreateMessage("Field username must not be empty", CHECKIN, ERR, SMALLEST));
    }

    if (email.empty())
    {
        flag = true;
        window.AddStateMessage(CreateMessage("Field email must not be empty", CHECKIN, ERR, SMALLEST));
    }

    if (password.empty())
    {
        flag = true;
        window.AddStateMessage(CreateMessage("Field password must not be empty", CHECKIN, ERR, SMALLEST));
    }

    if (password != confirmPassword)
    {
        flag = true;
        window.AddStateMessage(CreateMessage("Password must be equal to ConfirmPassword", CHECKIN, ERR, SMALLEST));
    }

    if (!isValidEmail(email))
    {
        flag = true;
        window.AddStateMessage(CreateMessage("Email is unavailable", CHECKIN, ERR, SMALLEST));
    }
    if (flag) // простая валидация на стороне клиента
    {
        window.registration(); // очищаем данные добавляем информацию о ошибках
        return;
    }

    pqxx::connection *session = make_session();
    pqxx::work tx(*session);
    flag = false;
    {
        pqxx::result username_check = tx.exec("select * from users where username = $1 limit 1", pqxx::params{name});
        if (!username_check.empty())
        {
            flag = true;
            window.AddStateMessage(CreateMessage("User with this username is already exists. Try another one", CHECKIN, ERR, SMALLEST));
        }

        pqxx::result email_check = tx.exec("select * from users where email = $1 limit 1", pqxx::params{email});
        if (!email_check.empty())
        {
            flag = true;
            window.AddStateMessage(CreateMessage("User with this email is already exists. Try another one", CHECKIN, ERR, SMALLEST));
        }
    }
    if (flag) // сложная валидация с проверкой на существование
    {
        window.registration(); // очищаем данные добавляем информацию о ошибках
        return;
    }
    try
    {
        tx.exec("insert into users (username, email, password) values ($1, $2, $3)", pqxx::params{name, email, password});
        window.AddStateMessage(CreateMessage("Succes authorization new user", LOGIN, SUCCES, SMALLEST));
    }
    catch (std::exception &error)
    {
        window.AddStateMessage(CreateMessage("Something went wrong. Please, try again", CHECKIN, ERR, SMALLEST));
    }
    tx.commit();
    delete_session(session);
    window.login();
}

void DurakOnline::login()
{
    std::string email = window.get_login_Email().text().toStdString();
    std::string password = window.get_login_Password().text().toStdString();
    // Валидация
    bool flag = false;
    if (email.empty())
    {
        flag = true;
        window.AddStateMessage(CreateMessage("Field email must not be empty", LOGIN, ERR, SMALLEST));
    }

    if (password.empty())
    {
        flag = true;
        window.AddStateMessage(CreateMessage("Field password must not be empty", LOGIN, ERR, SMALLEST));
    }

    if (flag)
    {
        window.login();
        return;
    }

    pqxx::connection *session = make_session();
    pqxx::work tx(*session);

    pqxx::result r = tx.exec("select id, username, email, rating, picture from users where email=$1 and password=$2 limit 1", pqxx::params{email, password});

    if (!r.empty())
    {
        pqxx::row user = r[0];

        CurrentUser NewUser(user[0].as<unsigned long>(), user[1].as<std::string>(), user[2].as<std::string>(), user[3].as<uint32_t>());
        current_user = std::move(NewUser);
        window.AddStateMessage(CreateMessage("Succesful authentication", MAIN, SUCCES, SMALLEST));

        window.get_profile_Username().setText(QString::fromStdString(current_user.get_username()));
        window.get_profile_Rank().setText(QString::number(current_user.get_rating()));

        window.main();
    }
    else
    {
        window.AddStateMessage(CreateMessage("Unavailable email or password", LOGIN, ERR, SMALLEST));
        window.login();
    }
}

void DurakOnline::main()
{
    window.main();
}

void DurakOnline::logout()
{
    current_user.to_null();
    window.login();
}

void DurakOnline::profile()
{
    // пользователь не видит свой же статус и приглашение на игру
    window.get_profile_Status().hide();
    window.get_profile_Invite().hide();
    window.InsertMessage(PROFILE);
    window.profile();
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
        Status = LOOKING_FOR;
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

void DurakOnline::ChangeUserName()
{
    std::string NewName = window.GetNewName().toStdString();
    if (NewName == "")
    {
        return;
    }
    pqxx::connection *session = make_session();
    pqxx::work tx(*session);
    pqxx::result username_check = tx.exec("select * from users where username = $1 limit 1", pqxx::params{NewName});
    if (!username_check.empty())
    {
        window.AddStateMessage(CreateMessage("User with this username is already exists. Try another one", PROFILE, ERR, SMALLEST));
    }
    else
    {
        tx.exec("update users set username = $1 where id = $2", pqxx::params{NewName, current_user.get_id()});
        current_user.set_username(NewName);
        // Не вижу смысла каждый раз при загрузке профиля обновлять имя соглано current_user
        window.get_profile_Username().setText(QString::fromStdString(current_user.get_username()));
        tx.commit();
    }

    delete_session(session);
}

void DurakOnline::ChangePhoto()
{
    std::string NewPhoto = window.GetNewPhoto().toStdString();
    if (NewPhoto != "")
    {
        bool flag = client.Client_Connect(SERVER_IP, SERVER_PORT); // Подключаемся к серверу
        if (!flag)
        {
            return;
        }
        std::ifstream NewImg(NewPhoto, std::ios::binary);
    }
}

int DurakOnline::start()
{
    connect();
    window.setWindowTitle("Chess");
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
    QObject::connect(&window.get_main_MyProfile(), &QPushButton::clicked, this, &DurakOnline::profile);
    QObject::connect(&client, &Durak_Client::ServerSentData, this, &DurakOnline::play);
    QObject::connect(&LeaderBoardUpdateTimer, &QTimer::timeout, this, &DurakOnline::UpdateLeaderBoard);
    QObject::connect(&window.get_profile_BackBttn(), &QPushButton::clicked, this, &DurakOnline::main);
    QObject::connect(&window.get_profile_ChangeUnBttn(), &QPushButton::clicked, this, &DurakOnline::ChangeUserName);
    QObject::connect(&window.get_profile_ChangePhoto(), &QPushButton::clicked, this, &DurakOnline::ChangePhoto);
    QObject::connect(&window, &QWidget::destroyed, this, &DurakOnline::OnCloseWindow);
}

void DurakOnline::play() // Соперник уже найден
{
    Mark1 recv_data = Mark1::deserialize(client.GetData());
    if (recv_data.type == DataType::START)
    {
        Status = IN_GAME;

        window.InsertMessage(PLAY); // удаляем старые записи если есть
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

    else if (recv_data.type == DataType::MOVE)
    {
        const std::vector<uint8_t> &LastMoves = board->DeserializeMove(recv_data.data + 4);
        board->replace(7 - LastMoves[0], 7 - LastMoves[1], 7 - LastMoves[2], 7 - LastMoves[3]);
        QString message = QString::fromStdString(board->numericToAlgebraic(LastMoves[0], LastMoves[1], LastMoves[2], LastMoves[3]));
        window.AddStateMessage(CreateMessage(message, PLAY, TXT, BIG));
        window.UpdateBoard(*board, MyColor);
        IsMyTurn = true;
    }
    else if (recv_data.type == DataType::CLASTING)
    {
        const std::vector<uint8_t> &LastMoves = board->DeserializeMove(recv_data.data + 4);
        board->clasting(7 - LastMoves[0], 7 - LastMoves[1], 7 - LastMoves[2], 7 - LastMoves[3]);
        QString message = "Clasting";
        window.AddStateMessage(CreateMessage(message, PLAY, TXT, BIG));
        window.UpdateBoard(*board, MyColor);
        IsMyTurn = true;
    }
    else if (recv_data.type == DataType::LEAVE_ENEMY) // Соперник покинул игру. Текущий игрок снова добавляется в очередь!!!!
    {
        window.wait();
        window.get_wait_Timer().start();
        Status = LOOKING_FOR;
    }
    else if (recv_data.type == DataType::SHUTDOWN)
    {
        client.Client_Disconnect();
        client.set_ready(false); // Больше не слушаем сервер
        Status = ONLINE;
        window.main();
    }
    else if (recv_data.type == DataType::CHECKMATE)
    {
        window.AddStateMessage(CreateMessage("You are lose!", PLAY, ERR, BIG));
        const std::vector<uint8_t> &LastMoves = board->DeserializeMove(recv_data.data + 4);
        board->replace(7 - LastMoves[0], 7 - LastMoves[1], 7 - LastMoves[2], 7 - LastMoves[3]);
        window.UpdateBoard(*board, MyColor);
    }
    window.InsertMessage(PLAY, false);
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

            bool ans = board->move(current_row, current_column, last_row, last_column);
            std::cout << "Move: " << ans << '\n';
            if (ans) // вызывать метод move у доски. Вставить в MOVE изменения LastMove
            {
                Mark1 to_send;
                if (board->isCheckmate(current_row, current_column, last_row, last_column))
                {
                    window.AddStateMessage(CreateMessage("You are win!", PLAY, SUCCES, BIG));
                    board->replace(current_row, current_column, last_row, last_column);
                    to_send.type = DataType::CHECKMATE;
                }
                else if (board->isValidCastling(current_row, current_column, last_row, last_column)) // Рокировка
                {
                    window.AddStateMessage(CreateMessage("Clasting", PLAY, TXT, BIG));
                    board->clasting(current_row, current_column, last_row, last_column);
                    to_send.type = DataType::CLASTING;
                }
                else // Обычный ход
                {
                    QString message = QString::fromStdString(board->numericToAlgebraic(current_row, current_column, last_row, last_column));
                    window.AddStateMessage(CreateMessage(message, PLAY, TXT, BIG));
                    board->replace(current_row, current_column, last_row, last_column);
                    to_send.type = DataType::MOVE;
                }

                window.InsertMessage(PLAY, false);
                window.UpdateBoard(*board, MyColor);

                char *new_board_serialize = board->SerializeMove();
                uint32_t net_session_id = htonl(session_id);
                char *data = new char[8];
                memcpy(data, &net_session_id, 4);
                memcpy(data + 4, new_board_serialize, 4);
                to_send.length = 8;
                to_send.data = data;

                int send_bytes = client.Client_Send(to_send);
                IsMyTurn = false;
            }
            FirstPosition = nullptr;
            SecondPosition = nullptr;
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

void DurakOnline::UpdateLeaderBoard()
{
    pqxx::connection *session = make_session();
    pqxx::work tx(*session);
    pqxx::result response = tx.exec("select username, rating from users order by rating desc limit 50");
    int idx = 0;
    for (auto i = response.begin(); i != response.end(); ++i)
    {
        window.UpdateLeaderBoard(i[0].as<std::string>(), i[1].as<std::string>(), idx++);
    }
}

void DurakOnline::OnCloseWindow()
{
    if (client.is_ready())
    {
        if (Status == IN_GAME)
        {
            Disconnect();
        }
        else if (Status == LOOKING_FOR)
        {
            StopFind();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Сервер должен успеть сработать
        client.Client_Disconnect();
        client.set_ready(false); // Больше не слушаем сервер
    }
}