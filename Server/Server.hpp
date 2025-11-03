#include <list>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <iostream>
#include <mutex>
#include <chrono>
#define BACKLOG 128

class Durak_Server // основной поток слушает подключения и добавление в очередь
{
private:
    SOCKET server_socket;
    std::list<SOCKET> clients;
    void print(sockaddr *);
    void Server_Accept();
    std::mutex mtx;

public:
    Durak_Server();
    ~Durak_Server();
    bool Server_Bind(char *IP, char *PORT);
    bool Server_Start_Listen();
    void Server_Go();
};
