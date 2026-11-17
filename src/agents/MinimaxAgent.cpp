#include "agents/MinimaxAgent.hpp"
#include <limits>  // для std::numeric_limits

// Определяем константы на основе типа Score
static constexpr Score WIN_SCORE = std::numeric_limits<Score>::max() / 2;
static constexpr Score NEG_INF   = std::numeric_limits<Score>::lowest() / 2;

MinimaxAgent::MinimaxAgent(int maxDepth) : maxDepth(maxDepth) {}

Score MinimaxAgent::evaluate(const Board& board, int depth, int currentPlayer) const {
    GameState state = board.getGameState();
    
    if (state == GameState::WinX) 
        return (currentPlayer == 1) ? (WIN_SCORE - depth) : (-WIN_SCORE + depth);
    if (state == GameState::WinO) 
        return (currentPlayer == 2) ? (WIN_SCORE - depth) : (-WIN_SCORE + depth);
    if (state == GameState::Draw) 
        return 0;
    
    return board.evaluate(currentPlayer); 
}

Score MinimaxAgent::minimax(Board& board, int depth, int currentPlayer) const {
    if (board.getGameState() != GameState::Ongoing || depth >= maxDepth) {
        return evaluate(board, depth, currentPlayer);
    }

    Score bestScore = NEG_INF;
    int size = board.GetSize();
    
    for (int i = 0; i < size * size; ++i) {
        if (board.getCell(i) == 0) {
            board.makeMove(i);
            
            Score currentScore = -minimax(board, depth + 1, 3 - currentPlayer);
            
            board.undoMove(i);
            
            if (currentScore > bestScore) {
                bestScore = currentScore;
            }
        }
    }
    return bestScore;
}

int MinimaxAgent::chooseMove(const Board& board) const {
    Board copy = board;
    int player = copy.whoseMove();
    int bestMove = -1;
    Score bestScore = NEG_INF;
    int size = copy.GetSize();

    for (int i = 0; i < size * size; ++i) {
        if (copy.getCell(i) == 0) {
            copy.makeMove(i);
            
            Score score = -minimax(copy, 0, 3 - player);
            
            copy.undoMove(i);
            
            if (score > bestScore) {
                bestScore = score;
                bestMove = i;
            }
        }
    }
    return bestMove;
}