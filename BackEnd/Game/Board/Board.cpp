#include "Board.hpp"

Board::Board(FigureColor MyColor) {
  LastMoves.resize(4);
  self.resize(64);
  // Инициализируем все клетки пустыми фигурами
  for (int i = 0; i < 64; ++i) {
    self[i] = Figure();
  }

  // ЧЕРНЫЕ фигуры
  self[7 * 8 + 0] = Figure(FigureName::ROOK, FigureColor::BLACK);    // a8
  self[7 * 8 + 1] = Figure(FigureName::KNIGHT, FigureColor::BLACK);  // b8
  self[7 * 8 + 2] = Figure(FigureName::BISHOP, FigureColor::BLACK);  // c8
  self[7 * 8 + 3] = Figure(FigureName::QUEEN, FigureColor::BLACK);   // d8
  self[7 * 8 + 4] = Figure(FigureName::KING, FigureColor::BLACK);    // e8
  self[7 * 8 + 5] = Figure(FigureName::BISHOP, FigureColor::BLACK);  // f8
  self[7 * 8 + 6] = Figure(FigureName::KNIGHT, FigureColor::BLACK);  // g8
  self[7 * 8 + 7] =
      Figure(FigureName::ROOK,
             FigureColor::BLACK);  // h8 внизу черные, индексы большие

  // Черные пешки
  for (int i = 0; i < 8; ++i) {
    self[6 * 8 + i] = Figure(FigureName::PAWN, FigureColor::BLACK);  // линия 7
  }
  // БЕЛЫЕ фигуры
  self[0 * 8 + 0] = Figure(FigureName::ROOK, FigureColor::WHITE);    // a1
  self[0 * 8 + 1] = Figure(FigureName::KNIGHT, FigureColor::WHITE);  // b1
  self[0 * 8 + 2] = Figure(FigureName::BISHOP, FigureColor::WHITE);  // c1
  self[0 * 8 + 3] = Figure(FigureName::QUEEN, FigureColor::WHITE);   // d1
  self[0 * 8 + 4] = Figure(FigureName::KING, FigureColor::WHITE);    // e1
  self[0 * 8 + 5] = Figure(FigureName::BISHOP, FigureColor::WHITE);  // f1
  self[0 * 8 + 6] = Figure(FigureName::KNIGHT, FigureColor::WHITE);  // g1
  self[0 * 8 + 7] =
      Figure(FigureName::ROOK,
             FigureColor::WHITE);  // h1 белые сверху, индексы маленькие

  // Белые пешки
  for (int i = 0; i < 8; ++i) {
    self[1 * 8 + i] = Figure(FigureName::PAWN, FigureColor::WHITE);  // линия 2
  }
  if (MyColor == FigureColor::WHITE) {
    for (int i = 0; i < 16; ++i) {
      std::swap(self[i], self[63 - i]);
    }
  }
  myColor = MyColor;
}

char* Board::serialize() {
  char* result = new char[capacity()];
  for (int i = 0; i < 64; ++i) {
    char* figure_serialize = self[i].serialize();
    memcpy(result + i * 2, figure_serialize, 2);
    delete[] figure_serialize;
  }
  return result;
}

void Board::deserialize(char* buffer) {
  Figure figure;
  for (int i = 0; i < 64; ++i) {
    figure.deserialize(buffer + i * 2);
    self[i] = figure;
  }
}

void Board::show() const {
  for (int i = 0; i < 64; ++i) {
    Figure alfa = self[i];
    std::cout << alfa.to_string();
    if (i % 8 == 7) {
      std::cout << '\n';
    } else {
      std::cout << ' ';
    }
  }
}

void Board::replace(int c_row, int c_column, int l_row,
                    int l_column)  // аргументы от 0 до 7
{
  LastMoves[0] = c_row;
  LastMoves[1] = c_column;
  LastMoves[2] = l_row;
  LastMoves[3] = l_column;

  int idx1 = c_row * 8 + c_column;
  int idx2 = l_row * 8 + l_column;
  self[idx2] = self[idx1];
  self[idx1] = Figure();
}

void Board::replace() {
  int c_row = 7 - LastMoves[0];
  int c_column = 7 - LastMoves[1];
  int l_row = 7 - LastMoves[2];
  int l_column = 7 - LastMoves[3];

  int idx1 = c_row * 8 + c_column;
  int idx2 = l_row * 8 + l_column;
  self[idx2] = self[idx1];
  self[idx1] = Figure();
}

