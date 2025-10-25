#include "DataBase/database.hpp"
#include "Interface/Window.hpp"
#include "string"


class DurakOnline : public QObject
{
    Q_OBJECT
private:
    QApplication app;
    Window window;
    void connect();
private slots:
    bool registration();

public:
    DurakOnline(int argc, char *argv[]);
    int start();
};