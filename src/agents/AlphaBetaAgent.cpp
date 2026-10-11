#include "agents/AlphaBetaAgent.hpp"

AlphaBetaAgent::AlphaBetaAgent(int md) : maxDepth(md) {}

int AlphaBetaAgent::evaluate(const Board& board, int depth) const {
    if (board.isWin(1)) return  10000 - depth;
    if (board.isWin(2)) return -10000 + depth;
    if (board.isDraw()) return 0;
    return board.evaluate(1);   // эвристика, если глубина исчерпана
}

int AlphaBetaAgent::alphaBeta(Board& board, int depth, int alpha, int beta,
                               bool isMaximizing) const {
    if (board.isGameOver() || depth >= maxDepth) {
        return evaluate(board, depth);
    }

    auto moves = board.getEmptyCells();

    if (isMaximizing) {
        int best = -100000;
        for (int i = 0; i < moves.GetLength(); ++i) {
            int move = moves.Get(i);
            board.makeMove(move);
            int score = alphaBeta(board, depth + 1, alpha, beta, false);
            board.undoMove(move);

            if (score > best) best = score;
            if (best > alpha) alpha = best;
            if (alpha >= beta) break;
        }
        return best;
    } else {
        int best = 100000;
        for (int i = 0; i < moves.GetLength(); ++i) {
            int move = moves.Get(i);
            board.makeMove(move);
            int score = alphaBeta(board, depth + 1, alpha, beta, true);
            board.undoMove(move);

            if (score < best) best = score;
            if (best < beta) beta = best;
            if (alpha >= beta) break;
        }
        return best;
    }
}

int AlphaBetaAgent::chooseMove(const Board& board) const {
    Board copy = board;
    int player = copy.whoseMove();

    int bestMove = -1;
    int bestScore = (player == 1) ? -100000 : 100000;

    auto moves = copy.getEmptyCells();
    for (int i = 0; i < moves.GetLength(); ++i) {
        int move = moves.Get(i);
        copy.makeMove(move);
        int score = alphaBeta(copy, 0, -100000, 100000, player == 2);
        copy.undoMove(move);

        if (player == 1) {
            if (score > bestScore) { bestScore = score; bestMove = move; }
        } else {
            if (score < bestScore) { bestScore = score; bestMove = move; }
        }
    }

    return bestMove;
}