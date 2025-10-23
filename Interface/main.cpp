#include <QApplication>
#include "Window.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window MainWindow;
    MainWindow.login();
    MainWindow.showMaximized();
    MainWindow.connect();
    return app.exec();
}
