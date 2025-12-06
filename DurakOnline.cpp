#include "DurakOnline.hpp"

char SERVER_IP[] = "172.20.10.3";
char SERVER_PORT[] = "6666";

DurakOnline::DurakOnline(int argc, char* argv[])
    : app(argc, argv),
      session_id(0),
      FirstPosition(nullptr),
      SecondPosition(nullptr),
      board(nullptr) {
  LeaderBoardUpdateTimer.setInterval(std::chrono::seconds(60));
  std::vector<ProfileButton*> NewBttns = window.FillLeaderBoard();
  for (auto i = NewBttns.begin(); i != NewBttns.end(); ++i) {
    QObject::connect(*i, &ProfileButton::clicked, this,
                     &DurakOnline::CheckEnemyProfile);
  }
  LeaderBoardUpdateTimer.start();
}

void DurakOnline::MakeConnection() {
  if (!client.is_ready()) {
    bool flag = client.Client_Connect(SERVER_IP, SERVER_PORT);
    if (flag) {
      client.set_ready(true);
      std::thread listen_thread(&Durak_Client::Client_Listen,
                                std::ref(client));  // ждем команд от сервера
      listen_thread.detach();
    }
  }
}

void DurakOnline::registration() {
  std::string name = window.get_reg_Username().text().toStdString();
  std::string email = window.get_reg_Email().text().toStdString();
  std::string password = window.get_reg_Password().text().toStdString();
  std::string confirmPassword =
      window.get_reg_ConfirmPassword().text().toStdString();

  bool flag = false;  // Были или нет ошибки

  if (name.empty()) {
    flag = true;
    window.AddStateMessage(CreateMessage("Field username must not be empty",
                                         CHECKIN, ERR, SMALLEST));
  }

  if (email.empty()) {
    flag = true;
    window.AddStateMessage(
        CreateMessage("Field email must not be empty", CHECKIN, ERR, SMALLEST));
  }

  if (password.empty()) {
    flag = true;
    window.AddStateMessage(CreateMessage("Field password must not be empty",
                                         CHECKIN, ERR, SMALLEST));
  }

  if (password != confirmPassword) {
    flag = true;
    window.AddStateMessage(CreateMessage(
        "Password must be equal to ConfirmPassword", CHECKIN, ERR, SMALLEST));
  }

  if (!isValidEmail(email)) {
    flag = true;
    window.AddStateMessage(
        CreateMessage("Email is unavailable", CHECKIN, ERR, SMALLEST));
  }
  if (flag)  // простая валидация на стороне клиента
  {
    window.registration();  // очищаем данные добавляем информацию о ошибках
    return;
  }

  pqxx::connection* session = make_session();
  pqxx::work tx(*session);
  flag = false;
  {
    pqxx::result username_check = tx.exec(
        "select * from users where username = $1 limit 1", pqxx::params{name});
    if (!username_check.empty()) {
      flag = true;
      window.AddStateMessage(CreateMessage(
          "User with this username is already exists. Try another one", CHECKIN,
          ERR, SMALLEST));
    }

    pqxx::result email_check = tx.exec(
        "select * from users where email = $1 limit 1", pqxx::params{email});
    if (!email_check.empty()) {
      flag = true;
      window.AddStateMessage(CreateMessage(
          "User with this email is already exists. Try another one", CHECKIN,
          ERR, SMALLEST));
    }
  }
  if (flag)  // сложная валидация с проверкой на существование
  {
    window.registration();  // очищаем данные добавляем информацию о ошибках
    return;
  }
  try {
    tx.exec("insert into users (username, email, password) values ($1, $2, $3)",
            pqxx::params{name, email, password});
    window.AddStateMessage(CreateMessage("Succes authorization new user", LOGIN,
                                         SUCCES, SMALLEST));
  } catch (std::exception& error) {
    window.AddStateMessage(CreateMessage(
        "Something went wrong. Please, try again", CHECKIN, ERR, SMALLEST));
  }
  tx.commit();
  delete_session(session);
  window.login();
}

