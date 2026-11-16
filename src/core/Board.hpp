#pragma once
#include <cstdint>
#include <iostream>
#include "MutableArraySequence.hpp"

using Score = int; 

enum class GameState {
    Ongoing,
    WinX,
    WinO,
    Draw
};

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
    int  getCell(int index) const;
    bool isCellTaken(int row, int col) const;
    bool makeMove(int row, int col);
    bool makeMove(int index);
    bool undoMove(int row, int col);
    bool undoMove(int index);
    int  countMoves() const;
    int  whoseMove() const;
    bool isWin(int player) const;
    bool isDraw() const;
    
    GameState getGameState() const; // enum
    
    uint64_t getState() const;
    void print() const;
    Score evaluate(int player) const; //возвращает score 
};