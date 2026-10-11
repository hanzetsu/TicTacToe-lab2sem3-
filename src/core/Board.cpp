#include "core/Board.hpp"
#include "Exceptions.hpp"

Board::Board(int n, int k) : N(n), K(k), cells(n * n, 0) {}

void Board::checkIndex(int r, int c) const {
    if (r < 0 || r >= N)
        throw IndexOutOfRange(r, N, "Board::checkIndex: row out of range");
    if (c < 0 || c >= N)
        throw IndexOutOfRange(c, N, "Board::checkIndex: col out of range");
}

int Board::getCell(int r, int c) const {
    checkIndex(r, c);
    return cells[r * N + c];
}

bool Board::isCellTaken(int r, int c) const {
    checkIndex(r, c);
    return cells[r * N + c] != 0;
}

bool Board::makeMove(int r, int c) {
    checkIndex(r, c);
    if (cells[r * N + c] != 0) return false;
    cells[r * N + c] = whoseMove();
    return true;
}

bool Board::makeMove(int index) {
    return makeMove(index / N, index % N);
}

bool Board::undoMove(int r, int c) {
    checkIndex(r, c);
    if (cells[r * N + c] == 0) return false;
    cells[r * N + c] = 0;
    return true;
}

bool Board::undoMove(int index) {
    return undoMove(index / N, index % N);
}
int Board::countMoves() const {
    int cnt = 0;
    for (int v : cells) if (v != 0) ++cnt;
    return cnt;
}

int Board::whoseMove() const {
    return (countMoves() % 2) + 1;
}

bool Board::isWin(int player) const {
    // Горизонтали
    for (int r = 0; r < N; ++r)
        for (int c = 0; c + K <= N; ++c) {
            bool ok = true;
            for (int k = 0; k < K; ++k)
                if (cells[r * N + c + k] != player) { ok = false; break; }
            if (ok) return true;
        }
    // Вертикали
    for (int r = 0; r + K <= N; ++r)
        for (int c = 0; c < N; ++c) {
            bool ok = true;
            for (int k = 0; k < K; ++k)
                if (cells[(r + k) * N + c] != player) { ok = false; break; }
            if (ok) return true;
        }
    // Диагонали ↘
    for (int r = 0; r + K <= N; ++r)
        for (int c = 0; c + K <= N; ++c) {
            bool ok = true;
            for (int k = 0; k < K; ++k)
                if (cells[(r + k) * N + c + k] != player) { ok = false; break; }
            if (ok) return true;
        }
    // Диагонали ↙
    for (int r = 0; r + K <= N; ++r)
        for (int c = K - 1; c < N; ++c) {
            bool ok = true;
            for (int k = 0; k < K; ++k)
                if (cells[(r + k) * N + c - k] != player) { ok = false; break; }
            if (ok) return true;
        }
    return false;
}

bool Board::isDraw() const {
    if (isWin(1) || isWin(2)) return false;
    for (int v : cells) if (v == 0) return false;
    return true;
}

bool Board::isGameOver() const {
    return isWin(1) || isWin(2) || isDraw();
}

uint64_t Board::getState() const {
    uint64_t s = 0;
    for (int i = 0; i < N * N; ++i)
        s |= (uint64_t(cells[i]) << (i * 2));
    return s;
}

MutableArraySequence<int> Board::getEmptyCells() const {
    MutableArraySequence<int> result;
    for (int i = 0; i < N * N; ++i)
        if (cells[i] == 0) result.Append(i);
    return result;
}

void Board::print() const {
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            char ch = '.';
            if (cells[r * N + c] == 1) ch = 'X';
            else if (cells[r * N + c] == 2) ch = 'O';
            std::cout << ch << ' ';
        }
        std::cout << '\n';
    }
}

int Board::evaluate(int player) const {
    int score = 0;

    auto countLine = [&](int r1, int c1, int dr, int dc) {
        int xc = 0, oc = 0;
        for (int k = 0; k < K; ++k) {
            int v = cells[(r1 + dr * k) * N + (c1 + dc * k)];
            if (v == 1) ++xc;
            else if (v == 2) ++oc;
        }
        if (oc == 0) score += xc;
        if (xc == 0) score -= oc;
    };

    // Горизонтали
    for (int r = 0; r < N; ++r)
        for (int c = 0; c + K <= N; ++c) countLine(r, c, 0, 1);
    // Вертикали
    for (int r = 0; r + K <= N; ++r)
        for (int c = 0; c < N; ++c) countLine(r, c, 1, 0);
    // Диагонали ↘
    for (int r = 0; r + K <= N; ++r)
        for (int c = 0; c + K <= N; ++c) countLine(r, c, 1, 1);
    // Диагонали ↙
    for (int r = 0; r + K <= N; ++r)
        for (int c = K - 1; c < N; ++c) countLine(r, c, 1, -1);

    return (player == 1) ? score : -score;
}