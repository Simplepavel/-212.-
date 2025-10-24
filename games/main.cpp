#include <windows.h>

#include <clocale>
#include <iostream>

#include "durak.h"

int main() {
  SetConsoleOutputCP(65001);
  SetConsoleCP(65001);

  Game game;
  game.startGame();

  while (!game.isGameOver()) {
    game.playTurn();
  }

  std::cout << "Игра окончена!" << std::endl;
  return 0;
}