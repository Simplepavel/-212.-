#include "durak.h"

#include <iostream>
#include <limits>
#include <vector>

Game::Game() : state(GameState::GAME_OVER), player1turn(true) {}

void Game::startGame() {
  deck.reset();
  player1.clear();
  player2.clear();
  attackCards.clear();
  defenseCards.clear();

  trumpCard = deck.drawCard();

  dealCards();
  state = GameState::ATTACK;
  player1turn = true;

  std::cout << "=== Начало игры ===" << std::endl;
  std::cout << "КОЗЫРЬ: " << trumpCard.toString() << std::endl;
}

void Game::dealCards() {
  for (int i = 0; i < 6; ++i) {
    if (!deck.isEmpty()) {
      player1.addCard(deck.drawCard());
    }
    if (!deck.isEmpty()) {
      player2.addCard(deck.drawCard());
    }
  }
}

void Game::displayPlayerHand(const Player& player, const std::string& name) {
  std::cout << name << " в руке: ";
  std::vector<Card> cards = player.getCards();
  for (size_t i = 0; i < cards.size(); ++i) {
    std::cout << (i + 1) << ":" << cards[i].toString() << " ";
  }
  std::cout << std::endl;
}

void Game::displayCards(const std::vector<Card>& cards,
                        const std::string& title) {
  std::cout << title << ": ";
  for (size_t i = 0; i < cards.size(); ++i) {
    std::cout << (i + 1) << ":" << cards[i].toString() << " ";
  }
  std::cout << std::endl;
}

void Game::displayGameState() {
  std::cout << "\n=== ИГРОВОЙ СТОЛ ===" << std::endl;

  displayPlayerHand(player1, "ИГРОК 1");
  displayPlayerHand(player2, "ИГРОК 2");

  std::cout << std::endl;

  if (state == GameState::ATTACK) {
    std::cout << "<ХОД " << (player1turn ? "ИГРОКА 1" : "ИГРОКА 2") << ">"
              << std::endl;
  } else {
    std::cout << "<ХОД " << (!player1turn ? "ИГРОКА 1" : "ИГРОКА 2") << ">"
              << std::endl;
  }

  std::cout << std::endl;

  if (!attackCards.empty() || !defenseCards.empty()) {
    displayCards(attackCards, "АТАКУЮЩИЕ КАРТЫ");
    displayCards(defenseCards, "КАРТЫ ОТБИТЬСЯ");
  }

  std::cout << "КАРД В КОЛОДЕ: " << deck.cardsLeft() << std::endl;
  std::cout << "КОЗЫРЬ: " << trumpCard.toString() << std::endl;
  std::cout << "========================\n" << std::endl;
}

void Game::playTurn() {
  if (state == GameState::GAME_OVER) {
    std::cout << "КОНЕЦ ИГРЫ!" << std::endl;
    return;
  }

  displayGameState();

  if (state == GameState::ATTACK) {
    if (player1turn) {
      // атака игрока 1
      std::cout << "Игрок 1 атакует!" << std::endl;
      std::vector<Card> cards = player1.getCards();
      if (cards.empty()) {
        std::cout << "У игрока 1 нет карт!" << std::endl;
        return;
      }

      int cardIndex;
      while (true) {
        // выбираем карту которую будем крыть
        std::cout << "Выберите карту для атаки (1-" << cards.size() << "): ";
        std::cin >> cardIndex;
        if (cardIndex < 1 || cardIndex > static_cast<int>(cards.size())) {
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          std::cout << "Неверный ввод! Попробуйте снова." << std::endl;
        } else {
          break;
        }
      }

      attack(cardIndex - 1);
    } else {
      // атака игрока 2
      std::cout << "Игрок 2 атакует!" << std::endl;
      std::vector<Card> cards = player2.getCards();
      if (cards.empty()) {
        std::cout << "У игрока 2 нет карт!" << std::endl;
        return;
      }

      int cardIndex;
      while (true) {
        // выбираем карту которую будем крыть
        std::cout << "Выберите карту для атаки (1-" << cards.size() << "): ";
        std::cin >> cardIndex;
        if (cardIndex < 1 || cardIndex > static_cast<int>(cards.size())) {
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          std::cout << "Неверный ввод! Попробуйте снова." << std::endl;
        } else {
          break;
        }
      }

      attack(cardIndex - 1);
    }
  } else if (state == GameState::DEFEND) {
    if (!player1turn) {
      // защита игрока 1
      std::cout << "Игрок 1 защищается!" << std::endl;
      std::vector<Card> defenderCards = player1.getCards();
      if (defenderCards.empty()) {
        std::cout << "У игрока 1 нет карт для защиты!" << std::endl;
        takeCards();
        return;
      }

      if (attackCards.empty()) {
        std::cout << "Нет карт для отбития!" << std::endl;
        return;
      }

      int attackIndex, defenseIndex;
      while (true) {
        std::cout << "Выберите атакующую карту (1-" << attackCards.size()
                  << "): ";
        std::cin >> attackIndex;
        std::cout << "Выберите свою карту для защиты (1-"
                  << defenderCards.size() << "): ";
        std::cin >> defenseIndex;

        if (attackIndex < 1 ||
            attackIndex > static_cast<int>(attackCards.size()) ||
            defenseIndex < 1 ||
            defenseIndex > static_cast<int>(defenderCards.size())) {
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          std::cout << "Неверный ввод! Попробуйте снова." << std::endl;
        } else {
          break;
        }
      }

      defend(attackIndex - 1, defenseIndex - 1);
    } else {
      // защита игрока 2
      std::cout << "Игрок 2 защищается!" << std::endl;
      std::vector<Card> defenderCards = player2.getCards();
      if (defenderCards.empty()) {
        std::cout << "У игрока 2 нет карт для защиты!" << std::endl;
        takeCards();
        return;
      }

      if (attackCards.empty()) {
        std::cout << "Нет карт для отбития!" << std::endl;
        return;
      }

      int attackIndex, defenseIndex;
      while (true) {
        std::cout << "Выберите атакующую карту (1-" << attackCards.size()
                  << "): ";
        std::cin >> attackIndex;
        if (attackIndex != 0) {
          std::cout << "Выберите свою карту для защиты (1-"
                    << defenderCards.size() << "): ";
          std::cin >> defenseIndex;
        } else {
          defenseIndex = 1;
        }

        if ((attackIndex < 1 && attackIndex != 0) ||
            attackIndex > static_cast<int>(attackCards.size()) ||
            defenseIndex < 1 ||
            defenseIndex > static_cast<int>(defenderCards.size())) {
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          std::cout << "Неверный ввод! Попробуйте снова." << std::endl;
        } else {
          break;
        }
      }

      defend(attackIndex - 1, defenseIndex - 1);
    }
  }
}

