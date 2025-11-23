#include <list>
#include <queue>
#include <map>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <iostream>
#include <mutex>
#include <chrono>
#include "../Protocol/Protocol.hpp"
#include "Session.hpp"
#include "../../DataBase/database.hpp"
#define BACKLOG 128

// Убрать все нижние подчеркивания

class Durak_Server // основной поток слушает подключения и добавление в очередь
{
private:
    SOCKET server_socket;
    std::list<SOCKET> clients;
    std::queue<Player> line;
    std::map<uint32_t, Session *> play_sessions;
    void print(sockaddr *);
    void Server_Accept();
    void Make_Session(const Player &, const Player &);
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
