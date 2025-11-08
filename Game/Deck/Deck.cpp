#include "Deck.hpp"

Deck::Deck() : count(0) {}

void Deck::add_card(const Card &new_vl_)
{
    deck.push_back(new_vl_);
    ++count;
}

const Card &Deck::get_card(unsigned int idx) const
{
    return deck[idx];
}

void Deck::remove_card(unsigned int idx)
{
    if (idx >= 0 && idx < count)
    {
        deck.erase(deck.begin() + idx);
        --count;
    }
}

void Deck::shuffle()
{
    srand(time(nullptr));
    for (int i = 0; i < 100; ++i)
    {
        int idx1 = rand() % count;
        int idx2 = rand() % count;
        std::swap(deck[idx1], deck[idx2]);
    }
}

char *Deck::serialize()
{
    char *buffer = new char[(count + 1) * 4];
    uint32_t count_cards = htonl(static_cast<uint32_t>(count));
    memcpy(buffer, &count_cards, 4);
    int idx = 1;
    for (auto i = deck.begin(); i != deck.end(); ++i)
    {
        char *a = i->serialize();
        memcpy(buffer + idx * 4, a, 4);
        delete[] a;
        ++idx;
    }
    return buffer;
}

Deck *Deck::deserialize(char *buffer)
{
    uint32_t net_count_card;
    memcpy(&net_count_card, buffer, 4);
    uint32_t count_card = ntohl(net_count_card);

    Deck temp;
    Deck *result = temp.create_empty();

    for (int i = 1; i <= count_card; ++i)
    {
        result->add_card(Card::deserialize(buffer + 4 * i));
    }
    return result;
}

Deck *Deck::create_empty()
{
    return new Deck();
}

uint8_t Deck::GetTotalByteSize() // максимум 37 * 4 < 255
{
    return (count + 1) * 4;
}

std::ostream &operator<<(std::ostream &cout, const Deck &deck)
{
    for (int i = 0; i < deck.get_count(); ++i)
    {
        cout << deck.get_card(i) << ' ';
    }
    return cout;
}
