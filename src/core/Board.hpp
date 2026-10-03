#pragma once
#include "Exceptions.hpp"
#include <iostream>
class Board {
    private:
    int board[3][3] = {0};
    void CheckIndex(int r, int c) const;
    public:
    static constexpr int SIZE_OF_BOARD = 3;
    Board() = default;
    ~Board() = default;
    int getCell (int row, int col) const;
    int countMoves () const;
    int whoseMove() const;
    bool makeMove (int row, int col);
    bool undoMove (int row, int col);
    bool isCellTaken(int r, int c) const;
    bool isDraw() const;
    bool isWin(int player) const;
    bool isGameOver() const;
    void print() const;
};