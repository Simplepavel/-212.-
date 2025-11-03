#include "Client.hpp"

int main()
{
    Durak_Client cl;
    char ip[] = "127.0.0.1";
    char port[] = "7538";
    cl.Client_Connect(ip, port);
    char data[4096];
    char ans;
    while (ans != 'y')
    {
        std::cout << ">";
        std::cin.getline(data, sizeof(data));
        if (strlen(data) == 0)
        {
            continue;
        }
        cl.Client_Send(data);
        cl.Client_Recv();
        std::cout << "Stop(y/n)?";
        std::cin >> ans;
    }
    cl.Client_Disconnect();
    return 0;
}