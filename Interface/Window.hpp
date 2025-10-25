#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSizePolicy>
#include <QDebug>
#include <QStackedLayout>
#include <QApplication>

class Window : public QWidget
{
private:
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

    QLineEdit *main_SearchRoom;
    QPushButton *main_FindRoomBttn;
    QPushButton *main_CreateRoomBttn;
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

public:
    Window(QWidget *parent = nullptr);
    void login();
    void main();
    void connect();
    void registration();

    // Констатнтный доступ к элементам формы регистрации
    const QLineEdit &get_reg_Username() { return *reg_Username; }
    const QLineEdit &get_reg_Email() { return *reg_Email; }
    const QLineEdit &get_reg_Password() { return *reg_Password; }
    const QLineEdit &get_reg_ConfirmPassword() { return *reg_ConfirmPassword; }
    const QPushButton &get_reg_SubmitBttn() { return *reg_SubmitBttn; } // Чтобы привязать к ней действие
    // Констатнтный доступ к элементам формы регистрации
};