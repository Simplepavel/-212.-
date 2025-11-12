#include "Session.hpp"

Session::Session() : shop(), pl1(), pl2(), stol(), pl1_attack(false) {}

Session::Session(const Session &argv)
{
    shop = argv.shop;
    pl1 = argv.pl1;
    pl2 = argv.pl2;
    stol = argv.stol;
    pl1_attack = argv.pl1_attack;
}

Session::Session(const Session &&argv)
{
    shop = std::move(argv.shop);
    pl1 = std::move(argv.pl1);
    pl2 = std::move(argv.pl2);
    stol = argv.stol;
    pl1_attack = argv.pl1_attack;
}

Session &Session::operator=(const Session &argv)
{
    if (this != &argv)
    {
        shop = argv.shop;
        pl1 = argv.pl1;
        pl2 = argv.pl2;
        stol = argv.stol;
        pl1_attack = argv.pl1_attack;
    }
    return *this;
}

Session &Session::operator=(const Session &&argv)
{
    if (this != &argv)
    {
        shop = std::move(argv.shop);
        pl1 = std::move(argv.pl1);
        pl2 = std::move(argv.pl2);
        stol = argv.stol;
        pl1_attack = argv.pl1_attack;
    }
    return *this;
}

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
}

Session::Session(const Shop &sh, const Player &p1, const Player &p2, const Card &card, bool ip1a) : shop(sh), pl1(p1), pl2(p2), stol(card), pl1_attack(ip1a) {}

void Session::show_state()
{
    std::cout << "==============Game==================\n";
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
    idx += pl1_size;
    // десерелизация pl1

    uint8_t pl2_size;
    memcpy(&pl2_size, buffer + idx, 1);
    idx += 1;
    Player new_pl2 = Player::deserialize(buffer + idx);
    idx += pl2_size;
    // десерелизация pl2

    uint8_t stol_size;
    memcpy(&stol_size, buffer + idx, 1);
    idx += 1;
    Card new_stol = Card::deserialize(buffer + idx);
    idx += stol_size;
    // десерелизация stol

    bool new_pl1_attack = buffer[idx] == 1;
    // десереализация pl1_attacker

    return Session(*new_shop, new_pl1, new_pl2, new_stol, new_pl1_attack);
}

void Session::self_deserialize(char *buffer)
{
    *this = deserialize(buffer);
}