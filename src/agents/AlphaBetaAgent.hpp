#pragma once
#include <cstdint>
#include "agents/IAgent.hpp"
#include "core/Board.hpp"
#include "MutableArraySequence.hpp"
#include "structures/HashTableDictionary.hpp"

class AlphaBetaAgent : public IAgent {
public:
    explicit AlphaBetaAgent(int maxDepth = 9);
    int chooseMove(const Board& board) const override;

    void setCacheEnabled(bool enabled) { useCache = enabled; }
    bool isCacheEnabled() const { return useCache; }

    enum class BoundFlag : uint8_t { Exact = 0, Lower = 1, Upper = 2 };

    struct TTEntry {
        int value;
        BoundFlag flag;
        TTEntry() : value(0), flag(BoundFlag::Exact) {}
        TTEntry(int val, BoundFlag f) : value(val), flag(f) {}
    };

private:
    using Cache = HashTableDictionary<uint64_t, TTEntry>;
    
    int maxDepth;
    bool useCache = false;

    int evaluate(const Board& board, int depth) const;
    
    int getCenterDistance(int cellIndex, int boardSize) const;

    MutableArraySequence<int> getOrderedMoves(const Board& board) const;
    
    int alphaBetaSearch(Board& board, int depth, int alpha, int beta,
                        bool isMaximizing, Cache* transpositionTable) const;
};