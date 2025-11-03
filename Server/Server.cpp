#include "Server.hpp"

Durak_Server::Durak_Server() : server_socket(INVALID_SOCKET)
{
    WORD version = MAKEWORD(2, 2);
    WSAData specific_data;
    WSAStartup(version, &specific_data);
}

Durak_Server::~Durak_Server()
{
    if (server_socket != INVALID_SOCKET)
    {
        shutdown(server_socket, 2);
        closesocket(server_socket); // сделать более graceful
    }
    WSACleanup();
}

bool Durak_Server::Server_Bind(char *IP, char *PORT)
{
    int error;
    ADDRINFO hints{};
    ADDRINFO *iter;
    ADDRINFO *result = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    error = getaddrinfo(IP, PORT, &hints, &result);
    if (error != 0)
    {
        std::cout << "Can not getaddrinfo " << PORT << ':' << IP << '\n';
        return false;
    }
    for (iter = result; iter != nullptr; iter = iter->ai_next)
    {
        server_socket = socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol);
        if (server_socket == INVALID_SOCKET)
        {
            continue;
        }
        if (bind(server_socket, iter->ai_addr, iter->ai_addrlen) != 0)
        {
            closesocket(server_socket);
            continue;
        }
        break;
    }
    if (iter == nullptr)
    {
        std::cout << "Cannot bind to " << IP << ':' << PORT << '\n';
        freeaddrinfo(result);
        return false;
    }
    std::cout << "Succesful binding with ";
    print(iter->ai_addr);
    std::cout << '\n';
    freeaddrinfo(result);
    return true;
}

bool Durak_Server::Server_Start_Listen()
{
    int error = listen(server_socket, BACKLOG);
    if (error != 0)
    {
        std::cout << "Failed listen";
        return false;
    }
    return true;
}

void Durak_Server::Server_Accept()
{
    sockaddr_storage their_addr{};
    socklen_t their_len = sizeof(sockaddr_storage);
    int cs;
    std::cout << "Wait for connection\n";
    while (true)
    {
        cs = accept(server_socket, (sockaddr *)&their_addr, &their_len); // client socket
        if (cs != INVALID_SOCKET)
        {
            std::cout << "New connection from: ";
            print((sockaddr *)&their_addr);
            std::cout << '\n';
            std::lock_guard<std::mutex> lock(mtx);
            clients.push_back(cs);
        }
    }
}

void Durak_Server::print(sockaddr *addr)
{
    char result[INET6_ADDRSTRLEN]{0};
    unsigned short port;
    if (addr->sa_family == AF_INET)
    {
        inet_ntop(AF_INET, &((sockaddr_in *)addr)->sin_addr, result, sizeof(result));
        port = ntohs(((sockaddr_in *)addr)->sin_port);
    }
    else
    {
        inet_ntop(AF_INET6, &((sockaddr_in6 *)addr)->sin6_addr, result, sizeof(result));
        port = ntohs(((sockaddr_in *)addr)->sin_port);
    }
    std::cout << result << ":" << port;
}

void Durak_Server::Server_Go()
{
    std::thread thr1(&Durak_Server::Server_Accept, std::ref(*this));
    fd_set master;
    std::list<SOCKET> copy_clients;
    int max_socket;
    std::list<SOCKET> delete_clients; // для удаления
    while (true)
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            copy_clients = clients;
        }
        if (copy_clients.empty())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            continue;
        }
        max_socket = -1;
        FD_ZERO(&master);
        for (auto i = copy_clients.begin(); i != copy_clients.end(); ++i)
        {
            FD_SET(*i, &master);
            if (max_socket < *i)
            {
                max_socket = *i;
            }
        }
        timeval time;
        time.tv_sec = 0;
        time.tv_usec = 500000;

        int ready_read = select(max_socket + 1, &master, nullptr, nullptr, &time);
        if (ready_read > 0)
        {
            for (auto i = copy_clients.begin(); i != copy_clients.end(); ++i)
            {
                if (FD_ISSET(*i, &master))
                {
                    char data[4096]{0};
                    int bytes = recv(*i, data, sizeof(data), 0);
                    if (bytes > 0)
                    {
                        send(*i, data, strlen(data), 0);
                    } // все хорошо, отправляем назад прочитанные данные
                    else if (bytes == 0)
                    {
                        std::cout << "Client ";
                        sockaddr_storage their_addr;
                        socklen_t their_len = sizeof(their_addr);
                        getpeername(*i, (sockaddr *)&their_addr, &their_len);
                        print((sockaddr *)&their_addr);
                        std::cout << " has disconnected\n";
                        closesocket(*i);
                        delete_clients.push_back(*i);
                    } // клиент отключился
                    else
                    {
                        std::cout << "Something went wrong with client";
                        sockaddr_storage their_addr;
                        socklen_t their_len = sizeof(their_addr);
                        getpeername(*i, (sockaddr *)&their_addr, &their_len);
                        print((sockaddr *)&their_addr);
                    } // что то пошло не так
                }
            }
            mtx.lock();
            for (auto i = delete_clients.begin(); i != delete_clients.end(); ++i)
            {
                clients.remove(*i);
            }
            mtx.unlock();
        }
        else if (ready_read == 0)
        {
            continue;
        }
        else
        {
            std::cout << "Something went wrong with select";
        }
    }
    thr1.join();
}
