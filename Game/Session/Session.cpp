#include "Session.hpp"

Session::Session(const std::string &n1, const std::string &n2) : pl1(shop, n1), pl2(shop, n2)
{
    Card lowest1 = pl1.GetLowestTrump();
    Card lowest2 = pl2.GetLowestTrump();
    if (lowest1.is_valid() && lowest2.is_valid())
    {
        pl1_attack = lowest1 < lowest2;
    }
    if (lowest1.is_valid() && !lowest2.is_valid())
    {
        pl1_attack = true;
    }
    if (!lowest1.is_valid() && lowest2.is_valid())
    {
        pl1_attack = false;
    }
    if (!lowest1.is_valid() && !lowest2.is_valid())
    {
        pl1_attack = (rand() % 2 == 0);
    }

    std::cout << "==============Start Game==================\n";
    std::cout << pl1 << '\n';
    std::cout << pl2 << '\n';
    std::cout << "Shop: " << shop << '\n';
    std::cout << ((pl1_attack) ? ("Player1 is attaker") : ("Player2 is attacker")) << '\n';
}

char *Session::serialize()
{

    uint32_t total_size = shop.GetTotalByteSize() + pl1.GetTotalByteSize() + pl2.GetTotalByteSize() + stol.GetTotalByteSize() + 1;
    char *result = new char[total_size + 4]; // плюс 4 байта для длины каждого

    int idx = 0;

    // shop сериализаци
    // ToDo - вынести эти строки код в отдельную функцию
    uint8_t shop_size = shop.GetTotalByteSize();
    char *shop_serialize = shop.serialize();
    memcpy(result + idx, &shop_size, 1);
    idx += 1;
    memcpy(result + idx, shop_serialize, shop_size);
    delete[] shop_serialize;
    idx += shop.GetTotalByteSize();
    // сериализация shop закончена: длина данных + данные

    // pl1 сериализация
    uint8_t pl1_size = pl1.GetTotalByteSize();
    char *pl1_serialize = pl1.serialize();
    memcpy(result + idx, &pl1_size, 1);
    idx += 1;
    memcpy(result + idx, pl1_serialize, pl1_size);
    delete[] pl1_serialize;
    idx += pl1.GetTotalByteSize();

    // сериализация pl1 закончена: длина данных + данные

    // pl2 сериализация
    uint8_t pl2_size = pl2.GetTotalByteSize();
    char *pl2_serialize = pl2.serialize();
    memcpy(result + idx, &pl2_size, 1);
    idx += 1;
    memcpy(result + idx, pl2_serialize, pl2.GetTotalByteSize());
    delete[] pl2_serialize;
    idx += pl2.GetTotalByteSize();
    // сериализация pl2 закончена: длина данных + данные

    // stol сериализация
    uint8_t stol_size = stol.GetTotalByteSize();
    char *stol_serialize = stol.serialize();
    memcpy(result + idx, &stol_size, 1);
    idx += 1;
    memcpy(result + idx, stol_serialize, stol.GetTotalByteSize());
    delete[] stol_serialize;
    idx += stol.GetTotalByteSize();
    // сериализация stol закончена: длина данных + данные

    result[idx] = pl1_attack ? 1 : 0;

    return result;
}

Session Session::deserialize(char *buffer)
{
    int idx = 0;

    uint8_t shop_size;
    memcpy(&shop_size, buffer + idx, 1);
    idx += 1;
    Deck *_shop = Shop::deserialize(buffer + idx);
    Shop *new_shop = static_cast<Shop *>(_shop);
    idx += shop_size;
    // десерелизация shop

    uint8_t pl1_size;
    memcpy(&pl1_size, buffer + idx, 1);
    idx += 1;
    Player new_pl1 = Player::deserialize(buffer + idx);
    std::cout << new_pl1 << '\n';
    idx += pl1_size;
    // десерелизация pl1

    uint8_t pl2_size;
    memcpy(&pl2_size, buffer + idx, 1);
    idx += 1;
    Player new_pl2 = Player::deserialize(buffer + idx);
    std::cout << new_pl2 << '\n';
    idx += pl2_size;
    // десерелизация pl2

    uint8_t stol_size;
    memcpy(&stol_size, buffer + idx, 1);
    idx += 1;
    Card new_stol = Card::deserialize(buffer + idx);
    idx += stol_size;
    // десерелизация stol

    bool new_pl1_attack = buffer[idx] == 1;
    std::cout << "Shop: " << *new_shop << '\n';
    std::cout << ((new_pl1_attack) ? ("Player1 is attaker") : ("Player2 is attacker")) << '\n';

    std::cout << "end\n";
}