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

    void connect();
private slots:
    bool registration();
    bool login();
    void logout();
    void FindEnemy();
    void play();

public:
    DurakOnline(int argc, char *argv[]);
    int start();
};
