#include "Server.hpp"

int main(int argc, char *argv[])
{
    Durak_Server server;
    char port[] = "7538";
    char ip[] = "127.0.0.1";
    server.Server_Bind(ip, port);
    server.Server_Start_Listen();
    server.Server_Go();
    return 0;
}