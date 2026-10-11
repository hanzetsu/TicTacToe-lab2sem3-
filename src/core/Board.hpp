#pragma once
#include <vector>
#include <cstdint>
#include <iostream>
#include "MutableArraySequence.hpp"

class Board {
private:
    int N;
    int K;
    std::vector<int> cells;

    void checkIndex(int r, int c) const;

public:
    Board(int n = 3, int k = 3);

    int GetSize() const { return N; }
    int GetK()    const { return K; }

    int  getCell(int r, int c) const;
    bool isCellTaken(int r, int c) const;

    bool makeMove(int r, int c);
    bool makeMove(int index);
    bool undoMove(int r, int c);
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