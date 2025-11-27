#include <vector>

#include "../Figure/Figure.hpp"

#define DEBUG


class Board
{
private:
    // Сеня
    bool isFreeRow(int current_row, int current_column, int last_row,
                   int last_column) const;
    bool isFreeColumn(int current_row, int current_column, int last_row,
                      int last_column) const;
    bool isFreeDiagonal(int current_row, int current_column, int last_row,
                        int last_column) const;
    bool canMove(FigureColor color) const;

    bool whiteKingMoved = false;
    bool blackKingMoved = false;
    bool whiteRookMoved[2] = {false, false}; // 0-левая 1-правая
    bool blackRookMoved[2] = {false, false};

    bool whiteCheck = false;
    bool blackCheck = false;
    FigureColor myColor;
    // Сеня

    // Мое
    std::vector<Figure> self;
    std::vector<uint8_t> LastMoves; // последний ход
                                    // Мое

public:
    Board(FigureColor);
    // Сеня
    bool isValidMove(int current_row, int current_column, int last_row,
                     int last_column) const;
    bool isValidCastling(int king_row, int king_column, int rook_row,
                         int rook_column) const;

    bool isUnderAttack(int current_row, int current_column,
                       FigureColor attackerColor) const;
    bool isCheck(FigureColor color) const;
    bool kingWouldCheck(int current_row, int current_column, int last_row,
                        int last_column) const;
    bool isCheckmate(FigureColor color) const;
    bool isStalemate(FigureColor color) const;
    bool move(int current_row, int current_column, int last_row, int last_column);
    // Сеня

    // Мое
    char *serialize();
    void deserialize(char *);
    uint16_t capacity() { return 128; }
    Figure &operator[](int idx);
    // void replace(int c_row, int c_column, int l_row, int l_column);
    void replace();
    char *SerializeMove();
    void DeserializeMove(char *);

    // Мое
};
