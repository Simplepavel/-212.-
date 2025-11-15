#include "Protocol.hpp"

uint32_t Mark1::capacity() const { return length + 5; }

char *Mark1::serialize() const
{
    char *result = new char[capacity()];

    uint32_t net_length = htonl(length);

    memcpy(result, &type, 1);           // запоминаем тип данных
    memcpy(result + 1, &net_length, 4); // запоминаем длину данных
    memcpy(result + 5, data, length);   // запонимаем сами данные, которые в последстивии буду передаваться в методы self_deserialize
    return result;
}

Mark1 Mark1::deserialize(char *buffer)
{
    Mark1 result;
    memcpy(&result.type, buffer, 1);

    uint32_t net_length;
    memcpy(&net_length, buffer + 1,  4);
    result.length = ntohl(net_length);

    char *data = new char[result.length];
    memcpy(data, buffer + 5, result.length);
    result.data = data;
    return result;
}