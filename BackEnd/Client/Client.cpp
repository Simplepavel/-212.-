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
    char buffer[256];
    while (ready)
    {
        int bytes = ::recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes > 0)
        {
            Mark1 result = Mark1::deserialize(buffer);

            uint32_t net_session_id;
            memcpy(&net_session_id, result.data, 4);
            uint32_t session_id = ntohl(net_session_id);
            std::cout << "============Start info========\n";
            std::cout << "Session id " << session_id << '\n';

            bool AmIWhite = result.data[4] == 1;
            std::cout << "I move first: " << AmIWhite << '\n';

            std::string enemy_name;
            enemy_name.resize(result.length - 5);
            memcpy(&enemy_name[0], result.data + 5, result.length - 5);
            std::cout << "Enemy name " << enemy_name << '\n';

            // Получить доску и кто ходит первый
            // Породить сигнал и обработав этот сигнал в DurakOnline изобрать игровое поле со всеми данными
        }
    }
}