void DurakOnline::login() {
  std::string email = window.get_login_Email().text().toStdString();
  std::string password = window.get_login_Password().text().toStdString();
  // Валидация
  bool flag = false;
  if (email.empty()) {
    flag = true;
    window.AddStateMessage(
        CreateMessage("Field email must not be empty", LOGIN, ERR, SMALLEST));
  }

  if (password.empty()) {
    flag = true;
    window.AddStateMessage(CreateMessage("Field password must not be empty",
                                         LOGIN, ERR, SMALLEST));
  }

  if (flag) {
    window.login();
    return;
  }

  pqxx::connection* session = make_session();
  pqxx::work tx(*session);
  pqxx::result r = tx.exec(
      "select id, username, email, rating, status from users where email=$1 "
      "and password=$2 limit 1",
      pqxx::params{email, password});

  if (!r.empty()) {
    pqxx::row user = r[0];
    if (user[4].as<uint32_t>() == online) {
      window.AddStateMessage(CreateMessage(
          "This account is busy. Try another one", LOGIN, ERR, SMALLEST));
      window.login();
    } else {
      CurrentUser NewUser(user[0].as<unsigned long>(),
                          user[1].as<std::string>(), user[2].as<std::string>(),
                          user[3].as<uint32_t>());
      current_user = std::move(NewUser);
      window.SetUserId(current_user.get_id());
      window.AddStateMessage(
          CreateMessage("Succesful authentication", MAIN, SUCCES, SMALLEST));
      window.get_profile_id().setText("ID#" +
                                      QString::number(current_user.get_id()));
      window.get_profile_Username().setText(
          QString::fromStdString(current_user.get_username()));
      window.get_profile_Rank().setText(
          QString::number(current_user.get_rating()));
      UpdateLeaderBoard();
      window.main();
      UpdateUserStatus(online);
    }

    // Запрос фото по id(Вынести в функцию)
    // DownloadPhoto(current_user.get_id()); // Ответ обработать в GetServerData
    SendID();
    // Запрос фото по id(Вынести в функцию)
  } else {
    window.AddStateMessage(
        CreateMessage("Unavailable email or password", LOGIN, ERR, SMALLEST));
    window.login();
  }
}

void DurakOnline::ServerGetData()  // Сервер отправил информацию
{
  Mark1 recv_data = Mark1::deserialize(client.GetData());
  if (recv_data.type == DataType::START) {
    UpdateUserStatus(in_match);

    window.InsertMessage(PLAY);  // удаляем старые записи если есть
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
    std::vector<CellButton*> NewBttns =
        window.FillBoard();  // сюда передадим ссылку на Board
    for (auto i = NewBttns.begin(); i != NewBttns.end(); ++i) {
      QObject::connect(*i, &CellButton::clicked, this, &DurakOnline::MakeMove);
    }
    window.UpdateBoard(*board, MyColor);
    window.play();
  } else if (recv_data.type == DataType::MOVE) {
    const std::vector<uint8_t>& LastMoves =
        board->DeserializeMove(recv_data.data + 4);
    board->replace(7 - LastMoves[0], 7 - LastMoves[1], 7 - LastMoves[2],
                   7 - LastMoves[3]);
    QString message = QString::fromStdString(board->numericToAlgebraic(
        LastMoves[0], LastMoves[1], LastMoves[2], LastMoves[3]));
    window.AddStateMessage(CreateMessage(message, PLAY, TXT, BIG));
    window.UpdateBoard(*board, MyColor);
    IsMyTurn = true;
  } else if (recv_data.type == DataType::CLASTING) {
    const std::vector<uint8_t>& LastMoves =
        board->DeserializeMove(recv_data.data + 4);
    board->clasting(7 - LastMoves[0], 7 - LastMoves[1], 7 - LastMoves[2],
                    7 - LastMoves[3]);
    QString message = "Clasting";
    window.AddStateMessage(CreateMessage(message, PLAY, TXT, BIG));
    window.UpdateBoard(*board, MyColor);
    IsMyTurn = true;
  } else if (recv_data.type ==
             DataType::LEAVE_ENEMY)  // Соперник покинул игру. Текущий игрок
                                     // снова добавляется в очередь!!!!
  {
    UpdateUserStatus(looking_for);
    window.wait();
    window.get_wait_Timer().start();
  } else if (recv_data.type == DataType::CHECKMATE) {
    window.AddStateMessage(CreateMessage("You are lose!", PLAY, ERR, BIG));
    const std::vector<uint8_t>& LastMoves =
        board->DeserializeMove(recv_data.data + 4);
    board->replace(7 - LastMoves[0], 7 - LastMoves[1], 7 - LastMoves[2],
                   7 - LastMoves[3]);
    window.UpdateBoard(*board, MyColor);
  } else if (recv_data.type == DataType::UPLOAD_PHOTO) {
    uint32_t net_id;
    memcpy(&net_id, recv_data.data, 4);
    uint32_t id = ntohl(net_id);

    char* outStr = new char[(recv_data.length - 4) * 3 / 4 + 1];
    int outSize =
        base64Decode(recv_data.data + 4, recv_data.length - 4, outStr);

    QPixmap Image;
    QByteArray byteArray = QByteArray::fromRawData(outStr, outSize);
    Image.loadFromData(byteArray);

    if (id == current_user.get_id())  // Наше фото
    {
      RoundedAvatar& OwnPhotoProfile = window.get_profile_RoundedAvatar();
      OwnPhotoProfile.setPixmap(Image);
    } else {
      RoundedAvatar& EnemyPhotoProfile = window.get_profile_EnemyAvatar();
      EnemyPhotoProfile.setPixmap(Image);
    }
  } else if (recv_data.type == DataType::INVITE) {
    uint32_t net_enemy_id;
    memcpy(&net_enemy_id, recv_data.data, 4);
    uint32_t enemy_id = ntohl(net_enemy_id);
    pqxx::connection* session = make_session();
    pqxx::work tx(*session);
    pqxx::result response = tx.exec("select username from users where id = $1",
                                    pqxx::params{enemy_id});
    if (!response.empty()) {
      pqxx::row row = response[0];
      std::string username = row[0].as<std::string>();
      int answer = window.DialogWindow(
          QString::fromStdString(username + " wants to play with you!"));
      if (answer == QDialog::Accepted)  // Мы хотим играть с enemy
      {
        uint32_t net_id = htonl(current_user.get_id());

        Mark1 to_send;
        to_send.data = new char[8];
        to_send.length = 8;
        to_send.type = CREATE_GAME;

        memcpy(to_send.data, &net_id, 4);
        memcpy(to_send.data + 4, &net_enemy_id, 4);
        client.Client_Send(to_send);
      }
    }
    delete_session(session);
  }
  client.ClearData();
  window.InsertMessage(PLAY, false);
}

