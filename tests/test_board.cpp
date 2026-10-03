#include <gtest/gtest.h>
#include "core/Board.hpp"
#include "core/Exceptions.hpp"

// ============================================================
//  Конструктор и пустое поле
// ============================================================

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
    EXPECT_EQ(b.whoseMove(), 1);   // 1 = крестики
    EXPECT_EQ(b.countMoves(), 0);
}

// ============================================================
//  whoseMove — чередование игроков
// ============================================================

TEST(BoardWhoseMove, AlternatesBetweenPlayers) {
    Board b;
    EXPECT_EQ(b.whoseMove(), 1);   // X
    b.makeMove(0, 0);
    EXPECT_EQ(b.whoseMove(), 2);   // O
    b.makeMove(1, 0);
    EXPECT_EQ(b.whoseMove(), 1);   // X
    b.makeMove(0, 1);
    EXPECT_EQ(b.whoseMove(), 2);   // O
}

// ============================================================
//  makeMove / undoMove
// ============================================================

TEST(BoardMakeMove, PlacesCorrectSymbol) {
    Board b;
    EXPECT_TRUE(b.makeMove(0, 0));
    EXPECT_EQ(b.getCell(0, 0), 1);   // X
    EXPECT_TRUE(b.makeMove(1, 1));
    EXPECT_EQ(b.getCell(1, 1), 2);   // O
}

TEST(BoardMakeMove, ReturnsFalseOnTakenCell) {
    Board b;
    EXPECT_TRUE(b.makeMove(0, 0));
    EXPECT_FALSE(b.makeMove(0, 0));   // занято
    EXPECT_EQ(b.countMoves(), 1);
}

TEST(BoardUndo, RestoresEmptyCell) {
    Board b;
    b.makeMove(1, 1);
    EXPECT_TRUE(b.undoMove(1, 1));
    EXPECT_EQ(b.getCell(1, 1), 0);
    EXPECT_EQ(b.countMoves(), 0);
    EXPECT_EQ(b.whoseMove(), 1);   // снова X
}

TEST(BoardUndo, ReturnsFalseOnEmptyCell) {
    Board b;
    EXPECT_FALSE(b.undoMove(0, 0));
}

TEST(BoardUndo, SequenceOfMovesAndUndos) {
    Board b;
    b.makeMove(0, 0);   // X
    b.makeMove(1, 1);   // O
    b.makeMove(2, 2);   // X
    EXPECT_EQ(b.countMoves(), 3);

    b.undoMove(2, 2);
    b.undoMove(1, 1);
    EXPECT_EQ(b.countMoves(), 1);
    EXPECT_EQ(b.getCell(0, 0), 1);
    EXPECT_EQ(b.getCell(1, 1), 0);
    EXPECT_EQ(b.getCell(2, 2), 0);
}

// ============================================================
//  isCellTaken
// ============================================================

TEST(BoardCellTaken, EmptyVsTaken) {
    Board b;
    EXPECT_FALSE(b.isCellTaken(0, 0));
    b.makeMove(0, 0);
    EXPECT_TRUE(b.isCellTaken(0, 0));
}

// ============================================================
//  isWin — все 8 линий (для X)
// ============================================================

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
    b.makeMove(0,0);   // X
    b.makeMove(1,0);   // O
    b.makeMove(0,1);   // X
    b.makeMove(1,1);   // O
    b.makeMove(2,2);   // X
    b.makeMove(1,2);   // O — победа
    EXPECT_TRUE(b.isWin(2));
    EXPECT_FALSE(b.isWin(1));
}

// ============================================================
//  isDraw
// ============================================================

TEST(BoardDraw, FullBoardWithoutWinnerIsDraw) {
    Board b;
    b.makeMove(0,0);   // X
    b.makeMove(0,1);   // O
    b.makeMove(0,2);   // X
    b.makeMove(1,1);   // O
    b.makeMove(1,0);   // X
    b.makeMove(1,2);   // O
    b.makeMove(2,1);   // X
    b.makeMove(2,0);   // O
    b.makeMove(2,2);   // X

    EXPECT_FALSE(b.isWin(1));
    EXPECT_FALSE(b.isWin(2));
    EXPECT_TRUE(b.isDraw());
    EXPECT_TRUE(b.isGameOver());
}

TEST(BoardDraw, WinOnLastMoveIsNotDraw) {
    Board b;
    b.makeMove(0,0);   // X
    b.makeMove(1,0);   // O
    b.makeMove(0,1);   // X
    b.makeMove(1,1);   // O
    b.makeMove(0,2);   // X
    b.makeMove(2,0);   // O
    b.makeMove(1,2);   // X
    b.makeMove(2,2);   // O
    b.makeMove(2,1);   // X — последний ход

    EXPECT_TRUE(b.isWin(1));
    EXPECT_FALSE(b.isDraw());     // ← ловит баг с isDraw
    EXPECT_TRUE(b.isGameOver());
}

// ============================================================
//  Исключения (CheckIndex)
// ============================================================

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