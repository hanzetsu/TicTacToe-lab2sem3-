#include "agents/CachedMinimaxAgent.hpp"

CachedMinimaxAgent::CachedMinimaxAgent(int md) : maxDepth(md) {}

int CachedMinimaxAgent::evaluate(const Board& board, int depth) const {
    if (board.isWin(1)) return  10000 - depth;
    if (board.isWin(2)) return -10000 + depth;
    if (board.isDraw()) return 0;
    return board.evaluate(1);   // эвристика, если глубина исчерпана
}

int CachedMinimaxAgent::minimaxCached(Board& board, int depth, bool isMaximizing,
                                       HashTableDictionary<uint64_t, int>& cache) const {
    uint64_t key = board.getState();
    if (cache.ContainsKey(key)) {
        return cache.Get(key);
    }

    int result;

    if (board.isGameOver() || depth >= maxDepth) {
        result = evaluate(board, depth);
    } else {
        auto moves = board.getEmptyCells();
        if (isMaximizing) {
            int best = -100000;
            for (int i = 0; i < moves.GetLength(); ++i) {
                int move = moves.Get(i);
                board.makeMove(move);
                int score = minimaxCached(board, depth + 1, false, cache);
                board.undoMove(move);
                if (score > best) best = score;
            }
            result = best;
        } else {
            int best = 100000;
            for (int i = 0; i < moves.GetLength(); ++i) {
                int move = moves.Get(i);
                board.makeMove(move);
                int score = minimaxCached(board, depth + 1, true, cache);
                board.undoMove(move);
                if (score < best) best = score;
            }
            result = best;
        }
    }

    cache.Add(key, result);
    return result;
}

int CachedMinimaxAgent::chooseMove(const Board& board) const {
    Board copy = board;
    int player = copy.whoseMove();

    HashTableDictionary<uint64_t, int> cache(1 << 20);   // 1 048 576 бакетов

    int bestMove = -1;
    int bestScore = (player == 1) ? -100000 : 100000;

    auto moves = copy.getEmptyCells();
    for (int i = 0; i < moves.GetLength(); ++i) {
        int move = moves.Get(i);
        copy.makeMove(move);
        int score = minimaxCached(copy, 0, player == 2, cache);
        copy.undoMove(move);

        if (player == 1) {
            if (score > bestScore) { bestScore = score; bestMove = move; }
        } else {
            if (score < bestScore) { bestScore = score; bestMove = move; }
        }
    }

    return bestMove;
}