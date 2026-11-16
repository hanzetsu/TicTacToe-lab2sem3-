#include <gtest/gtest.h>
#include "core/Board.hpp"

TEST(BoardConstructor, NewBoardIsEmpty) {
    Board b;
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            EXPECT_EQ(b.getCell(r, c), 0);
}

TEST(BoardConstructor, EmptyBoardIsOngoing) {
    Board b;
    EXPECT_EQ(b.getGameState(), GameState::Ongoing);
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
    EXPECT_EQ(b.getGameState(), GameState::WinX);
}

TEST(BoardWin, MiddleRow) {
    Board b;
    b.makeMove(1,0); b.makeMove(0,0);
    b.makeMove(1,1); b.makeMove(0,1);
    b.makeMove(1,2);
    EXPECT_EQ(b.getGameState(), GameState::WinX);
}

TEST(BoardWin, BottomRow) {
    Board b;
    b.makeMove(2,0); b.makeMove(0,0);
    b.makeMove(2,1); b.makeMove(0,1);
    b.makeMove(2,2);
    EXPECT_EQ(b.getGameState(), GameState::WinX);
}

TEST(BoardWin, LeftColumn) {
    Board b;
    b.makeMove(0,0); b.makeMove(0,1);
    b.makeMove(1,0); b.makeMove(1,1);
    b.makeMove(2,0);
    EXPECT_EQ(b.getGameState(), GameState::WinX);
}

TEST(BoardWin, MiddleColumn) {
    Board b;
    b.makeMove(0,1); b.makeMove(0,0);
    b.makeMove(1,1); b.makeMove(1,0);
    b.makeMove(2,1);
    EXPECT_EQ(b.getGameState(), GameState::WinX);
}

TEST(BoardWin, RightColumn) {
    Board b;
    b.makeMove(0,2); b.makeMove(0,0);
    b.makeMove(1,2); b.makeMove(1,0);
    b.makeMove(2,2);
    EXPECT_EQ(b.getGameState(), GameState::WinX);
}

TEST(BoardWin, MainDiagonal) {
    Board b;
    b.makeMove(0,0); b.makeMove(0,1);
    b.makeMove(1,1); b.makeMove(0,2);
    b.makeMove(2,2);
    EXPECT_EQ(b.getGameState(), GameState::WinX);
}

TEST(BoardWin, AntiDiagonal) {
    Board b;
    b.makeMove(0,2); b.makeMove(0,0);
    b.makeMove(1,1); b.makeMove(0,1);
    b.makeMove(2,0);
    EXPECT_EQ(b.getGameState(), GameState::WinX);
}

TEST(BoardWin, OCanWinToo) {
    Board b;
    b.makeMove(0,0); b.makeMove(1,0);
    b.makeMove(0,1); b.makeMove(1,1);
    b.makeMove(2,2); b.makeMove(1,2);
    EXPECT_EQ(b.getGameState(), GameState::WinO);
}

TEST(BoardDraw, FullBoardWithoutWinnerIsDraw) {
    Board b;
    b.makeMove(0,0); b.makeMove(0,1);
    b.makeMove(0,2); b.makeMove(1,1);
    b.makeMove(1,0); b.makeMove(1,2);
    b.makeMove(2,1); b.makeMove(2,0);
    b.makeMove(2,2);

    EXPECT_EQ(b.getGameState(), GameState::Draw);
}

TEST(BoardDraw, WinOnLastMoveIsNotDraw) {
    Board b;
    b.makeMove(0,0); b.makeMove(1,0);
    b.makeMove(0,1); b.makeMove(1,1);
    b.makeMove(0,2); b.makeMove(2,0);
    b.makeMove(1,2); b.makeMove(2,2);
    b.makeMove(2,1);

    EXPECT_EQ(b.getGameState(), GameState::WinX);
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
    int emptyCount = 0;
    for (int i = 0; i < 9; ++i) {
        if (b.getCell(i) == 0) emptyCount++;
    }
    EXPECT_EQ(emptyCount, 9);
}

TEST(BoardState, EmptyBoardIsZero) {
    Board b;
    EXPECT_EQ(b.getState(), 0u);
}

TEST(BoardState, DifferentPositionsDifferentStates) {
    Board a, b;
    a.makeMove(0, 0); 
    b.makeMove(0, 1); 
    EXPECT_NE(a.getState(), b.getState());
}