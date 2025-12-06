#include <list>
#include <queue>
#include <map>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <iostream>
#include <mutex>
#include <chrono>
#include <fstream>
#include "../Protocol/Protocol.hpp"
#include "Session.hpp"
#include "../../DataBase/database.hpp"
#include "../Base64/Base64.hpp"
#define BACKLOG 128

// Убрать все нижние подчеркивания

class Durak_Server // основной поток слушает подключения и добавление в очередь
{
private:
    SOCKET server_socket;
    std::map<uint32_t, SOCKET> ties; // пары id - сокет
    std::list<SOCKET> clients;
    std::list<Player> line;
    std::map<int32_t, int32_t> rating;

    // Система подсчета рейнтинга

    std::map<uint32_t, Session *> play_sessions;
    void print(sockaddr *);
    void Server_Accept();
    void Make_Session(Player &, Player &);
    Mark1 MakeStartPacket(pqxx::work &tx, const Player &pl, uint32_t session_id, bool);
    std::mutex mtx;

public:
    Durak_Server();
    ~Durak_Server();
    bool Server_Bind(char *IP, char *PORT);
    bool Server_Start_Listen();
    int Server_Send(const Mark1 &, int);
    void Server_Go();
};
