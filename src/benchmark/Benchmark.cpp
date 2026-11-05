#include "benchmark/Benchmark.hpp"
#include "Exceptions.hpp"
#include <chrono>

Benchmark::Benchmark(const std::string& csvPath)
    : path(csvPath), headerWritten(false) {
    file.open(path);
    if (!file.is_open()) {
        throw InvalidArgument("Cannot open " + path);
    }
}

Benchmark::~Benchmark() {
    if (file.is_open()) file.close();
}

void Benchmark::writeHeader() {
    file << "algorithm,scenario,time_ms\n";
    headerWritten = true;
}

void Benchmark::Run(IAgent& agent, const std::string& algoName,
                    const std::string& scenarioName, const Board& board) {
    if (!headerWritten) writeHeader();

    auto t0 = std::chrono::high_resolution_clock::now();
    int move = agent.chooseMove(board);
    auto t1 = std::chrono::high_resolution_clock::now();

    double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();

    file << algoName << "," << scenarioName << "," << ms << "\n";
    file.flush();
}