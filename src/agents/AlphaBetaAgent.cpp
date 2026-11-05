#include "agents/AlphaBetaAgent.hpp"
#include <algorithm>

AlphaBetaAgent::AlphaBetaAgent(int maxDepth) : maxDepth(maxDepth) {}

int AlphaBetaAgent::evaluate(const Board& board, int depth) const {
    if (board.isWin(1)) return  10000 - depth;
    if (board.isWin(2)) return -10000 + depth;
    if (board.isDraw()) return 0;
    return board.evaluate(1);
}

int AlphaBetaAgent::getCenterDistance(int cellIndex, int boardSize) const {
    int row = cellIndex / boardSize;
    int col = cellIndex % boardSize;
    int centerRow = boardSize / 2;
    int centerCol = boardSize / 2;
    
    int rowDist = (row > centerRow) ? (row - centerRow) : (centerRow - row);
    int colDist = (col > centerCol) ? (col - centerCol) : (centerCol - col);
    
    return rowDist + colDist;
}

MutableArraySequence<int> AlphaBetaAgent::getOrderedMoves(const Board& board) const {
    MutableArraySequence<int> cells = board.getEmptyCells();
    int boardSize = board.GetSize();
    int n = cells.GetLength();

    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            int dist1 = getCenterDistance(cells.Get(j), boardSize);
            int dist2 = getCenterDistance(cells.Get(j + 1), boardSize);
            
            if (dist1 > dist2) {
                int temp = cells.Get(j);
                cells.Set(j, cells.Get(j + 1));
                cells.Set(j + 1, temp);
            }
        }
    }
    return cells;
}

int AlphaBetaAgent::alphaBetaSearch(Board& board, int depth, int alpha, int beta,
                                    bool isMaximizing, Cache* transpositionTable) const {
    uint64_t boardStateKey = 0;
    int initialAlpha = alpha;
    int initialBeta  = beta;

    if (transpositionTable) {
        boardStateKey = board.getState();
        if (transpositionTable->ContainsKey(boardStateKey)) {
            TTEntry entry = transpositionTable->Get(boardStateKey);
            
            if (entry.flag == BoundFlag::Exact) return entry.value;
            if (entry.flag == BoundFlag::Lower && entry.value >= beta)  return entry.value;
            if (entry.flag == BoundFlag::Upper && entry.value <= alpha) return entry.value;
            
            if (entry.flag == BoundFlag::Lower && entry.value > alpha) alpha = entry.value;
            if (entry.flag == BoundFlag::Upper && entry.value < beta)  beta  = entry.value;
        }
    }

    int finalScore;
       
    if (board.isGameOver() || depth >= maxDepth) {
        finalScore = evaluate(board, depth);
    } else {
        MutableArraySequence<int> availableMoves = getOrderedMoves(board);

        if (isMaximizing) {
            int bestScore = -100000;
            for (int i = 0; i < availableMoves.GetLength(); ++i) {
                int cellIndex = availableMoves.Get(i);
                board.makeMove(cellIndex);
                int currentScore = alphaBetaSearch(board, depth + 1, alpha, beta, false, transpositionTable);
                board.undoMove(cellIndex);

                if (currentScore > bestScore) bestScore = currentScore;
                if (bestScore > alpha) alpha = bestScore;
                if (alpha >= beta) break;
            }
            finalScore = bestScore;
        } 
        else {
            int bestScore = 100000;
            for (int i = 0; i < availableMoves.GetLength(); ++i) {
                int cellIndex = availableMoves.Get(i);
                board.makeMove(cellIndex);
                int currentScore = alphaBetaSearch(board, depth + 1, alpha, beta, true, transpositionTable);
                board.undoMove(cellIndex);

                if (currentScore < bestScore) bestScore = currentScore;
                if (bestScore < beta) beta = bestScore;
                if (alpha >= beta) break; 
            }
            finalScore = bestScore;
        }
    }

    if (transpositionTable) {
        BoundFlag boundFlag;
        if      (finalScore <= initialAlpha) boundFlag = BoundFlag::Upper;
        else if (finalScore >= initialBeta)  boundFlag = BoundFlag::Lower;
        else                                 boundFlag = BoundFlag::Exact;
        
        transpositionTable->Add(boardStateKey, TTEntry(finalScore, boundFlag));
    }

    return finalScore;
}

int AlphaBetaAgent::chooseMove(const Board& board) const {
    Board boardCopy = board;
    int currentPlayer = boardCopy.whoseMove();

    Cache* transpositionTable = nullptr;
    if (useCache) {
        transpositionTable = new Cache(1 << 20); 
    }

    int bestMove = -1;
    int bestScore = (currentPlayer == 1) ? -100000 : 100000;
    MutableArraySequence<int> availableMoves = getOrderedMoves(boardCopy);

    for (int i = 0; i < availableMoves.GetLength(); ++i) {
        int cellIndex = availableMoves.Get(i);
        boardCopy.makeMove(cellIndex);
        
        int currentScore = alphaBetaSearch(boardCopy, 0, -100000, 100000, 
                                           currentPlayer == 2, transpositionTable);
        boardCopy.undoMove(cellIndex);

        if (currentPlayer == 1) {
            if (currentScore > bestScore) { bestScore = currentScore; bestMove = cellIndex; }
        } else {
            if (currentScore < bestScore) { bestScore = currentScore; bestMove = cellIndex; }
        }
    }

    if (transpositionTable) {
        delete transpositionTable;
    }
    
    return bestMove;
}