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
                    char NetCapacityBytes[4]{0};
                    uint32_t NetCapacity;
                    recv(*i, NetCapacityBytes, 4, 0);

                    memcpy(&NetCapacity, NetCapacityBytes, 4);
                    uint32_t Capacity = ntohl(NetCapacity);

                    char data[Capacity]{0};
                    int bytes = recv(*i, data, Capacity, 0);

                    if (bytes > 0)
                    {
                        Mark1 recv_data = Mark1::deserialize(data);
                        if (recv_data.type == DataType::FIND_ENEMY)
                        {
                            uint32_t net_id;
                            memcpy(&net_id, recv_data.data, 4);
                            Player pl1(ntohl(net_id), *i);
                            if (!line.empty())
                            {
                                Player pl2 = line.front();
                                line.erase(line.begin());
                                Make_Session(pl2, pl1);
                            }
                            else
                            {
                                line.push_back(pl1);
                            }
                            rating[pl1.id] = 0;
                        }
                        else if (recv_data.type == DataType::CHECKMATE)
                        {
                            uint32_t net_session_id;
                            memcpy(&net_session_id, recv_data.data, 4);
                            uint32_t session_id = ntohl(net_session_id);

                            Session *play_session = play_sessions[session_id];
                            SOCKET opp_socket = play_session->Reciver(*i).fd;
                            int send_data = Server_Send(recv_data, opp_socket); // отправили ход

                            Player winner = play_session->Sender(*i);
                            Player loser = play_session->Reciver(*i);

                            rating[winner.id] += (30 + rand() % 2);
                            rating[loser.id] -= (15 + rand() % 2);
                            play_session->IsFinished = true; // Сессия закочена победой одного из игроков

                            // Помимо отправки хода еще какие нибудь действия
                            // Удалять ссесия не надо. Она удалиться как только один игрок нажмет Next или Stop
                        }
                        else if (recv_data.type == DataType::MOVE || recv_data.type == DataType::CLASTING)
                        {
                            uint32_t net_session_id;
                            memcpy(&net_session_id, recv_data.data, 4);
                            uint32_t session_id = ntohl(net_session_id);

                            Session *play_session = play_sessions[session_id];
                            SOCKET opp_socket = play_session->Reciver(*i).fd;

                            int send_data = Server_Send(recv_data, opp_socket);
                        }
                        else if (recv_data.type == DataType::DISCONNECT)
                        {
                            uint32_t net_session_id;
                            memcpy(&net_session_id, recv_data.data, 4);
                            uint32_t session_id = ntohl(net_session_id);

                            Session *play_session = play_sessions[session_id];

                            SOCKET opp_socket = play_session->Reciver(*i).fd;

                            Mark1 to_send1;
                            to_send1.type = DataType::LEAVE_ENEMY;
                            to_send1.length = 0; // вроде никаких данных передавать не следует
                            to_send1.data = nullptr;

                            Server_Send(to_send1, opp_socket);

                            Mark1 to_send2;
                            to_send2.type = DataType::SHUTDOWN;
                            to_send2.length = 0; // вроде никаких данных передавать не следует
                            to_send2.data = nullptr;

                            Server_Send(to_send2, *i);

                            // игрок, снова нуждающийся в противнике
                            Player Alone = play_session->Reciver(*i);
                            if (!line.empty())
                            {
                                Player pl2 = line.front();
                                line.erase(line.begin());
                                Make_Session(pl2, Alone);
                            }
                            else
                            {
                                line.push_back(Alone);
                            }

                            // Наконец, изменение рейтига для покинувшего
                            Player loser = play_session->Sender(*i);
                            if (!play_session->IsFinished)
                            {
                                rating[loser.id] += (-10 + rand() % 2);
                            }

                            pqxx::connection *database_session = make_session();
                            pqxx::work tx(*database_session);
                            if (rating[loser.id] >= 0) // нельзя отнимать от отрицательного индекса
                            {
                                tx.exec("update users set rating = rating + $1 where id = $2", pqxx::params{rating[loser.id], loser.id});
                            }
                            else
                            {
                                tx.exec("update users set rating = rating + $1 where id = $2 and rating > 100", pqxx::params{rating[loser.id], loser.id});
                            }
                            tx.commit();
                            delete_session(database_session);
                            play_sessions.erase(session_id);
                            delete play_session;
                        }
                        else if (recv_data.type == DataType::NEXT_ENEMY)
                        {
                            uint32_t net_session_id;
                            memcpy(&net_session_id, recv_data.data, 4);
                            uint32_t session_id = ntohl(net_session_id);
                            Session *play_session = play_sessions[session_id];

                            SOCKET one = play_session->pl1.fd;
                            SOCKET two = play_session->pl2.fd;

                            SOCKET opp_socket = (one == *i) ? two : one;

                            Mark1 to_send1;
                            to_send1.type = DataType::LEAVE_ENEMY;
                            to_send1.length = 0; // вроде никаких данных передавать не следует
                            to_send1.data = nullptr;

                            // Оба переходят в статус ожидания
                            Server_Send(to_send1, opp_socket);
                            Server_Send(to_send1, *i);

                            Player First = (play_session->pl1.fd == opp_socket) ? play_session->pl1 : play_session->pl2;
                            Player Second = (play_session->pl1.fd == opp_socket) ? play_session->pl2 : play_session->pl1;
                            if (!line.empty()) // сначала брошенному
                            {
                                Player pl2 = line.front();
                                line.erase(line.begin());
                                Make_Session(pl2, First);
                                if (!line.empty()) // теперь бросившему
                                {
                                    pl2 = line.front();
                                    line.erase(line.begin());
                                    Make_Session(pl2, Second);
                                }
                                else
                                {
                                    line.push_back(Second);
                                }
                            }
                            else
                            {
                                line.push_back(First);
                                line.push_back(Second);
                            }

                            if (!play_session->IsFinished) // Только если отпрашиш решил сдаться
                            {
                                Player loser = play_session->Sender(*i);
                                rating[loser.id] += (-30 - rand() % 2); // наказание решившего покинуть игру
                            }

                            play_sessions.erase(session_id);
                            delete play_session;
                        }
                        else if (recv_data.type == DataType::STOP_FIND_ENEMY)
                        {
                            uint32_t net_id;
                            memcpy(&net_id, recv_data.data, recv_data.length);
                            uint32_t id = ntohl(net_id);
                            for (auto j = line.begin(); j != line.end(); ++j) // оптимизировать!!!
                            {
                                if (j->id == id)
                                {
                                    line.erase(j);
                                    break;
                                }
                            }
                            Mark1 to_send;
                            to_send.type = DataType::SHUTDOWN;
                            to_send.length = 0;
                            to_send.data = nullptr;
                            Server_Send(to_send, *i);

                            pqxx::connection *database_session = make_session();
                            pqxx::work tx(*database_session);
                            if (rating[id] >= 0) // нельзя отнимать от отрицательного индекса
                            {
                                tx.exec("update users set rating = rating + $1 where id = $2", pqxx::params{rating[id], id});
                            }
                            else
                            {
                                tx.exec("update users set rating = rating + $1 where id = $2 and rating > 100", pqxx::params{rating[id], id});
                            }
                            tx.commit();
                            delete_session(database_session);
                        }
                        else if (recv_data.type == DataType::DOWLOAD_PHOTO)
                        {

                            uint32_t net_id;
                            memcpy(&net_id, recv_data.data, 4);
                            uint32_t user_id = ntohl(net_id);
                            std::cout << "User id who wanst to update photo " << user_id << '\n';
                            std::string SavePath = "Static/" + std::to_string(user_id) + ".jpg";
                            std::cout << "Save to " << SavePath << '\n';

                            std::ofstream outFile(SavePath, std::ios::binary);
                            char *outStr = new char[recv_data.length * 3 / 4 + 1];
                            int outLength = base64Decode(recv_data.data + 4, recv_data.length - 4, outStr);
                            outFile.write(outStr, outLength);
                            delete[] outStr;

                            // обновляем информацию о пользователе и его фото профиля
                            pqxx::connection *session = make_session();
                            pqxx::work tx(*session);
                            tx.exec("update users set picture = $1 where id = $2", pqxx::params{SavePath, user_id});
                            tx.commit();
                            delete_session(session);

                            // отключаем клиента сразу же после успешного сохранения фото
                            Mark1 to_send;
                            to_send.type = DataType::SHUTDOWN;
                            to_send.length = 0;
                            to_send.data = nullptr;
                            Server_Send(to_send, *i);
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
            delete_clients.clear();
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
    int result = send(fd, mark1_serialize, data.fullsize(), 0);
    delete[] mark1_serialize;
    return result;
}

void Durak_Server::Make_Session(Player &pl1, Player &pl2)
{
    Session *new_session = new Session(pl1, pl2);
    std::cout << "New session id: " << new_session->id << '\n';
    play_sessions[new_session->id] = new_session;
    pqxx::connection *database_session = make_session();
    pqxx::work tx(*database_session);

    bool Player1White = rand() % 2 == 0;
    Mark1 ToPlayer1 = MakeStartPacket(tx, pl2, new_session->id, Player1White);

    bool Player2White = !Player1White;
    Mark1 ToPlayer2 = MakeStartPacket(tx, pl1, new_session->id, Player2White);

    Server_Send(ToPlayer1, pl1.fd);
    Server_Send(ToPlayer2, pl2.fd);
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

Mark1 Durak_Server::MakeStartPacket(pqxx::work &tx, const Player &pl, uint32_t session_id, bool flag)
{
    Mark1 to_send;
    pqxx::result username = tx.exec("select username from users where id=$1", pqxx::params{pl.id});
    std::string username_value = username[0][0].as<std::string>();

    uint32_t net_session_id = htonl(session_id); // 4 байта

    char *data = new char[username_value.size() + 5];

    memcpy(data, &net_session_id, 4);
    data[4] = (flag) ? 1 : 0;
    memcpy(data + 5, &username_value[0], username_value.size());

    to_send.type = DataType::START;
    to_send.length = username_value.size() + 5;
    to_send.data = data;

    return to_send;
}