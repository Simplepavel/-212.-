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
#include <QDialog>
#include <QFileDialog>

#include <QFont>
#include <QPalette>
#include <QColor>
#include <QList>
#include <QPixmap>
#include <QTimer>
#include <QScrollArea>

#include "../BackEnd/Game/Board/Board.hpp"
#include "CellButton/CellButton.hpp"
#include "ProfileButton/ProfileButton.hpp"
#include "RoundedAvatar/RoundedAvatar.hpp"
#include "Utils.hpp"

#include <list>

class Window : public QWidget
{
private:
    // Неигровые параметры
    QScreen *screen_Objc;
    int screen_Width;       // Ширина экрана
    int screen_Height;      // Высота экрана
    uint32_t CurrentUserId; // id текущего пользователя
    int cell_Size;          // Высота X Ширина игровой клетки
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
    QHBoxLayout *profile_Layout;

    QWidget *profile_UserInfoWidget;
    QVBoxLayout *profile_UserInfoLayout;

    QLabel *profile_id;
    RoundedAvatar *profile_Avatar;

    QHBoxLayout *profile_UsernameLayout;
    QPushButton *profile_ChangeUsernameBttn;
    QLabel *profile_Username;

    QPushButton *profile_ChangePhoto;
    QLabel *profile_Rank;
    QPushButton *profile_BackBttn;
    // Профиль

    // Профиль соперника
    QWidget *ProfileEnemyWidget;
    QHBoxLayout *ProfileEnemyLayout;

    QPushButton *profile_EnemyBackBttn;

    QWidget *profile_EnemyInfoWidget;
    QVBoxLayout *profile_EnemyInfoLayout;

    QLabel *profile_EnemyId;

    RoundedAvatar *profile_EnemyAvatar;
    QLabel *profile_EnemyName;

    QLabel *profile_EnemyRank;
    QLabel *profile_Status;

    QPushButton *profile_Invite;

    // Профиль соперника

    // Плохое соединение.
    QWidget *bad_ConnectionWidget;
    QVBoxLayout *bad_ConnectionLayout;

    QPushButton *bad_Retry;

    // Плохое соединение.

private slots:
    void UpdateWaitLabel();

public:
    Window(QWidget *parent = nullptr);
    void SetUserId(int new_id) { CurrentUserId = new_id; }
    void login();
    void main();
    void connect();
    void registration();
    void play();
    void wait();
    void profile();
    void EnemyProfile();
    void BadConnection();
    void InsertMessage(Owners, bool DeleteOld = true); // Вставка предупрждений
    void AddStateMessage(const Message &);

    QString GetNewName();
    QString GetNewPhoto();

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
    const QPushButton &get_main_LogoutBttn() { return *main_LogoutBttn; }
    const QPushButton &get_main_PlayBttn() { return *main_PlayBttn; }
    const QPushButton &get_main_MyProfile() { return *main_MyProfile; }
    std::vector<ProfileButton *> FillLeaderBoard(); // заполнение доски лидеров шаблонами для вставки
    QVBoxLayout &get_main_LeaderBoardLayout() { return *main_LeaderBoardLayout; }
    void UpdateLeaderBoard(const std::string &username, uint32_t, const std::string &rating, int idx);
    // Константный доступ к элементам формы главного окна

    // Метод относящиеся к игровому полю
    void UpdateBoard(Board &NewBoard, FigureColor MyColor);
    QLabel &get_play_EnemyName() { return *play_EnemyName; }
    std::vector<CellButton *> FillBoard(); // вызывать при START и получать список кнопок для connect
    const QPushButton &get_play_StopBttn() { return *play_StopBttn; };
    const QPushButton &get_play_NextBttn() { return *play_NextBttn; };
    // Метод отсносящиеся к игровому полю

    // Методы относящиеся к профилю
    const QPushButton &get_profile_BackBttn() { return *profile_BackBttn; }
    QLabel &get_profile_id() { return *profile_id; }
    QLabel &get_profile_Username() { return *profile_Username; }
    QLabel &get_profile_Rank() { return *profile_Rank; }
    QPushButton &get_profile_ChangeUnBttn() { return *profile_ChangeUsernameBttn; }
    QPushButton &get_profile_ChangePhoto() { return *profile_ChangePhoto; }
    RoundedAvatar &get_profile_RoundedAvatar() { return *profile_Avatar; }
    // Методы относящиеся к профилю

    // Методы относящиеся к профилю соперника
    const QPushButton &get_profile_EnemyBackBttn() { return *profile_EnemyBackBttn; }
    const QPushButton &get_profile_Invite() { return *profile_Invite; }
    QLabel &get_profile_EnemyName() { return *profile_EnemyName; }
    QLabel &get_profile_EnemyRank() { return *profile_EnemyRank; }
    QLabel &get_profile_Status() { return *profile_Status; }
    RoundedAvatar &get_profile_EnemyAvatar() { return *profile_EnemyAvatar; }
    QLabel &get_profile_EnemyId() { return *profile_EnemyId; }
    // Методы относящиеся к профилю соперника

    //  Методы относящиеся к полю ожидания
    QTimer &get_wait_Timer() { return *wait_Timer; }
    const QPushButton &get_wait_StopBttn() { return *wait_StopBttn; }
    // Методы относящиеся к полю ожидания

    // Методы плохого соединения
    const QPushButton &get_RetryBttn() { return *bad_Retry; }
    // Методы плохого соединения
};
