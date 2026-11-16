#pragma once
#include "core/Board.hpp"
#include "IAgent.hpp"

class MinimaxAgent : public IAgent {
public:
    MinimaxAgent(int maxDepth = 9);  
    int chooseMove(const Board& board) const override;
private:
    int maxDepth;                        
    Score evaluate(const Board& board, int depth, int currentPlayer) const;
    Score negamax(Board& board, int depth, int currentPlayer) const;
};