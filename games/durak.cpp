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
  turnCards.clear();

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
    std::cout << "<АТАКА " << (player1turn ? "ИГРОКА 1" : "ИГРОКА 2") << ">"
              << std::endl;
  } else if (state == GameState::DEFEND) {
    std::cout << "<ЗАЩИТА " << (!player1turn ? "ИГРОКА 1" : "ИГРОКА 2") << ">"
              << std::endl;
  } else if (state == GameState::ADD_ATTACK) {
     std::cout << "<ПОДКИДЫВАНИЕ " << (player1turn ? "ИГРОКА 1" : "ИГРОКА 2") << ">"
              << std::endl;
  }

  std::cout << std::endl;

  if (!attackCards.empty() || !defenseCards.empty()) {
    displayCards(attackCards, "АТАКУЮЩИЕ КАРТЫ");
  }

  std::cout << "КАРД В КОЛОДЕ: " << deck.cardsLeft() << std::endl;
  std::cout << "КОЗЫРЬ: " << trumpCard.toString() << std::endl;
  std::cout << "========================\n" << std::endl;
}

bool Game::canToss(const Card& card) {
    for(Card& cardOnTurn : turnCards) {
        if(cardOnTurn.getRank() == card.getRank()) return true;
    }
    return false;
}

void Game::playTurn() {
  if (state == GameState::GAME_OVER) {
    std::cout << "КОНЕЦ ИГРЫ!" << std::endl;
    return;
  }

  displayGameState();

  if (state == GameState::ATTACK) {
    Player& attacker = (player1turn ? player1 : player2);
    std::cout << "Игрок " << (player1turn ? "1" : "2") << " атакует!" << std::endl;

    while(true) {
        std::vector<Card> cards = attacker.getCards();
        if (cards.empty()) {
            std::cout << "У игрока " << (player1turn ? "1" : "2") << " нет карт!" << std::endl;
            if(attackCards.empty()) endTurn();
            break;
        }

        displayPlayerHand(attacker, "ИГРОК " + std::string(player1turn ? "1" : "2"));

        std::cout << "Выберите карту для атаки (1-" << cards.size() << ") или 0, чтобы закончить: ";
        int cardIndex;
        std::cin >> cardIndex;

        if (cardIndex == 0) {
            if (attackCards.empty()) {
                std::cout << "Вы должны сыграть хотя бы одну карту." << std::endl;
                continue;
            }
            std::cout << "Атака завершена." << std::endl;
            state = GameState::DEFEND;
            break; 
        }

        if (cardIndex < 1 || cardIndex > static_cast<int>(cards.size())) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Неверный ввод! Попробуйте снова." << std::endl;
        } else {
            Card selectedCard = cards[cardIndex - 1];
            if (attackCards.empty() || canToss(selectedCard)) {
                attack(cardIndex - 1); 
            } else {
                 std::cout << "Нельзя атаковать этой картой!" << std::endl;
            }
        }
    }
  } else if (state == GameState::DEFEND) {
    Player& defender = (!player1turn ? player1 : player2);
    std::cout << "Игрок " << (!player1turn ? "1" : "2") << " защищается!" << std::endl;

    std::vector<Card> defenderCards = defender.getCards();
    if (defenderCards.empty()) {
        std::cout << "У игрока " << (!player1turn ? "1" : "2") << " нет карт для защиты!" << std::endl;
        takeCards();
        return;
    }

    displayPlayerHand(defender, "ИГРОК " + std::string(!player1turn ? "1" : "2"));

    int attackIndex, defenseIndex;
    while (true) {
        std::cout << "Выберите атакующую карту (1-" << attackCards.size()
                  << ") или 0, чтобы взять карты ";
        std::cin >> attackIndex;

        if (attackIndex == 0) {
            takeCards();
            return;
        }
        if (attackIndex < 1 ||
            attackIndex > static_cast<int>(attackCards.size())) {
              std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
              std::cout << "Неверный ввод! Попробуйте снова." << std::endl;
            }

        std::cout << "Выберите свою карту для защиты (1-"
                    << defenderCards.size() << "): ";
        std::cin >> defenseIndex;

        if (defenseIndex < 1 ||
            defenseIndex > static_cast<int>(defenderCards.size())) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Неверный ввод! Попробуйте снова." << std::endl;
        } else {
            break;
        }
    }
    defend(attackIndex - 1, defenseIndex - 1);
  } else if (state == GameState::ADD_ATTACK) {
    Player& attacker = (player1turn ? player1 : player2);
    Player& defender = (!player1turn ? player1 : player2);

    std::cout << "Игрок " << (player1turn ? "1" : "2") << " может подкинуть!" << std::endl;

     while(true) {
        std::vector<Card> cards = attacker.getCards();
        if (cards.empty()) {
            std::cout << "У атакующего нет карт, чтобы подкинуть." << std::endl;
            endTurn();
            break;
        }

        int defenderCardCount = defender.cardCount();
        if (attackCards.size() >= 6 || attackCards.size() >= (6 - defenderCardCount + defenseCards.size())) {
             std::cout << "Достигнут лимит карт (6) или у защитника не хватит карт. Бито." << std::endl;
             endTurn();
             break;
        }

        displayPlayerHand(attacker, "ИГРОК " + std::string(player1turn ? "1" : "2"));

        std::cout << "Выберите карту для подкидывания (1-" << cards.size() << ") или 0, чтобы сказать бито: ";
        int cardIndex;
        std::cin >> cardIndex;

        if (cardIndex == 0) {
            std::cout << "Бито. Ход завершен." << std::endl;
            endTurn();
            break; 
        }

        if (cardIndex < 1 || cardIndex > static_cast<int>(cards.size())) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Неверный ввод! Попробуйте снова." << std::endl;
        } else {
            Card selectedCard = cards[cardIndex - 1];
            if (canToss(selectedCard)) {
                attack(cardIndex - 1); 
                state = GameState::DEFEND;
                break;
            } else {
                 std::cout << "Нельзя подкинуть эту карту!" << std::endl;
            }
        }
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
    turnCards.push_back(selectedCard);

    if (player1turn) {
      player1.removeCard(selectedCard);
    } else {
      player2.removeCard(selectedCard);
    }

    state = GameState::DEFEND;
    displayCards(attackCards, "Карты для атаки");
  } else {
    std::cout << "У игрока нет этой карты!" << std::endl;
  }
}

