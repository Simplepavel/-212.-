// #pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <QObject>
#include "../Protocol/Protocol.hpp"
#include <cstring>
class Durak_Client : public QObject
{
    Q_OBJECT
private:
    SOCKET client_socket;
    void print(sockaddr *);
    bool ready; // Слушаем ли сервер
    char *buffer = nullptr;
signals:
    void ServerSentData();

public:
    Durak_Client();
    bool Client_Connect(char *IP, char *PORT);
    bool Client_Disconnect();
    int Client_Send(const Mark1 &);
    void Client_Listen();
    void set_ready(bool new_value) { ready = new_value; };
    bool is_ready() { return ready; }
    char *GetData();
};