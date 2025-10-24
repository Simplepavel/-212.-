#ifndef DURAK_H
#define DURAK_H

#include <iostream>
#include <vector>

#include "general/deck.h"
#include "general/player.h"

enum GameState { ATTACK, DEFEND, GAME_OVER };

class Game {
 private:
  void dealCards();
  void checkGameOver();
  void displayGameState();
  void displayCards(const std::vector<Card>& cards, const std::string& title);
  void displayPlayerHand(const Player& player, const std::string& name);

  Deck deck;
  Player player1;
  Player player2;
  GameState state;
  Card trumpCard;
  std::vector<Card> attackCards;
  std::vector<Card> defenseCards;
  bool player1turn;

 public:
  Game();

  void startGame();
  void playTurn();
  void attack(int cardIndex);
  void defend(int attackCardIndex, int defenseCardIndex);
  void takeCards();
  void endTurn();

  bool isGameOver() const { return state == GameState::GAME_OVER; }
};

#endif