char* Board::SerializeMove() {
  char* result = new char[4];
  result[0] = LastMoves[0];
  result[1] = LastMoves[1];
  result[2] = LastMoves[2];
  result[3] = LastMoves[3];
  return result;
}

void Board::DeserializeMove(char* buffer) {
  LastMoves[0] = buffer[0];
  LastMoves[1] = buffer[1];
  LastMoves[2] = buffer[2];
  LastMoves[3] = buffer[3];
}

Figure& Board::operator[](int idx) { return self[idx]; }

bool Board::isFreeRow(int current_row, int current_column, int last_row,
                      int last_column) const {
  int step = (last_column > current_column) ? 1 : -1;
  for (int col = current_column + step; col != last_column; col += step) {
    if (self[current_row * 8 + col].is_valid()) return false;
  }
  return true;
}

bool Board::isFreeColumn(int current_row, int current_column, int last_row,
                         int last_column) const {
  int step = (last_row > current_row) ? 1 : -1;
  for (int row = current_row + step; row != last_row; row += step) {
    if (self[row * 8 + current_column].is_valid()) return false;
  }
  return true;
}

bool Board::isFreeDiagonal(int current_row, int current_column, int last_row,
                           int last_column) const {
  int rowStep = (last_row > current_row) ? 1 : -1;
  int colStep = (last_column > current_column) ? 1 : -1;
  for (int i = 1; i < abs(last_row - current_row); i++) {
    int row = current_row + i * rowStep;
    int col = current_column + i * colStep;
    if (self[row * 8 + col].is_valid()) return false;
  }
  return true;
}

