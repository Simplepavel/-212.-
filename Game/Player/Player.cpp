#include "Player.hpp"

PlayerClient::PlayerClient() : Durak_Client::Durak_Client(), opp_id(-1)
{
    Client_Connect("127.0.0.1", "6666");
}

void PlayerClient::connect(int _opp_id)
{
    opp_id = _opp_id;
}
