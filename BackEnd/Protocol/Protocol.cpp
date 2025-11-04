#include "Protocol.hpp"

void Protocol::serialize(char *buffer)
{
    uint32_t net_id = htonl(id);
    uint32_t net_opp_id = htonl(opp_id);
    memcpy(buffer, &net_id, 4);
    memcpy(buffer + 4, &net_opp_id, 4);
    memcpy(buffer + 8, &move, 1);

    card.serialize(buffer + 9);
}

Protocol Protocol::deserialize(char *buffer)
{
    Protocol result;
    uint32_t net_id;
    uint32_t net_opp_id;

    memcpy(&net_id, buffer, 4);
    memcpy(&net_opp_id, buffer + 4, 4);
    memcpy(&result.move, buffer + 8, 1);
    result.id = ntohl(net_id);
    result.opp_id = ntohl(net_opp_id);

    result.card = Card::deserialize(buffer + 9);
    return result;
}