#include "core/Board.hpp"
#include "Exceptions.hpp"

Board::Board(int n, int k) : boardSize(n), winLength(k) {
    for (int i = 0; i < boardSize * boardSize; ++i) {
        cells.Append(0);
    }
}

void Board::checkIndex(int row, int col) const {
    if (row < 0 || row >= boardSize)
        throw IndexOutOfRange(static_cast<std::size_t>(row), static_cast<std::size_t>(boardSize), 
                              "Board::checkIndex: row out of range");
    if (col < 0 || col >= boardSize)
        throw IndexOutOfRange(static_cast<std::size_t>(col), static_cast<std::size_t>(boardSize), 
                              "Board::checkIndex: col out of range");
}

int Board::getCell(int row, int col) const {
    checkIndex(row, col);
    return cells.Get(row * boardSize + col);
}

int Board::getCell(int index) const {
    return cells.Get(index);
}

bool Board::isCellTaken(int row, int col) const {
    checkIndex(row, col);
    return cells.Get(row * boardSize + col) != 0;
}

bool Board::makeMove(int row, int col) {
    checkIndex(row, col);
    int idx = row * boardSize + col;
    if (cells.Get(idx) != 0) return false;
    cells.Set(idx, whoseMove());
    return true;
}

bool Board::makeMove(int index) {
    return makeMove(index / boardSize, index % boardSize);
}

bool Board::undoMove(int row, int col) {
    checkIndex(row, col);
    int idx = row * boardSize + col;
    if (cells.Get(idx) == 0) return false;
    cells.Set(idx, 0);
    return true;
}

bool Board::undoMove(int index) {
    return undoMove(index / boardSize, index % boardSize);
}

int Board::countMoves() const {
    int cnt = 0;
    for (int i = 0; i < boardSize * boardSize; ++i) {
        if (cells.Get(i) != 0) ++cnt;
    }
    return cnt;
}

int Board::whoseMove() const {
    return (countMoves() % 2) + 1;
}

bool Board::isWin(int player) const {
    for (int row = 0; row < boardSize; ++row)
        for (int col = 0; col + winLength <= boardSize; ++col) {
            bool ok = true;
            for (int k = 0; k < winLength; ++k)
                if (cells.Get(row * boardSize + col + k) != player) { ok = false; break; }
            if (ok) return true;
        }
    for (int row = 0; row + winLength <= boardSize; ++row)
        for (int col = 0; col < boardSize; ++col) {
            bool ok = true;
            for (int k = 0; k < winLength; ++k)
                if (cells.Get((row + k) * boardSize + col) != player) { ok = false; break; }
            if (ok) return true;
        }
    for (int row = 0; row + winLength <= boardSize; ++row)
        for (int col = 0; col + winLength <= boardSize; ++col) {
            bool ok = true;
            for (int k = 0; k < winLength; ++k)
                if (cells.Get((row + k) * boardSize + col + k) != player) { ok = false; break; }
            if (ok) return true;
        }
    for (int row = 0; row + winLength <= boardSize; ++row)
        for (int col = winLength - 1; col < boardSize; ++col) {
            bool ok = true;
            for (int k = 0; k < winLength; ++k)
                if (cells.Get((row + k) * boardSize + col - k) != player) { ok = false; break; }
            if (ok) return true;
        }
    return false;
}

bool Board::isDraw() const {
    if (isWin(1) || isWin(2)) return false;
    for (int i = 0; i < boardSize * boardSize; ++i) {
        if (cells.Get(i) == 0) return false;
    }
    return true;
}

GameState Board::getGameState() const {
    if (isWin(1)) return GameState::WinX;
    if (isWin(2)) return GameState::WinO;
    if (isDraw()) return GameState::Draw;
    return GameState::Ongoing;
}

uint64_t Board::getState() const { 
    uint64_t state = 0;
    for (int i = 0; i < boardSize * boardSize; ++i)
        state |= (uint64_t(cells.Get(i)) << (i * 2));
    return state;
}

void Board::print() const {
    for (int row = 0; row < boardSize; ++row) {
        for (int col = 0; col < boardSize; ++col) {
            char ch = '.';
            int val = cells.Get(row * boardSize + col);
            if (val == 1) ch = 'X';
            else if (val == 2) ch = 'O';
            std::cout << ch << ' ';
        }
        std::cout << '\n';
    }
}

Score Board::evaluate(int player) const {
    Score score = 0;

    auto countLine = [&](int startRow, int startCol, int rowStep, int colStep) {
        int xCount = 0;
        int oCount = 0;

        for (int k = 0; k < winLength; ++k) {
            int val = cells.Get((startRow + rowStep * k) * boardSize + (startCol + colStep * k));
            if (val == 1) ++xCount;
            else if (val == 2) ++oCount;
        }

        if (oCount == 0) {
            if (xCount == 1) score += 1;
            else if (xCount == 2) score += 10;
            else if (xCount == 3) score += 100;
            else if (xCount == 4) score += 1000;
        }
        
        if (xCount == 0) {
            if (oCount == 1) score -= 1;
            else if (oCount == 2) score -= 10;
            else if (oCount == 3) score -= 100;
            else if (oCount == 4) score -= 1000;
        }
    };

    for (int row = 0; row < boardSize; ++row)
        for (int col = 0; col + winLength <= boardSize; ++col) 
            countLine(row, col, 0, 1);

    for (int row = 0; row + winLength <= boardSize; ++row)
        for (int col = 0; col < boardSize; ++col) 
            countLine(row, col, 1, 0);

    for (int row = 0; row + winLength <= boardSize; ++row)
        for (int col = 0; col + winLength <= boardSize; ++col) 
            countLine(row, col, 1, 1);

    for (int row = 0; row + winLength <= boardSize; ++row)
        for (int col = winLength - 1; col < boardSize; ++col) 
            countLine(row, col, 1, -1);

    return (player == 1) ? score : -score;
}