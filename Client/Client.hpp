#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
class Durak_Client
{
private:
    SOCKET client_socket;
    void print(sockaddr *);

public:
    Durak_Client();
    bool Client_Connect(char *IP, char *PORT);
    bool Client_Disconnect();
    int Client_Send(char *data);
    int Client_Recv();
};