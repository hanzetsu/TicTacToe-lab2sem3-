#include <gtest/gtest.h>
#include "core/Board.hpp"

TEST(BoardConstructor, NewBoardIsEmpty) {
    Board b;
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            EXPECT_EQ(b.getCell(r, c), 0);
}

TEST(BoardConstructor, EmptyBoardIsNotGameOver) {
    Board b;
    EXPECT_FALSE(b.isWin(1));
    EXPECT_FALSE(b.isWin(2));
    EXPECT_FALSE(b.isDraw());
    EXPECT_FALSE(b.isGameOver());
}

TEST(BoardConstructor, XMovesFirst) {
    Board b;
    EXPECT_EQ(b.whoseMove(), 1);
    EXPECT_EQ(b.countMoves(), 0);
}

TEST(BoardWhoseMove, AlternatesBetweenPlayers) {
    Board b;
    EXPECT_EQ(b.whoseMove(), 1);
    b.makeMove(0, 0);
    EXPECT_EQ(b.whoseMove(), 2);
    b.makeMove(1, 0);
    EXPECT_EQ(b.whoseMove(), 1);
    b.makeMove(0, 1);
    EXPECT_EQ(b.whoseMove(), 2);
}

TEST(BoardMakeMove, PlacesCorrectSymbol) {
    Board b;
    EXPECT_TRUE(b.makeMove(0, 0));
    EXPECT_EQ(b.getCell(0, 0), 1);
    EXPECT_TRUE(b.makeMove(1, 1));
    EXPECT_EQ(b.getCell(1, 1), 2);
}

TEST(BoardMakeMove, ReturnsFalseOnTakenCell) {
    Board b;
    EXPECT_TRUE(b.makeMove(0, 0));
    EXPECT_FALSE(b.makeMove(0, 0));
    EXPECT_EQ(b.countMoves(), 1);
}

TEST(BoardUndo, RestoresEmptyCell) {
    Board b;
    b.makeMove(1, 1);
    EXPECT_TRUE(b.undoMove(1, 1));
    EXPECT_EQ(b.getCell(1, 1), 0);
    EXPECT_EQ(b.countMoves(), 0);
    EXPECT_EQ(b.whoseMove(), 1);
}

TEST(BoardUndo, ReturnsFalseOnEmptyCell) {
    Board b;
    EXPECT_FALSE(b.undoMove(0, 0));
}

TEST(BoardUndo, SequenceOfMovesAndUndos) {
    Board b;
    b.makeMove(0, 0);
    b.makeMove(1, 1);
    b.makeMove(2, 2);
    EXPECT_EQ(b.countMoves(), 3);

    b.undoMove(2, 2);
    b.undoMove(1, 1);
    EXPECT_EQ(b.countMoves(), 1);
    EXPECT_EQ(b.getCell(0, 0), 1);
    EXPECT_EQ(b.getCell(1, 1), 0);
    EXPECT_EQ(b.getCell(2, 2), 0);
}

TEST(BoardCellTaken, EmptyVsTaken) {
    Board b;
    EXPECT_FALSE(b.isCellTaken(0, 0));
    b.makeMove(0, 0);
    EXPECT_TRUE(b.isCellTaken(0, 0));
}

TEST(BoardWin, TopRow) {
    Board b;
    b.makeMove(0,0); b.makeMove(1,0);
    b.makeMove(0,1); b.makeMove(1,1);
    b.makeMove(0,2);
    EXPECT_TRUE(b.isWin(1));
    EXPECT_FALSE(b.isWin(2));
}

TEST(BoardWin, MiddleRow) {
    Board b;
    b.makeMove(1,0); b.makeMove(0,0);
    b.makeMove(1,1); b.makeMove(0,1);
    b.makeMove(1,2);
    EXPECT_TRUE(b.isWin(1));
}

TEST(BoardWin, BottomRow) {
    Board b;
    b.makeMove(2,0); b.makeMove(0,0);
    b.makeMove(2,1); b.makeMove(0,1);
    b.makeMove(2,2);
    EXPECT_TRUE(b.isWin(1));
}

