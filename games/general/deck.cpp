#include "deck.h"

#include <algorithm>
#include <random>

Deck::Deck() { reset(); }

void Deck::reset() {
  cards.clear();
  for (int s = HEARTS; s <= SPADES; ++s) {
    for (int r = SIX; r <= ACE; ++r) {
      cards.push_back(Card(static_cast<Suit>(s), static_cast<Rank>(r)));
    }
  }
  shuffle();
}

void Deck::shuffle() {
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(cards.begin(), cards.end(), g);
}

Card Deck::drawCard() {
  if (!cards.empty()) {
    Card tmp = cards.front();
    cards.erase(cards.begin());
    return tmp;
  }
  return Card();
}

bool Deck::isEmpty() const { return cards.empty(); }

int Deck::cardsLeft() const { return cards.size(); }
