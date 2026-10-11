#pragma once
#include "agents/IAgent.hpp"
#include "structures/HashTableDictionary.hpp"
#include "core/Board.hpp"

class CachedMinimaxAgent : public IAgent {
public:
    explicit CachedMinimaxAgent(int maxDepth = 9);
    int chooseMove(const Board& board) const override;

private:
    int maxDepth;

    int evaluate(const Board& board, int depth) const;
    int minimaxCached(Board& board, int depth, bool isMaximizing,
                      HashTableDictionary<uint64_t, int>& cache) const;
};