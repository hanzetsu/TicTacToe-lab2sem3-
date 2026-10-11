#pragma once
#include "agents/IAgent.hpp"
#include "core/Board.hpp"

class AlphaBetaAgent : public IAgent {
public:
    explicit AlphaBetaAgent(int maxDepth = 9);
    int chooseMove(const Board& board) const override;

private:
    int maxDepth;
    int evaluate(const Board& board, int depth) const;
    int alphaBeta(Board& board, int depth, int alpha, int beta,
                  bool isMaximizing) const;
};