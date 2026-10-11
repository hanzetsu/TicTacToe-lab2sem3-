#include "Board.hpp"
void Board::CheckIndex(int r, int c) const
{
    if (r >= SIZE_OF_BOARD || r < 0)
    {
        throw IndexOutOfRange(r, SIZE_OF_BOARD, "Вы вышли за границы поля");
    }
    if (c >= SIZE_OF_BOARD || c < 0)
    {
        throw IndexOutOfRange(c, SIZE_OF_BOARD, "Вы вышли за границы поля");
    }
}
bool Board::isCellTaken(int r, int c) const
{
    CheckIndex(r, c);
    return board[r][c] != 0;
}
int Board::getCell(int row, int col) const
{
    CheckIndex(row, col);
    return board[row][col];
}
int Board::countMoves() const
{ // функция подсчета ходов, первыми всегда начинают крестики
    int count = 0;
    for (int i = 0; i < SIZE_OF_BOARD; i++)
    {
        for (int j = 0; j < SIZE_OF_BOARD; j++)
        {
            if (board[i][j] != 0)
                count += 1;
        }
    }
    return count;
}
int Board::whoseMove() const
{
    return (countMoves() % 2) + 1; // 1 - крестики; 2 - нолики;
}
bool Board::makeMove(int row, int col)
{
    CheckIndex(row, col);
    if (isCellTaken(row, col))
    {
        return false;
    }
    if (whoseMove() == 1) // крестики
    {
        board[row][col] = 1;
    }
    else // нолики
    {
        board[row][col] = 2;
    }
    return true;
}

bool Board::undoMove(int row, int col)
{
    CheckIndex(row, col);
    if (!isCellTaken(row, col))
    {
        return false;
    }
    board[row][col] = 0;
    return true;
}

bool Board::isDraw() const
{
    if (isWin(1) || isWin(2)) return false;
    for (int i = 0; i < SIZE_OF_BOARD; i++)
    {
        for (int j = 0; j < SIZE_OF_BOARD; j++)
        {
            if (!isCellTaken(i, j))
            {
                return false;
            }
        }
    }
    return true;
}
bool Board::isWin(int player) const
{
    if (board[0][0] == player && board[0][1] == player && board[0][2] == player)
        return true;
    if (board[1][0] == player && board[1][1] == player && board[1][2] == player)
        return true;
    if (board[2][0] == player && board[2][1] == player && board[2][2] == player)
        return true;

    if (board[0][0] == player && board[1][0] == player && board[2][0] == player)
        return true;
    if (board[0][1] == player && board[1][1] == player && board[2][1] == player)
        return true;
    if (board[0][2] == player && board[1][2] == player && board[2][2] == player)
        return true;

    if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
        return true;
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player)
        return true;

    return false;
}
bool Board::isGameOver() const
{
    if (isWin(1) || isWin(2) || isDraw())
        return true;
    return false;
}

void Board::print() const {
    for (int r = 0; r < SIZE_OF_BOARD; ++r) {
        for (int c = 0; c < SIZE_OF_BOARD; ++c) {
            char ch = '.';
            if (board[r][c] == 1) ch = 'X';
            else if (board[r][c] == 2) ch = 'O';
            std::cout << ch << ' ';
        }
        std::cout << '\n';
    }
}

MutableArraySequence<int> Board::getEmptyCells() const {
    MutableArraySequence<int>  result;
    for (int i = 0; i < SIZE_OF_BOARD; i++) {
        for (int j = 0; j < SIZE_OF_BOARD; j++) {
            if(!isCellTaken(i,j)) {
                result.Append(i * SIZE_OF_BOARD + j);
            }
        }
    }
    return result;
}

void Board::makeMove(int index) {
    makeMove(index / SIZE_OF_BOARD, index % SIZE_OF_BOARD);
}
void Board::undoMove(int index) {
    undoMove(index / SIZE_OF_BOARD, index % SIZE_OF_BOARD);
}
