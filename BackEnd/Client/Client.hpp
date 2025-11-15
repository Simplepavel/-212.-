#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "../Protocol/Protocol.hpp"
class Durak_Client
{
private:
    SOCKET client_socket;
    void print(sockaddr *);
    bool ready;

public:
    Durak_Client();
    bool Client_Connect(char *IP, char *PORT);
    bool Client_Disconnect();
    int Client_Send(const Mark1 &);
    void Client_Listen();
    void set_ready(bool new_value) { ready = new_value; }
};