void Game::attack(int cardIndex) {
  std::vector<Card> cards =
      (player1turn ? player1.getCards() : player2.getCards());

  if (cardIndex < 0 || cardIndex >= static_cast<int>(cards.size())) {
    std::cout << "Неверный индекс карты!" << std::endl;
    return;
  }

  Card selectedCard = cards[cardIndex];

  if ((player1turn && player1.hasCard(selectedCard)) ||
      (!player1turn && player2.hasCard(selectedCard))) {
    attackCards.push_back(selectedCard);

    if (player1turn) {
      player1.removeCard(selectedCard);
    } else {
      player2.removeCard(selectedCard);
    }

    state = GameState::DEFEND;
    std::cout << "Атаковано картой: " << selectedCard.toString() << std::endl;
  } else {
    std::cout << "У игрока нет этой карты!" << std::endl;
  }
}

void Game::defend(int attackCardIndex, int defenseCardIndex) {
  if (attackCardIndex == -1) {
    takeCards();
    return;
  } else if (attackCardIndex < 0 ||
             attackCardIndex >= static_cast<int>(attackCards.size())) {
    std::cout << "Неверный индекс атакующей карты!" << std::endl;
    return;
  }

  std::vector<Card> defenderCards =
      (!player1turn ? player1.getCards() : player2.getCards());
  if (defenseCardIndex < 0 ||
      defenseCardIndex >= static_cast<int>(defenderCards.size())) {
    std::cout << "Неверный индекс защитной карты!" << std::endl;
    return;
  }

  Card attackCard = attackCards[attackCardIndex];
  Card defenseCard = defenderCards[defenseCardIndex];

  bool canDefend = (defenseCard.getSuit() == attackCard.getSuit() &&
                    defenseCard.getRank() > attackCard.getRank()) ||
                   (defenseCard.getSuit() == trumpCard.getSuit() &&
                    attackCard.getSuit() != trumpCard.getSuit());

  if (canDefend) {
    defenseCards.push_back(defenseCard);

    if (!player1turn) {
      player1.removeCard(defenseCard);
    } else {
      player2.removeCard(defenseCard);
    }

    std::cout << "Отбито картой: " << defenseCard.toString() << std::endl;

    if (attackCards.size() == defenseCards.size()) {
      std::cout << "Все карты отбиты! Ход завершен." << std::endl;
      endTurn();
    }
  } else {
    std::cout << "Невозможно отбить этой картой!" << std::endl;
  }
}

void Game::takeCards() {
  std::cout << "Игрок забирает карты!" << std::endl;

  Player& player = (!player1turn ? player1 : player2);
  for (const Card& card : attackCards) {
    player.addCard(card);
  }
  for (const Card& card : defenseCards) {
    player.addCard(card);
  }

  attackCards.clear();
  defenseCards.clear();
  state = GameState::ATTACK;
  player1turn = !player1turn;
  endTurn();
}

void Game::endTurn() {
  attackCards.clear();
  defenseCards.clear();
  state = GameState::ATTACK;
  player1turn = !player1turn;

  while (player1.cardCount() < 6 && !deck.isEmpty()) {
    player1.addCard(deck.drawCard());
  }
  while (player2.cardCount() < 6 && !deck.isEmpty()) {
    player2.addCard(deck.drawCard());
  }

  checkGameOver();
}

void Game::checkGameOver() {
  if (player1.cardCount() == 0 && player2.cardCount() == 0) {
    state = GameState::GAME_OVER;
    std::cout << "=== НИЧЬЯ! ===" << std::endl;
  } else if (player1.cardCount() == 0) {
    state = GameState::GAME_OVER;
    std::cout << "=== ИГРОК 1 ВЫИГРАЛ! ===" << std::endl;
  } else if (player2.cardCount() == 0) {
    state = GameState::GAME_OVER;
    std::cout << "=== ИГРОК 2 ВЫИГРАЛ! ===" << std::endl;
  }
}