#include "Client.hpp"
int main()
{
    Durak_Client client;

    Protocol pr1;
    pr1.id = 125;
    pr1.opp_id = 425;
    pr1.move = MOVES::ATTACK;
    pr1.card = Card(Suits::CLUBS, Ranks::KING);
    client.Client_Connect("127.0.0.1", "7538");
    client.Client_Send(pr1);
    char t;
    std::cin >> t;
    client.Client_Disconnect();
    return 0;
}