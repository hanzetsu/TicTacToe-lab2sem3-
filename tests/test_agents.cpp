#include <gtest/gtest.h>
#include "core/Board.hpp"
#include "agents/MinimaxAgent.hpp"
#include "agents/AlphaBetaAgent.hpp"

TEST(MinimaxAgent, TakesWinningMove) {
    Board b;
    b.makeMove(0, 0); b.makeMove(1, 0);
    b.makeMove(0, 1); b.makeMove(1, 1);

    MinimaxAgent agent;
    EXPECT_EQ(agent.chooseMove(b), 2);
}

TEST(MinimaxAgent, DoesNotLoseFromEmpty) {
    Board b;
    MinimaxAgent agent;
    while (!b.isGameOver()) {
        int move = agent.chooseMove(b);
        b.makeMove(move);
    }
    EXPECT_TRUE(b.isDraw());
}

TEST(AlphaBetaAgent, TakesWinningMove) {
    Board b;
    b.makeMove(0, 0); b.makeMove(1, 0);
    b.makeMove(0, 1); b.makeMove(1, 1);

    AlphaBetaAgent agent;
    EXPECT_EQ(agent.chooseMove(b), 2);
}

TEST(AlphaBetaAgent, BlocksOpponentWin) {
    Board b;
    b.makeMove(0, 0);
    b.makeMove(2, 2);
    b.makeMove(0, 1);

    AlphaBetaAgent agent;
    EXPECT_EQ(agent.chooseMove(b), 2);
}

TEST(AlphaBetaAgent, DoesNotLoseFromEmpty) {
    Board b;
    AlphaBetaAgent agent;
    while (!b.isGameOver()) {
        int move = agent.chooseMove(b);
        b.makeMove(move);
    }
    EXPECT_TRUE(b.isDraw());
}

TEST(AlphaBetaAgent, CacheDoesNotChangeResult) {
    Board b;
    b.makeMove(0, 0);
    b.makeMove(1, 0);
    b.makeMove(0, 1);

    AlphaBetaAgent plain(9);
    AlphaBetaAgent cached(9);
    cached.setCacheEnabled(true);

    EXPECT_EQ(plain.chooseMove(b), cached.chooseMove(b));
}

TEST(AlphaBetaAgent, CacheFlagToggles) {
    AlphaBetaAgent ab;
    EXPECT_FALSE(ab.isCacheEnabled());
    ab.setCacheEnabled(true);
    EXPECT_TRUE(ab.isCacheEnabled());
    ab.setCacheEnabled(false);
    EXPECT_FALSE(ab.isCacheEnabled());
}