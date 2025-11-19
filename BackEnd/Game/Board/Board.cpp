#include "board.h"

Board::Board() {
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
  self[7 * 8 + 7] = Figure(FigureName::ROOK, FigureColor::BLACK);    // h8

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
  self[0 * 8 + 7] = Figure(FigureName::ROOK, FigureColor::WHITE);    // h1

  // Белые пешки
  for (int i = 0; i < 8; ++i) {
    self[1 * 8 + i] = Figure(FigureName::PAWN, FigureColor::WHITE);  // линия 2
  }
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

bool Board::isValidMove(int current_row, int current_column, int last_row,
                        int last_column) const {
  const Figure& figure = self[current_row * 8 + current_column];
  if (figure.is_valid()) {
    if (current_column == last_column && current_row == last_row) {
      return false;
    }
    switch (figure.get_name()) {
      case (KING):
        if (abs(last_column - current_column) <= 1 &&
            abs(last_row - current_row) <= 1) {
          return !self[last_row * 8 + last_column].is_valid();
        }
        return false;
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
          return !self[last_row * 8 + last_column].is_valid();
        }
        return false;
      case (PAWN):
        if (figure.get_color() == BLACK) {
          if (current_column == last_column && last_row - current_row == 1) {
            return !self[last_row * 8 + last_column].is_valid();
          } else if (current_column == last_column && current_row == 1 &&
                     last_row - current_row == 2) {
            return !self[(current_row + 1) * 8 + current_column].is_valid() &&
                   !self[last_row * 8 + last_column].is_valid();
          }
        }
        if (figure.get_color() == WHITE) {
          if (current_column == last_column && current_row - last_row == 1) {
            return !self[last_row * 8 + last_column].is_valid();
          } else if (current_column == last_column && current_row == 6 &&
                     current_row - last_row == 2) {
            return !self[(current_row - 1) * 8 + current_column].is_valid() &&
                   !self[last_row * 8 + last_column].is_valid();
          }
        }
        return false;
    }
  }
  return false;
}

Figure& Board::operator[](int idx) { return self[idx]; }