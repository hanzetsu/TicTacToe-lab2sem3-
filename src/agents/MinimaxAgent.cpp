#include "agents/MinimaxAgent.hpp"

MinimaxAgent::MinimaxAgent(int md) : maxDepth(md) {}

int MinimaxAgent::evaluate(const Board& board, int depth) const {
    if (board.isWin(1)) return  10000 - depth;
    if (board.isWin(2)) return -10000 + depth;
    if (board.isDraw()) return 0;
    return board.evaluate(1);
}

int MinimaxAgent::minimax(Board& board, int depth, bool isMaximizing) const {
    if (board.isGameOver() || depth >= maxDepth) {
        return evaluate(board, depth);
    }

    auto moves = board.getEmptyCells();

    if (isMaximizing) {
        int best = -100000;
        for (int i = 0; i < moves.GetLength(); ++i) {
            int move = moves.Get(i);
            board.makeMove(move);
            int score = minimax(board, depth + 1, false);
            board.undoMove(move);
            if (score > best) best = score;
        }
        return best;
    } else {
        int best = 100000;
        for (int i = 0; i < moves.GetLength(); ++i) {
            int move = moves.Get(i);
            board.makeMove(move);
            int score = minimax(board, depth + 1, true);
            board.undoMove(move);
            if (score < best) best = score;
        }
        return best;
    }
}

int MinimaxAgent::chooseMove(const Board& board) const {
    Board copy = board;
    int player = copy.whoseMove();

    int bestMove = -1;
    int bestScore = (player == 1) ? -100000 : 100000;

    auto moves = copy.getEmptyCells();
    for (int i = 0; i < moves.GetLength(); ++i) {
        int move = moves.Get(i);
        copy.makeMove(move);
        int score = minimax(copy, 0, player == 2);
        copy.undoMove(move);

        if (player == 1) {
            if (score > bestScore) { bestScore = score; bestMove = move; }
        } else {
            if (score < bestScore) { bestScore = score; bestMove = move; }
        }
    }

    return bestMove;
}