#include <string>
#include <vector>
#include "../Deck.hpp"
#include "../../Client/Client.hpp"

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
class PlayerClient : public Durak_Client
{
private:
    std::string username;
    std::vector<Card> cards;
    int opp_id; // id оппонента на севере
    int own_id; // собственный id на сервере

public:
    PlayerClient();
    void connect(int _opp_id, int _own_id);
    void start_game(Deck &deck);
};