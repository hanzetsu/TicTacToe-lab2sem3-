#include "agents/CachedMinimaxAgent.hpp"

int CachedMinimaxAgent::evaluate(const Board& board, int depth) const {
    if (board.isWin(1)) return  10 - depth;
    if (board.isWin(2)) return -10 + depth;
    return 0;
}

int CachedMinimaxAgent::minimaxCached(Board& board, int depth, bool isMaximizing,
                                       HashTableDictionary<uint32_t, int>& cache) const {
    if (board.isGameOver()) {
        uint32_t key = board.getState();
        if (cache.ContainsKey(key)) {
            return cache.Get(key);
        }
        int result = evaluate(board, depth);
        cache.Add(key, result);
        return result;
    }

    auto moves = board.getEmptyCells();

    if (isMaximizing) {
        int best = -1000;
        for (int i = 0; i < moves.GetLength(); ++i) {
            int move = moves.Get(i);
            board.makeMove(move);
            int score = minimaxCached(board, depth + 1, false, cache);
            board.undoMove(move);
            if (score > best) best = score;
        }
        return best;
    } else {
        int best = 1000;
        for (int i = 0; i < moves.GetLength(); ++i) {
            int move = moves.Get(i);
            board.makeMove(move);
            int score = minimaxCached(board, depth + 1, true, cache);
            board.undoMove(move);
            if (score < best) best = score;
        }
        return best;
    }
}

int CachedMinimaxAgent::chooseMove(const Board& board) const {
    Board copy = board;
    int player = copy.whoseMove();

    HashTableDictionary<uint32_t, int> cache(4096);

    int bestMove = -1;
    int bestScore = (player == 1) ? -1000 : 1000;

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