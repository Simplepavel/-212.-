#include "DataBase/database.hpp"
#include "Interface/Window.hpp"
#include "CurrentUser/CurrentUser.hpp"
#include <string>
#include <iostream>

class DurakOnline : public QObject
{
    Q_OBJECT
private:
    QApplication app;
    Window window;
    CurrentUser current_user;
    void connect();
private slots:
    bool registration();
    bool login();
    void logout();

public:
    DurakOnline(int argc, char *argv[]);
    int start();
};