void DurakOnline::main() { window.main(); }

void DurakOnline::logout() {
  UpdateUserStatus(offline);
  current_user.to_null();
  window.login();
}

void DurakOnline::profile() {
  // пользователь не видит свой же статус и приглашение на игру
  window.InsertMessage(PROFILE);
  window.profile();
}

void DurakOnline::EnemyProfile() { window.EnemyProfile(); }

void DurakOnline::ChangeUserName() {
  std::string NewName = window.GetNewName().toStdString();
  if (NewName == "") {
    return;
  }
  pqxx::connection* session = make_session();
  pqxx::work tx(*session);
  pqxx::result username_check = tx.exec(
      "select * from users where username = $1 limit 1", pqxx::params{NewName});
  if (!username_check.empty()) {
    window.AddStateMessage(CreateMessage(
        "User with this username is already exists. Try another one", PROFILE,
        ERR, SMALLEST));
  } else {
    tx.exec("update users set username = $1 where id = $2",
            pqxx::params{NewName, current_user.get_id()});
    current_user.set_username(NewName);
    window.get_profile_Username().setText(
        QString::fromStdString(current_user.get_username()));
    tx.commit();
  }
  delete_session(session);
}

void DurakOnline::ChangePhoto() {
  QString NewPhoto = window.GetNewPhoto();
  if (NewPhoto != "") {
    QPixmap NewImg(NewPhoto);
    RoundedAvatar& OldAvatar = window.get_profile_RoundedAvatar();
    NewImg = CutPixMap(NewImg, OldAvatar.height(), OldAvatar.width());
    OldAvatar.setPixmap(NewImg);

    int inSize;
    char* inStr = pixmapToCharArray(NewImg, inSize);
    char* outStr = new char[(inSize * 4 / 3) + 5]{};

    uint32_t net_id = htonl(current_user.get_id());
    memcpy(outStr, &net_id, 4);  // записали сверху id

    int outLength = base64Encode(inStr, inSize, outStr + 4);
    Mark1 to_send;
    to_send.type = UPLOAD_PHOTO;
    to_send.data = outStr;  // данные удалятся вместе с диструктором Mark1
    to_send.length = outLength + 4;

    client.Client_Send(to_send);
    delete[] inStr;
    return;
  }
}

