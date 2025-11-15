#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSizePolicy>
#include <QDebug>
#include <QStackedLayout>
#include <QApplication>
#include <QLabel>
#include <QScrollArea>
#include <QScreen>
#include <QRect>

class Window : public QWidget
{
private:
    // Неигровые параметры
    QScreen *screen_Objc;
    int screen_Width;  // Ширина
    int screen_Height; // Высота
    // Неигровые параметры
    // Вспомогательные элементы
    QStackedLayout *listOfLayout;
    // Вспомогательные элементы

    // Вход
    QWidget *login_Widget;
    QVBoxLayout *login_Layout;
    QLineEdit *login_Email;
    QLineEdit *login_Password;
    QPushButton *login_LoginBttn;
    QPushButton *login_RegBttn;
    // Вход

    // Главное меню
    QWidget *main_Widget;
    QVBoxLayout *main_Layout;

    QPushButton *main_PlayBttn;
    QPushButton *main_LogoutBttn;
    // Главное меню

    // Регистрация
    QWidget *reg_Widget;
    QVBoxLayout *reg_Layout;

    QLineEdit *reg_Username;
    QLineEdit *reg_Email;
    QLineEdit *reg_Password;
    QLineEdit *reg_ConfirmPassword;
    QPushButton *reg_SubmitBttn;
    QPushButton *reg_BackBttn;
    // Регистрация

    // Игровое полотно
    QWidget *play_Widget;
    QVBoxLayout *play_Layout;
    QLabel *play_EnemyName;
    QGridLayout *play_BoardLayot;
    QPushButton *play_NextBttn;
    QPushButton *play_StopBttn;
    // Игровое полотно

public:
    Window(QWidget *parent = nullptr);
    void login();
    void main();
    void connect();
    void registration();
    void play();

    // Констатнтный доступ к элементам формы регистрации
    const QLineEdit &get_reg_Username() { return *reg_Username; }
    const QLineEdit &get_reg_Email() { return *reg_Email; }
    const QLineEdit &get_reg_Password() { return *reg_Password; }
    const QLineEdit &get_reg_ConfirmPassword() { return *reg_ConfirmPassword; }
    const QPushButton &get_reg_SubmitBttn() { return *reg_SubmitBttn; } // Чтобы привязать к ней действие
    // Констатнтный доступ к элементам формы регистрации

    // Константный доступ к элементам форма входа
    const QLineEdit &get_login_Email() { return *login_Email; }
    const QLineEdit &get_login_Password() { return *login_Password; }
    const QPushButton &get_login_LoginBttn() { return *login_LoginBttn; }
    // Константный доступ к элементам форма входа

    // Константный доступ к элементам формы главного окна
    // const QLineEdit& get_main_SearchRoom(){return *main_SearchRoom;}
    // const QPushButton& get_main_FindRoomBttn(){return *main_FindRoomBttn;}
    // const QPushButton& get_main_CreateRoomBttn(){return *main_CreateRoomBttn;}
    const QPushButton &get_main_LogoutBttn() { return *main_LogoutBttn; }
    const QPushButton &get_main_PlayBttn() { return *main_PlayBttn; }
    // Константный доступ к элементам формы главного окна

};
