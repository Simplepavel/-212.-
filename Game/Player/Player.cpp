#include "Player.hpp"

PlayerClient::PlayerClient() : Durak_Client::Durak_Client(), opp_id(-1)
{
    Client_Connect("127.0.0.1", "6666"); // подключаеммся к игровому серверу. Просто подключение, мы еще не нашли пару
    // здесь бы получить собсвтенный id с сервера
}

void PlayerClient::connect(int _opp_id, int _own_id)
{
    opp_id = _opp_id;
    own_id = _own_id;
}

void PlayerClient::start_game(Deck &deck)
{
    if (opp_id == -1)
    {
        return;
    }
    for (int i = 0; i < 6; ++i)
    {
        cards.push_back(deck.get_card());
    }
}
