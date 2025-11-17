#include "DataBase/database.hpp"
#include "Interface/Window.hpp"
#include "CurrentUser/CurrentUser.hpp"
#include "BackEnd/Client/Client.hpp"

#include <string>
#include <iostream>
#include <thread>

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

    void connect();
private slots:
    bool registration();
    bool login();
    void logout();
    void FindEnemy();
    void play();
    void MakeMove();

public:
    DurakOnline(int argc, char *argv[]);
    int start();
};
