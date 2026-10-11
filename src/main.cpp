#include <iostream>
#include <string>
#include "core/Board.hpp"
#include "agents/MinimaxAgent.hpp"
#include "agents/AlphaBetaAgent.hpp"
#include "agents/CachedMinimaxAgent.hpp"
#include "benchmark/Benchmark.hpp"

void runBenchmark() {
    Benchmark bm("results.csv");

    // 3×3, полная глубина
    Board b3(3, 3);
    MinimaxAgent       mm9(9);
    AlphaBetaAgent     ab9(9);
    CachedMinimaxAgent cm9(9);

    bm.Run(mm9, "minimax",  "3x3_empty", b3);
    bm.Run(ab9, "alphabeta","3x3_empty", b3);
    bm.Run(cm9, "cached",   "3x3_empty", b3);

    // 4×4, ограниченная глубина
    Board b4(4, 3);
MinimaxAgent       mm4(4);
AlphaBetaAgent     ab4(4);
CachedMinimaxAgent cm4(4);

    bm.Run(mm4, "minimax",  "4x4_d5", b4);
    bm.Run(ab4, "alphabeta","4x4_d5", b4);
    bm.Run(cm4, "cached",   "4x4_d5", b4);

    std::cout << "Written to results.csv\n";
}

int main(int argc, char** argv) {
    if (argc > 1 && std::string(argv[1]) == "bench") {
        runBenchmark();
        return 0;
    }
    // ... обычная игра ...
}