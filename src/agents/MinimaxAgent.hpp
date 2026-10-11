#pragma once
#include "core/Board.hpp"
#include "IAgent.hpp"

class MinimaxAgent : public IAgent {
public:
    explicit MinimaxAgent(int maxDepth = 9);  
    int chooseMove(const Board& board) const override;
private:
    int maxDepth;                        
    int evaluate(const Board& board, int depth) const;
    int minimax(Board& board, int depth, bool isMaximizing) const;
};