TEST(BoardWin, LeftColumn) {
    Board b;
    b.makeMove(0,0); b.makeMove(0,1);
    b.makeMove(1,0); b.makeMove(1,1);
    b.makeMove(2,0);
    EXPECT_TRUE(b.isWin(1));
}

TEST(BoardWin, MiddleColumn) {
    Board b;
    b.makeMove(0,1); b.makeMove(0,0);
    b.makeMove(1,1); b.makeMove(1,0);
    b.makeMove(2,1);
    EXPECT_TRUE(b.isWin(1));
}

TEST(BoardWin, RightColumn) {
    Board b;
    b.makeMove(0,2); b.makeMove(0,0);
    b.makeMove(1,2); b.makeMove(1,0);
    b.makeMove(2,2);
    EXPECT_TRUE(b.isWin(1));
}

TEST(BoardWin, MainDiagonal) {
    Board b;
    b.makeMove(0,0); b.makeMove(0,1);
    b.makeMove(1,1); b.makeMove(0,2);
    b.makeMove(2,2);
    EXPECT_TRUE(b.isWin(1));
}

TEST(BoardWin, AntiDiagonal) {
    Board b;
    b.makeMove(0,2); b.makeMove(0,0);
    b.makeMove(1,1); b.makeMove(0,1);
    b.makeMove(2,0);
    EXPECT_TRUE(b.isWin(1));
}

TEST(BoardWin, OCanWinToo) {
    Board b;
    b.makeMove(0,0);
    b.makeMove(1,0);
    b.makeMove(0,1);
    b.makeMove(1,1);
    b.makeMove(2,2);
    b.makeMove(1,2);
    EXPECT_TRUE(b.isWin(2));
    EXPECT_FALSE(b.isWin(1));
}

TEST(BoardDraw, FullBoardWithoutWinnerIsDraw) {
    Board b;
    b.makeMove(0,0);
    b.makeMove(0,1);
    b.makeMove(0,2);
    b.makeMove(1,1);
    b.makeMove(1,0);
    b.makeMove(1,2);
    b.makeMove(2,1);
    b.makeMove(2,0);
    b.makeMove(2,2);

    EXPECT_FALSE(b.isWin(1));
    EXPECT_FALSE(b.isWin(2));
    EXPECT_TRUE(b.isDraw());
    EXPECT_TRUE(b.isGameOver());
}

TEST(BoardDraw, WinOnLastMoveIsNotDraw) {
    Board b;
    b.makeMove(0,0);
    b.makeMove(1,0);
    b.makeMove(0,1);
    b.makeMove(1,1);
    b.makeMove(0,2);
    b.makeMove(2,0);
    b.makeMove(1,2);
    b.makeMove(2,2);
    b.makeMove(2,1);

    EXPECT_TRUE(b.isWin(1));
    EXPECT_FALSE(b.isDraw());
    EXPECT_TRUE(b.isGameOver());
}

TEST(BoardExceptions, NegativeIndexThrows) {
    Board b;
    EXPECT_THROW(b.getCell(-1, 0), IndexOutOfRange);
    EXPECT_THROW(b.getCell(0, -1), IndexOutOfRange);
    EXPECT_THROW(b.makeMove(-1, 0), IndexOutOfRange);
}

TEST(BoardExceptions, LargeIndexThrows) {
    Board b;
    EXPECT_THROW(b.getCell(3, 0), IndexOutOfRange);
    EXPECT_THROW(b.getCell(0, 3), IndexOutOfRange);
    EXPECT_THROW(b.makeMove(3, 3), IndexOutOfRange);
}

TEST(BoardEmptyCells, EmptyBoardHas9Cells) {
    Board b;
    auto cells = b.getEmptyCells();
    EXPECT_EQ(cells.GetLength(), 9);
}

TEST(BoardState, EmptyBoardIsZero) {
    Board b;
    EXPECT_EQ(b.getState(), 0u);
}

TEST(BoardState, DifferentPositionsDifferentStates) {
    Board a, b;
    a.makeMove(0, 0);       // X
    b.makeMove(0, 1);       // X в другой клетке
    EXPECT_NE(a.getState(), b.getState());
}
