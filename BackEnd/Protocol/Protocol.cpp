#include "Protocol.hpp"

Mark1::Mark1() : type(DataType::NONE), length(-1), data(nullptr)
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

int32_t Mark1::capacity() const { return length + 5; }

char *Mark1::serialize() const
{
    char *result = new char[capacity()];

    int32_t net_length = htonl(length);

    memcpy(result, &type, 1);           // запоминаем тип данных
    memcpy(result + 1, &net_length, 4); // запоминаем длину данных
    memcpy(result + 5, data, length);   // запонимаем сами данные, которые в последстивии буду передаваться в методы self_deserialize
    return result;
}

void Mark1::SetDataType(char *buffer)
{
    memcpy(&type, buffer, 1);
}

void Mark1::SetDataLength(char *buffer)
{
    int32_t net_length;
    memcpy(&net_length, buffer + 1, 4);
    length = ntohl(net_length);
}

void Mark1::SetData(char *buffer)
{
    delete[] data;
    if (length == -1)
    {
        SetDataLength(buffer);
    }
    data = new char[length];
    memcpy(data, buffer + 5, length);
}

Mark1 Mark1::deserialize(char *buffer)
{
    Mark1 result;
    result.SetDataType(buffer);
    result.SetDataLength(buffer);
    result.SetData(buffer);
    return result;
}