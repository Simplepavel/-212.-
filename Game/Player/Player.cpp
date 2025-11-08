#include "Player.hpp"

Player::Player(Shop &shop, const std::string &name) : username(name)
{
    for (int i = 0; i < 6; ++i)
    {
        deck.add_card(shop.get_top());
    }
}

Player::Player(const Deck &d, const std::string &n) : deck(d), username(n)
{
}
Card Player::GetLowestTrump()
{
    Card min_trump;
    for (int i = 0; i < deck.get_count(); ++i)
    {
        Card current_card = deck.get_card(i);
        if (current_card.is_trump && !min_trump.is_valid())
        {
            min_trump = current_card;
        }
        if (current_card.is_trump && min_trump.is_valid())
        {
            if (current_card < min_trump)
            {
                min_trump = current_card;
            }
        }
    }
    return min_trump;
}

char *Player::serialize()
{
    uint32_t s1 = deck.get_count();
    uint32_t s2 = username.length();

    uint32_t name_lenght = htonl(s2);

    char *result = new char[(s1 + 1) * 4 + s2 + 4];

    memcpy(result, &name_lenght, 4);
    memcpy(result + 4, username.c_str(), s2);
    char *deck_serialize = deck.serialize();
    memcpy(result + 4 + s2, deck_serialize, (s1 + 1) * 4);
    delete[] deck_serialize;
    return result;
}

Player Player::deserialize(char *buffer)
{
    std::string new_name;
    Deck *new_deck;

    uint32_t new_name_size_net;
    memcpy(&new_name_size_net, buffer, 4);
    uint32_t new_name_size = ntohl(new_name_size_net);

    new_name.resize(new_name_size);

    memcpy(&new_name[0], buffer + 4, new_name_size);

    new_deck = Deck::deserialize(buffer + 4 + new_name_size);

    return Player(*new_deck, new_name);
}

void Player::add_card(const Card &card)
{
    deck.add_card(card);
}

uint8_t Player::GetTotalByteSize() // масксиму (36+1) * 4 + 100 + 4 < 255
{
    uint8_t s1 = deck.get_count();
    uint8_t s2 = username.length();
    return (s1 + 1) * 4 + s2 + 4;
}

std::ostream &operator<<(std::ostream &cout, const Player &pl)
{
    std::cout << pl.username << ": " << pl.deck;
    return cout;
}
