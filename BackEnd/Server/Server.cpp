#include "Server.hpp"

std::string url_base = "postgresql://postgres:NiPWYEfWWdhjhkATtEeg-g7ZD@localhost:5432/durak";

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
                    char data[105]{0};
                    int bytes = recv(*i, data, sizeof(data), 0);
                    if (bytes > 0)
                    {
                        Mark1 recv_data = Mark1::deserialize(data);
                        if (recv_data.type == FIND_ENEMY)
                        {
                            Player pl1;
                            uint32_t net_id;
                            memcpy(&net_id, recv_data.data, 4);
                            pl1.id = ntohl(net_id);
                            pl1.fd = *i;
                            if (!line.empty()) // первый типок из очереди будет твои соперником
                            {
                                Player pl2 = line.front();
                                line.pop();

                                Session *new_session = new Session(pl1, pl2);
                                play_sessions[new_session->id] = new_session;

                                pqxx::connection *database_session = make_session(url_base);
                                pqxx::work tx(*database_session);

                                pqxx::result username1 = tx.exec("select username from users where id=$1", pqxx::params{pl1.id});
                                pqxx::result username2 = tx.exec("select username from users where id=$1", pqxx::params{pl2.id});

                                std::string u1 = username1[0][0].as<std::string>();
                                std::string u2 = username2[0][0].as<std::string>();

                                char *packet1 = new char[u1.size() + 4];
                                char *packet2 = new char[u2.size() + 4];

                                uint32_t net_session_id = htonl(new_session->id);

                                memcpy(packet1, &net_session_id, 4);
                                memcpy(packet1 + 4, &u1[0], u1.size());

                                memcpy(packet2, &net_session_id, 4);
                                memcpy(packet2 + 4, &u2[0], u2.size());

                                std::cout << "Send START to user " << u1 << '\n';
                                Mark1 to_send1;
                                to_send1.length = u2.size() + 4;
                                to_send1.type = DataType::START;
                                to_send1.data = packet2;
                                Server_Send(to_send1, pl1.fd);

                                std::cout << "Send START to user " << u2 << '\n';

                                Mark1 to_send2;
                                to_send2.type = DataType::START;
                                to_send2.data = packet1;
                                to_send2.length = u1.size() + 4;
                                Server_Send(to_send2, pl2.fd);

                                delete[] packet1;
                                delete[] packet2;
                            }
                            else
                            {
                                line.push(pl1);
                                std::cout << pl1.id << '\n';
                            }
                            // ToDo Добавление клиента в очередь(Если она уже не пуста, значит соединение и начало игры)
                        }
                    }
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
                        std::cout << "Something went wrong with client ";
                        sockaddr_storage their_addr;
                        socklen_t their_len = sizeof(their_addr);
                        getpeername(*i, (sockaddr *)&their_addr, &their_len);
                        print((sockaddr *)&their_addr);
                        std::cout << '\n';
                        closesocket(*i);
                        delete_clients.push_back(*i);
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

int Durak_Server::Server_Send(const Mark1 &data, int fd)
{
    char *mark1_serialize = data.serialize();
    int result = send(fd, mark1_serialize, data.capacity(), 0);
    delete[] mark1_serialize;
    return result;
}