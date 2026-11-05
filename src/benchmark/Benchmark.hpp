#pragma once
#include "agents/IAgent.hpp"
#include "core/Board.hpp"
#include <string>
#include <fstream>


class Benchmark {
public:
    explicit Benchmark(const std::string& csvPath);
    ~Benchmark();

    void Run(IAgent& agent, const std::string& algoName,
             const std::string& scenarioName, const Board& board);

private:
    std::string path;
    std::ofstream file;
    bool headerWritten;

    void writeHeader();
};