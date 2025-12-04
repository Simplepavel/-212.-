#include "DataBase/database.hpp"
#include "Interface/Window.hpp"
#include "CurrentUser/CurrentUser.hpp"
#include "BackEnd/Client/Client.hpp"

#include <string>
#include <iostream>
#include <thread>

enum UserStatus : uint8_t
{
    online = 0,       // просто в сети(зеленым цветом)
    offline = 1,          // не в сети(серым цветом)
    in_match = 2,         // играет(желтым)
    looking_for_match = 3 // поиск соперника(желтым)
};

class DurakOnline : public QObject
{
    Q_OBJECT
private:
    QApplication app;
    Window window;
    Durak_Client client;
    CurrentUser current_user;
    Board *board;
    uint32_t session_id; // по умолчанию равно 0. Это значит что игрок не в игровой сессии
    bool IsMyTurn;
    FigureColor MyColor; // white - белый, black - черный

    MyPushButton *FirstPosition;
    MyPushButton *SecondPosition;

    QTimer LeaderBoardUpdateTimer;

    void connect();

    void update_user_status(UserStatus status); // обновление статуса пользователя на сервере
private slots:
    void registration();
    void login();
    void logout();
    void FindEnemy();
    void play();
    void profile();
    void MakeMove();
    void UpdateLeaderBoard();

    // Возможно объединить это в одну фунцию
    void Disconnect();
    void Next();
    void StopFind();

public:
    DurakOnline(int argc, char *argv[]);
    int start();
};
