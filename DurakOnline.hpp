#include "DataBase/database.hpp"
#include "Interface/Window.hpp"
#include "CurrentUser/CurrentUser.hpp"
#include "BackEnd/Client/Client.hpp"
#include "RatingSystem/rating.hpp"
#include "Backend/Base64/Base64.hpp"
#include <string>
#include <iostream>
#include <thread>
#include <fstream>

enum UserStatus : uint8_t
{
    offline,
    online,
    looking_for,
    in_match
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
    CellButton *FirstPosition;
    CellButton *SecondPosition;
    QTimer LeaderBoardUpdateTimer;
    UserStatus Status; // В игре. Ожидает соперника. Просто онлайн

    void connect();
private slots:
    void registration();
    void login();
    void logout();
    void ServerGetData();
    void profile();
    void EnemyProfile();
    void main();
    void MakeConnection();
    void MakeMove();
    void CheckEnemyProfile();
    void UpdateLeaderBoard();
    void ChangeUserName();
    void ChangePhoto();
    void OnCloseWindow();
    void BadConnection();

    // Возможно объединить это в одну фунцию
    void GameOver();
    void Next();
    void StopFind();
    void DownloadPhoto(uint32_t id); // Загрузка фото с сервера
    void FindEnemy();
    void UpdateUserStatus(UserStatus status);

public:
    DurakOnline(int argc, char *argv[]);
    int start();
};
