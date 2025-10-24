#include "card.h"

Card::Card() : suit(HEARTS), rank(SIX) {}

Card::Card(Suit suit, Rank rank) : suit(suit), rank(rank) {}

Suit Card::getSuit() const { return suit; }

Rank Card::getRank() const { return rank; }

std::string Card::toString() const {
  std::string suitStr;
  switch (suit) {
    case HEARTS:
      suitStr = "♥";
      break;
    case DIAMONDS:
      suitStr = "♦";
      break;
    case CLUBS:
      suitStr = "♣";
      break;
    case SPADES:
      suitStr = "♠";
      break;
  }

  std::string rankStr;
  switch (rank) {
    case JACK:
      rankStr = "J";
      break;
    case QUEEN:
      rankStr = "Q";
      break;
    case KING:
      rankStr = "K";
      break;
    case ACE:
      rankStr = "A";
      break;
    default:
      rankStr = std::to_string(rank);
      break;
  }

  return rankStr + suitStr;
}

bool Card::operator<(const Card& other) const {
  if (rank != other.rank) return rank < other.rank;
  return suit < other.suit;
}

bool Card::operator==(const Card& other) const {
  return suit == other.suit && rank == other.rank;
}
