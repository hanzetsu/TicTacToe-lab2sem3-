#pragma once
#include "core/Board.hpp"
#include "IAgent.hpp"

class MinimaxAgent : public IAgent{
public:
    int chooseMove(const Board& board) const;

private:
    int evaluate(const Board& board, int depth) const;
    int minimax(Board& board, int depth, bool isMaximizing) const;
};