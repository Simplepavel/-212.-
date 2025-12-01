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

#include <QFont>
#include <QPalette>
#include <QColor>
#include <QList>
#include <QPixmap>
#include <QTimer>
#include <QScrollArea>

#include "../BackEnd/Game/Board/Board.hpp"
#include "MyPushButton/MyPushButton.hpp"
#include "RoundedAvatar/RoundedAvatar.hpp"
#include "Utilts.hpp"

#include <list>

class Window : public QWidget
{
private:
    // Неигровые параметры
    QScreen *screen_Objc;
    int screen_Width;  // Ширина экрана
    int screen_Height; // Высота экрана

    int cell_Size; // Высота X Ширина игровой клетки
    std::list<Message> StateMessages;

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
    QHBoxLayout *main_Layout;

    // ЛидерБорд
    QWidget *main_LeaderBoardWidget;
    QVBoxLayout *main_LeaderBoardLayout;
    QScrollArea *main_LeaderBoardScroll;

    QLabel *main_LeaderBoardLabel;
    QHBoxLayout *main_SearchLayout;
    QLineEdit *main_SearchLine;
    QPushButton *main_SearchButton;

    // ЛидерБорд

    QVBoxLayout *main_ProfileLayout;
    QPushButton *main_PlayBttn;
    QPushButton *main_LogoutBttn;
    QPushButton *main_MyProfile;
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

    QHBoxLayout *play_Layout;
    QWidget *play_Widget;

    QScrollArea *play_ScrollArea;
    QVBoxLayout *play_ScrollLayout;
    QWidget *play_ScrollWidget;

    QVBoxLayout *play_GameLayout;
    QLabel *play_EnemyName;
    QGridLayout *play_BoardLayot;
    QPushButton *play_NextBttn;
    QPushButton *play_StopBttn;

    // Игровое полотно

    // Окно ожидание соперника
    QWidget *wait_Widget;
    QVBoxLayout *wait_Layout;
    QLabel *wait_Label; // здесь надпись: Идет поиск соперника. Ожидайте...
    QPushButton *wait_StopBttn;
    QTimer *wait_Timer;
    // Окно ожидание соперника

    // Профиль
    QWidget *profile_Widget;
    QVBoxLayout *profile_Layout;

    RoundedAvatar* profile_Avatar;
    QPushButton* profile_ChangePhoto;
    
    QLabel* profile_UsernameLabel;
    QLineEdit* profile_UsernameLineEdit;
    
    QLabel* profile_EmailLabel;
    QLineEdit* profile_EmailLineEdit;
    QLabel* profile_Rating;
    QLabel* profile_Rank;
    // Профиль

private slots:
    void UpdateWaitLabel();

public:
    Window(QWidget *parent = nullptr);
    void login();
    void main();
    void connect();
    void registration();
    void play();
    void wait();
    void profile();
    void InsertMessage(Owners, bool DeleteOld = true); // Вставка предупрждений
    void AddStateMessage(const Message &);

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
    const QPushButton &get_main_MyProfile() {return *main_MyProfile;}
    void FillLeaderBoard(); // заполнение доски лидеров шаблонами для вставки
    QVBoxLayout &get_main_LeaderBoardLayout() { return *main_LeaderBoardLayout; }
    void UpdateLeaderBoard(const std::string &username, const std::string &rating, int idx);
    // Константный доступ к элементам формы главного окна

    // Метод относящиеся к игровому полю
    void UpdateBoard(Board &NewBoard, FigureColor MyColor);
    QLabel &get_play_EnemyName() { return *play_EnemyName; }
    std::vector<MyPushButton *> FillBoard(); // вызывать при START и получать список кнопок для connect
    const QPushButton &get_play_StopBttn() { return *play_StopBttn; };
    const QPushButton &get_play_NextBttn() { return *play_NextBttn; };
    // Метод отсносящиеся к игровому полю

    // Методы относящиеся к полю ожидания
    QTimer &get_wait_Timer() { return *wait_Timer; }
    const QPushButton &get_wait_StopBttn() { return *wait_StopBttn; }
    // Методы относящиеся к полю ожидания
};