bool Board::isUnderAttack(int current_row, int current_column,
                          FigureColor attackerColor) const {
  // int pawnDir = (attackerColor == myColor) ? -1 : 1;

  // проверяем атакуют ли пешки
  for (int LR = -1; LR <= 1; LR += 2) {
    int column = current_column + LR;
    int row = current_row - 1;
    if (row >= 0 && row < 8 && column >= 0 && column < 8) {
      const Figure& figure = self[row * 8 + column];
      if (figure.is_valid() && figure.get_name() == PAWN &&
          figure.get_color() == attackerColor) {
        std::cout << "НАС АТАКУЕТ ПЕШКА " << row << ", " << column << std::endl;
        return true;
      }
    }
  }

  // проверяем атакуют ли кони
  const int knightMoves[8][2] = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
                                 {1, -2},  {1, 2},  {2, -1},  {2, 1}};
  for (int i = 0; i < 8; ++i) {
    int row = current_row + knightMoves[i][0];
    int column = current_column + knightMoves[i][1];
    if (row >= 0 && row < 8 && column >= 0 && column < 8) {
      const Figure& figure = self[row * 8 + column];
      if (figure.is_valid() && figure.get_name() == KNIGHT &&
          figure.get_color() == attackerColor) {
        std::cout << "НАС АТАКУЕТ КОНЬ " << row << ", " << column << std::endl;
        return true;
      }
    }
  }

  // проверяем атакуют ли по горизонтали или вертикали
  const int directionHV[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
  for (int i = 0; i < 4; ++i) {
    for (int j = 1; j < 8; ++j) {
      int row = current_row + directionHV[i][0] * j;
      int column = current_column + directionHV[i][1] * j;
      if (!(row >= 0 && row < 8 && column >= 0 && column < 8)) {
        break;
      }

      const Figure& figure = self[row * 8 + column];
      if (!figure.is_valid()) continue;
      if (figure.get_color() == attackerColor &&
          (figure.get_name() == ROOK || figure.get_name() == QUEEN ||
           (j == 1 ? figure.get_name() == KING : false))) {
        std::cout << "НАС АТАКУЮТ ПО ВЕРТИКАЛИ " << row << ", " << column
                  << std::endl;
        return true;
      }
      break;
    }
  }

  // проверяем атакуют ли по диагонали
  const int directionD[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
  for (int i = 0; i < 4; ++i) {
    for (int j = 1; j < 8; ++j) {
      int row = current_row + directionD[i][0] * j;
      int column = current_column + directionD[i][1] * j;
      if (!(row >= 0 && row < 8 && column >= 0 && column < 8)) {
        break;
      }

      const Figure& figure = self[row * 8 + column];
      if (!figure.is_valid()) continue;
      if (figure.get_color() == attackerColor &&
          (figure.get_name() == BISHOP || figure.get_name() == QUEEN ||
           (j == 1 ? figure.get_name() == KING : false))) {
        std::cout << "НАС АТАКУЮТ ПО ДИАГОНАЛИ " << row << ", " << column
                  << std::endl;
        return true;
      }
      break;
    }
  }

  return false;
}

bool Board::isCheck(FigureColor color) const {
  int king_row = -1;
  int king_column = -1;
  for (int i = 0; i < 64; i++) {
    const Figure& figure = self[i];
    if (figure.is_valid() && figure.get_name() == KING &&
        figure.get_color() == color) {
      king_row = i / 8;
      king_column = i % 8;
      break;
    }
  }

  return isUnderAttack(
      king_row, king_column,
      self[king_row * 8 + king_column].get_color() == BLACK ? WHITE : BLACK);
}

bool Board::isValidCastling(int king_row, int king_column, int rook_row,
                            int rook_column) const {
  const Figure& king = self[king_row * 8 + king_column];
  const Figure& rook = self[rook_row * 8 + rook_column];

  if (king_row != rook_row) {
    return false;
  }

  if (!king.is_valid() || king.get_name() != KING || !rook.is_valid() ||
      rook.get_name() != ROOK) {
    return false;
  }

  if (king.get_color() != rook.get_color()) {
    return false;
  }

  if ((king_column < rook_column && rook_column != 7) ||
      (king_column > rook_column && rook_column != 0)) {
    return false;
  }

  if (king.get_color() == BLACK) {
    if (blackKingMoved) {
      return false;
    }
    if (king_column < rook_column && blackRookMoved[1]) {
      return false;
    }
    if (king_column > rook_column && blackRookMoved[0]) {
      return false;
    }
  } else {
    if (whiteKingMoved) {
      return false;
    }
    if (king_column < rook_column && whiteRookMoved[1]) {
      return false;
    }
    if (king_column > rook_column && whiteRookMoved[0]) {
      return false;
    }
  }

  if (!isFreeRow(king_row, king_column, rook_row, rook_column)) {
    return false;
  }

  if (isUnderAttack(king_row, king_column,
                    king.get_color() == BLACK ? WHITE : BLACK)) {
    return false;
  }

  int step = king_column < rook_column ? 1 : -1;
  for (int col = king_column + step; col != king_column + 2 * step;
       col += step) {
    if (isUnderAttack(king_row, col,
                      king.get_color() == BLACK ? WHITE : BLACK)) {
      return false;
    }
  }

  return true;
}

bool Board::isValidMove(int current_row, int current_column, int last_row,
                        int last_column) const {
  const Figure& figure = self[current_row * 8 + current_column];

  if (figure.is_valid()) {
    if (current_column == last_column && current_row == last_row) {
      return false;
    }
    const Figure& targetFigure = self[last_row * 8 + last_column];
    if (figure.get_name() != KING &&
        figure.get_color() == targetFigure.get_color()) {
      return false;
    }

    switch (figure.get_name()) {
      case (KING):
        if (abs(last_column - current_column) <= 1 &&
            abs(last_row - current_row) <= 1) {
          return true;
        }
        return isValidCastling(current_row, current_column, last_row,
                               last_column);
      case (QUEEN):
        if (abs(last_column - current_column) == abs(last_row - current_row)) {
          return isFreeDiagonal(current_row, current_column, last_row,
                                last_column);
        } else if (last_column == current_column && last_row != current_row) {
          return isFreeColumn(current_row, current_column, last_row,
                              last_column);
        } else if (last_column != current_column && last_row == current_row) {
          return isFreeRow(current_row, current_column, last_row, last_column);
        }
        return false;
      case (ROOK):
        if (last_column == current_column && last_row != current_row) {
          return isFreeColumn(current_row, current_column, last_row,
                              last_column);
        } else if (last_column != current_column && last_row == current_row) {
          return isFreeRow(current_row, current_column, last_row, last_column);
        }
        return false;
      case (BISHOP):
        if (abs(last_column - current_column) == abs(last_row - current_row)) {
          return isFreeDiagonal(current_row, current_column, last_row,
                                last_column);
        }
        return false;
      case (KNIGHT):
        if ((abs(current_column - last_column) == 2 &&
             abs(current_row - last_row) == 1) ||
            (abs(current_column - last_column) == 1 &&
             abs(current_row - last_row) == 2)) {
          return true;
        }
        return false;
      case (PAWN):
        int direction = (figure.get_color() == myColor) ? -1 : 1;
        int startRow = (figure.get_color() == myColor) ? 6 : 1;

        if (!self[last_row * 8 + last_column].is_valid()) {
          if (current_column == last_column &&
              last_row - current_row == direction) {
            return true;
          } else if (current_column == last_column && current_row == startRow &&
                     last_row - current_row == 2 * direction) {
            return !self[(current_row + direction) * 8 + current_column]
                        .is_valid();
          }
        } else {
          if (last_row - current_row == direction &&
              abs(current_column - last_column) == 1) {
            return true;
          }
        }
        return false;
    }
  }
  return false;
}

bool Board::kingWouldCheck(int current_row, int current_column, int last_row,
                           int last_column) const {
  Board tmpBoard = *this;

  Figure& figure = tmpBoard.self[current_row * 8 + current_column];
  tmpBoard.self[last_row * 8 + last_column] = figure;
  tmpBoard.self[current_row * 8 + current_column] = Figure();

  return tmpBoard.isCheck(figure.get_color());
}

bool Board::canMove(FigureColor color) const {
  for (int from_row = 0; from_row < 8; from_row++) {
    for (int from_column = 0; from_column < 8; from_column++) {
      const Figure& figure = self[from_row * 8 + from_column];
      if (figure.is_valid() && figure.get_color() == color) {
        for (int to_row = 0; to_row < 8; to_row++) {
          for (int to_column = 0; to_column < 8; to_column++) {
            if (from_row == to_row && from_column == to_column) continue;
            if (isValidMove(from_row, from_column, to_row, to_column) &&
                !kingWouldCheck(from_row, from_column, to_row, to_column)) {
              return true;
            }
          }
        }
      }
    }
  }
  return false;
}

bool Board::isCheckmate(FigureColor color) const {
  return isCheck(color) && !canMove(color);
}

bool Board::isStalemate(FigureColor color) const {
  return !isCheck(color) && !canMove(color);
}

bool Board::move(int current_row, int current_column, int last_row,
                 int last_column) {
  if (!isValidMove(current_row, current_column, last_row, last_column)) {
    return false;
  }

  if (kingWouldCheck(current_row, current_column, last_row, last_column)) {
    return false;
  }

  Figure& figure = self[current_row * 8 + current_column];

  bool isCastling = false;
  if (figure.get_name() == KING) {
    if (isValidCastling(current_row, current_column, last_row, last_column)) {
      isCastling = true;
      if (current_column < last_column) {
        self[current_row * 8 + (current_column + 1)] =
            self[current_row * 8 + 7];
        self[current_row * 8 + 7] = Figure();
        if (figure.get_color() == WHITE) {
          whiteRookMoved[1] = true;
        } else {
          blackRookMoved[1] = true;
        }
      } else {
        self[current_row * 8 + (current_column - 1)] = self[current_row * 8];
        self[current_row * 8] = Figure();
        if (figure.get_color() == WHITE) {
          whiteRookMoved[0] = true;
        } else {
          blackRookMoved[0] = true;
        }
      }
      if (figure.get_color() == WHITE) {
        whiteKingMoved = true;
      } else {
        blackKingMoved = true;
      }
    }
  }
  if (figure.get_name() == ROOK) {
    if (figure.get_color() == WHITE) {
      if (current_column == 0) {
        whiteRookMoved[0] = true;
      } else if (current_column == 7) {
        whiteRookMoved[1] = true;
      }
    } else {
      if (current_column == 0) {
        blackRookMoved[0] = true;
      } else if (current_column == 7) {
        blackRookMoved[1] = true;
      }
    }
  }

  if (figure.get_name() == PAWN) {
    if (figure.get_color() == myColor && last_row == 0) {
      figure = Figure(QUEEN, myColor);
    } else if (figure.get_color() != myColor && last_row == 7) {
      figure = Figure(QUEEN, figure.get_color());
    }
  }

  if (isCastling) {
    int d = current_column < last_column ? 2 : -2;
    self[current_row * 8 + current_column + d] = figure;
    self[current_row * 8 + current_column] = Figure();
  } else {
    self[last_row * 8 + last_column] = figure;
    self[current_row * 8 + current_column] = Figure();
  }

  whiteCheck = isCheck(WHITE);
  blackCheck = isCheck(BLACK);
  return true;
}
