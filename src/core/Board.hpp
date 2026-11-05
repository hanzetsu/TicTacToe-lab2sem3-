#pragma once
#include <cstdint>
#include <iostream>
#include "MutableArraySequence.hpp"

class Board {
private:
    int boardSize;              
    int winLength;               
    MutableArraySequence<int> cells;

    void checkIndex(int row, int col) const;

public:
    Board(int n = 3, int k = 3);

    int GetSize() const { return boardSize; }
    int GetK()    const { return winLength; }

    int  getCell(int row, int col) const;
    bool isCellTaken(int row, int col) const;

    bool makeMove(int row, int col);
    bool makeMove(int index);
    bool undoMove(int row, int col);
    bool undoMove(int index);

    int  countMoves() const;
    int  whoseMove() const;

    bool isWin(int player) const;
    bool isDraw() const;
    bool isGameOver() const;

    uint64_t getState() const;
    MutableArraySequence<int> getEmptyCells() const;

    void print() const;
    int evaluate(int player) const;
};