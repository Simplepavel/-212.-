#include "Protocol.hpp"

Mark1::Mark1() : type(DataType::NONE), length(0), data(nullptr)
{
}

Mark1::Mark1(const Mark1 &argv)
{
    type = argv.type;
    length = argv.length;
    data = new char[length];
    memcpy(data, argv.data, length);
}

Mark1 &Mark1::operator=(const Mark1 &argv)
{
    if (this != &argv)
    {
        type = argv.type;
        length = argv.length;
        data = new char[length];
        memcpy(data, argv.data, length);
    }
    return *this;
}

Mark1::Mark1(Mark1 &&argv)
{
    type = argv.type;
    length = argv.length;
    data = argv.data;
    argv.length = 0;
    argv.data = nullptr;
}

Mark1 &Mark1::operator=(Mark1 &&argv)
{
    if (this != &argv)
    {
        type = argv.type;
        length = argv.length;
        data = argv.data;
        argv.length = 0;
        argv.data = nullptr;
    }
    return *this;
}

Mark1::~Mark1()
{
    delete[] data;
}

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
    memcpy(&net_length, buffer + 1, 4);
    result.length = ntohl(net_length);

    result.data = new char[result.length];
    memcpy(result.data, buffer + 5, result.length);
    return result;
}