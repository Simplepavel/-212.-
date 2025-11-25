#include "Client.hpp"

Durak_Client::Durak_Client() : ready(false)
{
    WORD version = MAKEWORD(2, 2);
    WSAData specific_data;
    WSAStartup(version, &specific_data);
}

bool Durak_Client::Client_Connect(char *IP, char *PORT)
{
    int error;
    ADDRINFO hints{};
    ADDRINFO *iter;
    ADDRINFO *result = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    error = ::getaddrinfo(IP, PORT, &hints, &result);

    for (iter = result; iter != nullptr; iter = iter->ai_next)
    {
        client_socket = ::socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol);
        if (client_socket == INVALID_SOCKET)
        {
            continue;
        }
        if (::connect(client_socket, iter->ai_addr, iter->ai_addrlen) != 0)
        {
            ::closesocket(client_socket);
            continue;
        }
        break;
    }
    if (iter == nullptr)
    {
        std::cout << "Cannot connect to " << IP << ':' << PORT << '\n';
        ::freeaddrinfo(result);
        return false;
    }
    std::cout << "Succesful connect with ";
    print(iter->ai_addr);
    std::cout << '\n';
    freeaddrinfo(result);
    return true;
}

bool Durak_Client::Client_Disconnect()
{
    ::shutdown(client_socket, SD_BOTH);
    char empty_buffer[4096]{0};
    int recived_bytes = 0;
    do
    {
        recived_bytes = recv(client_socket, empty_buffer, sizeof(empty_buffer), 0);
    } while (recived_bytes > 0);
    return (::closesocket(client_socket) == 0);
}

void Durak_Client::print(sockaddr *addr)
{
    char result[INET6_ADDRSTRLEN]{0};
    unsigned short port;
    if (addr->sa_family == AF_INET)
    {
        ::inet_ntop(AF_INET, &((sockaddr_in *)addr)->sin_addr, result, sizeof(result));
        port = ::ntohs(((sockaddr_in *)addr)->sin_port);
    }
    else
    {
        ::inet_ntop(AF_INET6, &((sockaddr_in6 *)addr)->sin6_addr, result, sizeof(result));
        port = ::ntohs(((sockaddr_in *)addr)->sin_port);
    }
    std::cout << result << ":" << port;
}

int Durak_Client::Client_Send(const Mark1 &data)
{
    char *mark1_serialize = data.serialize();
    int result = ::send(client_socket, mark1_serialize, data.capacity(), 0);
    delete[] mark1_serialize;
    return result;
}

void Durak_Client::Client_Listen()
{
    while (ready)
    {
        int bytes = ::recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes > 0)
        {
            emit ServerSentData();
        }
    }
}

char *Durak_Client::GetData()
{
    return buffer;
}