void DurakOnline::MakeMove() {
  if (IsMyTurn)  // Сейчас мой ход
  {
    if (FirstPosition == nullptr) {
      FirstPosition = static_cast<CellButton*>(sender());  // просто клетка
      if (!FirstPosition->get_figure()->is_valid()) {
        FirstPosition = nullptr;
        return;
      }
      if (FirstPosition->get_figure()->get_color() !=
          MyColor)  // не своя фигура
      {
        FirstPosition = nullptr;
        return;
      }
    } else  // Первая клетка получена
    {
      SecondPosition = static_cast<CellButton*>(sender());
      int current_row = FirstPosition->get_row();
      int current_column = FirstPosition->get_column();

      int last_row = SecondPosition->get_row();
      int last_column = SecondPosition->get_column();

      bool ans =
          board->move(current_row, current_column, last_row, last_column);
      if (ans)  // вызывать метод move у доски. Вставить в MOVE изменения
                // LastMove
      {
        Mark1 to_send;
        if (board->isCheckmate(current_row, current_column, last_row,
                               last_column)) {
          window.AddStateMessage(
              CreateMessage("You are win!", PLAY, SUCCES, BIG));
          board->replace(current_row, current_column, last_row, last_column);
          to_send.type = DataType::CHECKMATE;
        } else if (board->isValidCastling(current_row, current_column, last_row,
                                          last_column))  // Рокировка
        {
          window.AddStateMessage(CreateMessage("Clasting", PLAY, TXT, BIG));
          board->clasting(current_row, current_column, last_row, last_column);
          to_send.type = DataType::CLASTING;
        } else  // Обычный ход
        {
          QString message = QString::fromStdString(board->numericToAlgebraic(
              current_row, current_column, last_row, last_column));
          window.AddStateMessage(CreateMessage(message, PLAY, TXT, BIG));
          board->replace(current_row, current_column, last_row, last_column);
          to_send.type = DataType::MOVE;
        }

        window.InsertMessage(PLAY, false);
        window.UpdateBoard(*board, MyColor);

        char* new_board_serialize = board->SerializeMove();
        uint32_t net_session_id = htonl(session_id);
        char* data = new char[8];
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

void DurakOnline::GameOver() {
  UpdateUserStatus(online);
  uint32_t net_session_id = htonl(session_id);  // id сессии чтобы отключиться
  char* data = new char[4];
  memcpy(data, &net_session_id, 4);
  Mark1 to_send;
  to_send.type = GAMEOVER;
  to_send.length = 4;
  to_send.data = data;
  client.Client_Send(to_send);
  window.main();
}

void DurakOnline::Next() {
  UpdateUserStatus(looking_for);
  uint32_t net_session_id = htonl(session_id);  // id сессии чтобы отключиться
  char* data = new char[4];
  memcpy(data, &net_session_id, 4);
  Mark1 to_send;
  to_send.type = DataType::NEXT_ENEMY;
  to_send.length = 4;
  to_send.data = data;
  client.Client_Send(to_send);
}

void DurakOnline::StopFind() {
  UpdateUserStatus(online);
  uint32_t net_id =
      htonl(current_user.get_id());  // id сессии чтобы отключиться
  char* data = new char[4];
  memcpy(data, &net_id, 4);
  Mark1 to_send;
  to_send.type = DataType::STOP_FIND_ENEMY;
  to_send.length = 4;
  to_send.data = data;
  client.Client_Send(to_send);
  window.main();
}

void DurakOnline::DownloadPhoto(uint32_t id) {
  uint32_t net_id = htonl(id);
  char* data = new char[4];
  memcpy(data, &net_id, 4);
  Mark1 to_send;
  to_send.type = DataType::DOWNLOAD_PHOTO;
  to_send.length = 4;
  to_send.data = data;
  client.Client_Send(to_send);
}

void DurakOnline::FindEnemy() {
  UpdateUserStatus(looking_for);
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

void DurakOnline::InviteToGame() {
  ProfileButton* SenderId =
      static_cast<ProfileButton*>(sender());  // просто клетка

  Mark1 to_send;
  to_send.data = new char[8];

  uint32_t net_id = htonl(current_user.get_id());
  uint32_t net_enemy_id = htonl(SenderId->get_id());

  memcpy(to_send.data, &net_id, 4);            // мой id
  memcpy(to_send.data + 4, &net_enemy_id, 4);  // id соперника

  to_send.length = 8;
  to_send.type = DataType::INVITE;
  int bytes = client.Client_Send(to_send);
}

void DurakOnline::SendID() {
  Mark1 to_send;
  to_send.data = new char[4];

  uint32_t net_id = htonl(current_user.get_id());

  memcpy(to_send.data, &net_id, 4);  // мой id

  to_send.length = 4;
  to_send.type = DataType::ID;
  int bytes = client.Client_Send(to_send);
}

void DurakOnline::FillEnemyProfile(uint32_t enemy_id) {
  pqxx::connection* session = make_session();
  pqxx::work tx(*session);
  pqxx::result response =
      tx.exec("select username, rating, status from users where id = $1",
              pqxx::params{enemy_id});
  if (response.empty()) {
    window.AddStateMessage(
        CreateMessage("Nothing to find", MAIN, ERR, SMALLEST));
    window.InsertMessage(MAIN);
  } else {
    pqxx::row enemy = response[0];
    // Поменяли значения по доступу из window
    RoundedAvatar& EnemyImage = window.get_profile_EnemyAvatar();
    EnemyImage.setPixmap(QPixmap());
    QString EnemyName = QString::fromStdString(enemy[0].as<std::string>());
    QString EnemyRating = QString::fromStdString(enemy[1].as<std::string>());
    QString EnemyStatus =
        UserStatusToString((UserStatus)enemy[2].as<uint32_t>());
    QPalette PaletteStatus;
    QColor Colors[4] = {QColor(200, 200, 200), QColor(0, 255, 0),
                        QColor(255, 255, 255), QColor(255, 215, 0)};
    PaletteStatus.setColor(QPalette::WindowText,
                           Colors[enemy[2].as<uint32_t>()]);
    window.get_profile_Status().setText(EnemyStatus);
    window.get_profile_Status().setPalette(PaletteStatus);
    window.get_profile_EnemyId().setText("ID#" + QString::number(enemy_id));
    window.get_profile_EnemyName().setText(EnemyName);
    window.get_profile_EnemyRank().setText(EnemyRating);
    window.get_profile_Invite().set_id(enemy_id);
    // DownloadPhoto(enemy_id); // возможно фотка будет подгружаться после
    // отображения профиля. Все зависит от параметров сети
    window.EnemyProfile();
  }
  delete_session(session);
}

void DurakOnline::CheckEnemyProfile() {
  ProfileButton* bttn = static_cast<ProfileButton*>(sender());
  uint32_t enemy_id = bttn->get_id();
  FillEnemyProfile(enemy_id);
}

void DurakOnline::UpdateLeaderBoard() {
  pqxx::connection* session = make_session();
  pqxx::work tx(*session);
  pqxx::result response = tx.exec(
      "select username, id, rating from users order by rating desc limit 50");
  int idx = 0;
  for (auto i = response.begin(); i != response.end(); ++i) {
    std::string username = i[0].as<std::string>();
    uint32_t _id = i[1].as<uint32_t>();
    if (_id == current_user.get_id()) {
      username = "You";
    }
    window.UpdateLeaderBoard(username, _id, i[2].as<std::string>(), idx++);
  }
}

void DurakOnline::OnCloseWindow() {
  if (client.is_ready()) {
    if (Status == in_match) {
      GameOver();
    } else if (Status == looking_for) {
      StopFind();
    }
    UpdateUserStatus(offline);
    std::this_thread::sleep_for(
        std::chrono::milliseconds(100));  // Сервер должен успеть сработать
    client.Client_Disconnect();
    client.set_ready(false);  // Больше не слушаем сервер
  }
}

void DurakOnline::UpdateUserStatus(UserStatus status) {
  uint32_t user_id = current_user.get_id();
  if (user_id == 0) {
    return;
  }
  pqxx::connection* cx = make_session();
  if (cx == nullptr) {
    std::cerr << "Error: Could not connect to database for status update.\n";
    return;
  }
  try {
    pqxx::work tx{*cx};
    std::string sql_query = "UPDATE users SET status = $1 WHERE id = $2";
    tx.exec(sql_query, pqxx::params{(int)status, user_id});
    tx.commit();
    Status = status;
  } catch (const std::exception& e) {
    std::cerr << "Database Error in update_user_status: " << e.what() << '\n';
  }
  delete_session(cx);
}

void DurakOnline::SearchEnemyProfile() {
  QString SearchLineText = window.get_main_SearchLine().text();
  if (SearchLineText.isEmpty()) {
    window.AddStateMessage(
        CreateMessage("Nothing to find", MAIN, ERR, SMALLEST));
    window.InsertMessage(MAIN);
    return;
  }
  bool ok;
  uint32_t _id = SearchLineText.toULong(&ok);
  if (ok) {
    if (_id != current_user.get_id()) {
      FillEnemyProfile(_id);
    } else {
      window.profile();
    }
  } else {
    window.AddStateMessage(
        CreateMessage("Incorrect id! Try again", MAIN, ERR, SMALLEST));
    window.InsertMessage(MAIN);
    return;
  }
}

void DurakOnline::BadConnection() {
  MakeConnection();
  if (client.is_ready()) {
    window.login();
  } else {
    window.AddStateMessage(CreateMessage("Cannot to connect to play Server",
                                         BAD_CONNECTION, ERR, SMALLEST));
    window.BadConnection();
  }
}

void DurakOnline::connect() {
  QObject::connect(&window.get_reg_SubmitBttn(), &QPushButton::clicked, this,
                   &DurakOnline::registration);
  QObject::connect(&window.get_login_LoginBttn(), &QPushButton::clicked, this,
                   &DurakOnline::login);
  QObject::connect(&window.get_main_LogoutBttn(), &QPushButton::clicked, this,
                   &DurakOnline::logout);
  QObject::connect(&window.get_main_PlayBttn(), &QPushButton::clicked, this,
                   &DurakOnline::FindEnemy);
  QObject::connect(&window.get_play_StopBttn(), &QPushButton::clicked, this,
                   &DurakOnline::GameOver);
  QObject::connect(&window.get_play_NextBttn(), &QPushButton::clicked, this,
                   &DurakOnline::Next);
  QObject::connect(&window.get_wait_StopBttn(), &QPushButton::clicked, this,
                   &DurakOnline::StopFind);
  QObject::connect(&window.get_main_MyProfile(), &QPushButton::clicked, this,
                   &DurakOnline::profile);
  QObject::connect(&client, &Durak_Client::ServerSentData, this,
                   &DurakOnline::ServerGetData);
  QObject::connect(&LeaderBoardUpdateTimer, &QTimer::timeout, this,
                   &DurakOnline::UpdateLeaderBoard);
  QObject::connect(&window.get_profile_BackBttn(), &QPushButton::clicked, this,
                   &DurakOnline::main);
  QObject::connect(&window.get_profile_EnemyBackBttn(), &QPushButton::clicked,
                   this, &DurakOnline::main);
  QObject::connect(&window.get_profile_ChangeUnBttn(), &QPushButton::clicked,
                   this, &DurakOnline::ChangeUserName);
  QObject::connect(&window.get_profile_ChangePhoto(), &QPushButton::clicked,
                   this, &DurakOnline::ChangePhoto);
  QObject::connect(&window, &QWidget::destroyed, this,
                   &DurakOnline::OnCloseWindow);
  QObject::connect(&window.get_RetryBttn(), &QPushButton::clicked, this,
                   &DurakOnline::BadConnection);
  QObject::connect(&window.get_profile_Invite(), &QPushButton::clicked, this,
                   &DurakOnline::InviteToGame);
  QObject::connect(&window.get_main_SearchButton(), &QPushButton::clicked, this,
                   &DurakOnline::SearchEnemyProfile);
}

int DurakOnline::start() {
  connect();
  MakeConnection();
  if (!client.is_ready()) {
    window.AddStateMessage(CreateMessage("Cannot to connect to play Server",
                                         BAD_CONNECTION, ERR, SMALLEST));
    window.BadConnection();
  }
  window.setWindowTitle("Chess");
  window.connect();
  window.showMaximized();
  int answer = app.exec();
  return answer;
}