void Game::defend(int attackCardIndex, int defenseCardIndex) {
  std::vector<Card> defenderCards = (!player1turn ? player1.getCards() : player2.getCards());

  Card attackCard = attackCards[attackCardIndex];
  Card defenseCard = defenderCards[defenseCardIndex];

  bool canDefend = (defenseCard.getSuit() == attackCard.getSuit() &&
                    defenseCard.getRank() > attackCard.getRank()) ||
                   (defenseCard.getSuit() == trumpCard.getSuit() &&
                    attackCard.getSuit() != trumpCard.getSuit());

  if (canDefend) {
    defenseCards.push_back(defenseCard);
    turnCards.push_back(defenseCard);
    attackCards.erase(attackCards.begin() + attackCardIndex);

    if (!player1turn) {
      player1.removeCard(defenseCard);
    } else {
      player2.removeCard(defenseCard);
    }

    std::cout << "Отбито: " << attackCard.toString() << " покрыта " << defenseCard.toString() << std::endl;

    if (attackCards.size() == 0) {
      std::cout << "Все карты на столе отбиты! Атакующий может подкинуть." << std::endl;
      state = GameState::ADD_ATTACK;
    }
  } else {
    std::cout << "Невозможно отбить этой картой (" << defenseCard.toString() << ") карту " << attackCard.toString() << "!" << std::endl;
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
  turnCards.clear();
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