#include <gtest/gtest.h>
#include "core/Board.hpp"
#include "agents/MinimaxAgent.hpp"
#include "agents/AlphaBetaAgent.hpp"
#include "agents/CachedMinimaxAgent.hpp"
TEST(MinimaxAgent, TakesWinningMove)
{
    Board b;
    // X: 0,1,2   O: 3,4   → X ходит, должен выиграть в 2 (завершить верхнюю строку)
    b.makeMove(0, 0); // X
    b.makeMove(1, 0); // O
    b.makeMove(0, 1); // X
    b.makeMove(1, 1); // O

    MinimaxAgent agent;
    int move = agent.chooseMove(b);
    EXPECT_EQ(move, 2); // X должен завершить верхнюю горизонталь
}

TEST(MinimaxAgent, BlocksOpponentWin)
{
    Board b;
    // X: 0,1   O: 3,4   → X ходит, должен заблокировать O на 5
    b.makeMove(0, 0); // X
    b.makeMove(1, 0); // O
    b.makeMove(0, 1); // X
    b.makeMove(1, 1); // O

    MinimaxAgent agent;
    int move = agent.chooseMove(b);
    EXPECT_EQ(move, 2);
}

TEST(MinimaxAgent, DoesNotLoseFromEmpty)
{
    Board b;
    MinimaxAgent agent;
    // Играем партию: X = агент, O = агент
    while (!b.isGameOver())
    {
        int move = agent.chooseMove(b);
        b.makeMove(move);
    }
    // На пустом поле при идеальной игре обоих — ничья
    EXPECT_FALSE(b.isWin(1));
    EXPECT_FALSE(b.isWin(2));
    EXPECT_TRUE(b.isDraw());
}

TEST(AlphaBetaAgent, TakesWinningMove)
{
    Board b;
    b.makeMove(0, 0);
    b.makeMove(1, 0);
    b.makeMove(0, 1);
    b.makeMove(1, 1);

    AlphaBetaAgent agent;
    int move = agent.chooseMove(b);
    EXPECT_EQ(move, 2);
}

TEST(AlphaBetaAgent, BlocksOpponentWin)
{
    Board b;
    b.makeMove(0, 0);
    b.makeMove(2, 2);
    b.makeMove(0, 1);

    AlphaBetaAgent agent;
    int move = agent.chooseMove(b);
    EXPECT_EQ(move, 2);
}

TEST(AlphaBetaAgent, DoesNotLoseFromEmpty)
{
    Board b;
    AlphaBetaAgent agent;
    while (!b.isGameOver())
    {
        int move = agent.chooseMove(b);
        b.makeMove(move);
    }
    EXPECT_FALSE(b.isWin(1));
    EXPECT_FALSE(b.isWin(2));
    EXPECT_TRUE(b.isDraw());
}

TEST(AlphaBetaAgent, SameBestMoveAsMinimax)
{
    Board b;
    MinimaxAgent minimax;
    AlphaBetaAgent alphabeta;

    int m1 = minimax.chooseMove(b);
    int m2 = alphabeta.chooseMove(b);
    EXPECT_TRUE(m1 == m2);
}

TEST(CachedMinimaxAgent, TakesWinningMove)
{
    Board b;
    b.makeMove(0, 0);
    b.makeMove(1, 0);
    b.makeMove(0, 1);
    b.makeMove(1, 1);

    CachedMinimaxAgent agent;
    int move = agent.chooseMove(b);
    EXPECT_EQ(move, 2);
}
TEST(CachedMinimaxAgent, BlocksOpponentWin) {
    Board b;
    b.makeMove(0,0);   // X
    b.makeMove(1,0);   // O
    b.makeMove(2,2);   // X
    b.makeMove(1,1);   // O  ← O угрожает выиграть через (1,2)
    // X не может выиграть сейчас, но должен заблокировать (1,2)

    CachedMinimaxAgent agent;
    int move = agent.chooseMove(b);
    EXPECT_EQ(move, 5);
}
TEST(CachedMinimaxAgent, DoesNotLoseFromEmpty)
{
    Board b;
    CachedMinimaxAgent agent;
    while (!b.isGameOver())
    {
        int move = agent.chooseMove(b);
        b.makeMove(move);
    }
    EXPECT_FALSE(b.isWin(1));
    EXPECT_FALSE(b.isWin(2));
    EXPECT_TRUE(b.isDraw());
}

TEST(CachedMinimaxAgent, SameBestMoveAsMinimax)
{
    Board b;
    MinimaxAgent minimax;
    CachedMinimaxAgent cached;
    EXPECT_EQ(minimax.chooseMove(b), cached.chooseMove(b));
}