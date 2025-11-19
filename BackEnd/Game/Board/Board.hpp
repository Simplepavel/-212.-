#include <vector>

#include "../Figure/Figure.hpp"
class Board {
 private:
  std::vector<Figure> self;
  bool isFreeRow(int current_row, int current_column, int last_row,
                 int last_column) const;
  bool isFreeColumn(int current_row, int current_column, int last_row,
                    int last_column) const;
  bool isFreeDiagonal(int current_row, int current_column, int last_row,
                      int last_column) const;

 public:
  Board();
  char* serialize();
  void deserialize(char*);
  uint16_t capacity() { return 128; }
  void show() const;
  bool isValidMove(int current_row, int current_column, int last_row,
                   int last_column) const;
  Figure& operator[](int idx);
};
