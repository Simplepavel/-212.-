#include "player.h"

#include <algorithm>

Player::Player() {}

void Player::addCard(const Card& card) {
  cards.push_back(card);
  sortCards();
}

void Player::removeCard(const Card& card) {
  for (int i = 0; i < cards.size(); ++i) {
    if (cards[i] == card) {
      cards.erase(cards.begin() + i);
      break;
    }
  }
}

bool Player::hasCard(const Card& card) const {
  return std::find(cards.begin(), cards.end(), card) != cards.end();
}

std::vector<Card> Player::getCards() const { return cards; }

void Player::sortCards() { std::sort(cards.begin(), cards.end()); }

int Player::cardCount() const { return cards.size(); }

void Player::clear() { cards.clear(); }
