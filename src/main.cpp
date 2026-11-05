#include <iostream>
#include <string>
#include "core/Board.hpp"
#include "agents/MinimaxAgent.hpp"
#include "agents/AlphaBetaAgent.hpp"
#include "benchmark/Benchmark.hpp"

void runBenchmark() {
    Benchmark bm("results.csv");

    // 3×3, полная глубина
    Board b3(3, 3);
    MinimaxAgent  mm9(9);
    AlphaBetaAgent ab9(9);
    AlphaBetaAgent ab9c(9);
    ab9c.setCacheEnabled(true);

    std::cout << "Running 3x3 Minimax..." << std::endl;
    bm.Run(mm9,  "minimax",         "3x3_d9", b3);
    
    std::cout << "Running 3x3 Alpha-Beta..." << std::endl;
    bm.Run(ab9,  "alphabeta",       "3x3_d9", b3);
    
    std::cout << "Running 3x3 Alpha-Beta + Cache..." << std::endl;
    bm.Run(ab9c, "alphabeta+cache", "3x3_d9", b3);

    // 4×4, глубины 4 и 5
    Board b4(4, 3);
    MinimaxAgent  mm4a(4),  mm4b(5);
    AlphaBetaAgent ab4a(4), ab4b(5);
    AlphaBetaAgent ab4ca(4), ab4cb(5);
    ab4ca.setCacheEnabled(true);
    ab4cb.setCacheEnabled(true);

    std::cout << "Running 4x4 Minimax d4..." << std::endl;
    bm.Run(mm4a,  "minimax",         "4x4_d4", b4);
    
    std::cout << "Running 4x4 Minimax d5..." << std::endl;
    bm.Run(mm4b,  "minimax",         "4x4_d5", b4);
    
    std::cout << "Running 4x4 Alpha-Beta d4..." << std::endl;
    bm.Run(ab4a,  "alphabeta",       "4x4_d4", b4);
    
    std::cout << "Running 4x4 Alpha-Beta d5..." << std::endl;
    bm.Run(ab4b,  "alphabeta",       "4x4_d5", b4);
    
    std::cout << "Running 4x4 Alpha-Beta + Cache d4..." << std::endl;
    bm.Run(ab4ca, "alphabeta+cache", "4x4_d4", b4);
    
    std::cout << "Running 4x4 Alpha-Beta + Cache d5..." << std::endl;
    bm.Run(ab4cb, "alphabeta+cache", "4x4_d5", b4);

    std::cout << "Written to results.csv\n";
}

int main(int argc, char** argv) {
    if (argc > 1 && std::string(argv[1]) == "bench") {
        runBenchmark();
        return 0;
    }
    std::cout << "GUI target: gui\n";
    std::cout << "Benchmark:  ./console bench\n";
    return 0;
}