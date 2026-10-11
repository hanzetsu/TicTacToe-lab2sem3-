#pragma once
#include "agents/IAgent.hpp"
#include "structures/HashTableDictionary.hpp"
#include "core/Board.hpp"

class CachedMinimaxAgent : public IAgent {
public:
    int chooseMove(const Board& board) const override;

private:
    int evaluate(const Board& board, int depth) const;
    int minimaxCached(Board& board, int depth, bool isMaximizing,
                      HashTableDictionary<uint32_t, int>& cache) const;
};