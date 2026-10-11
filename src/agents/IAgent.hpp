#pragma once
#include "core/Board.hpp"

class IAgent {
public:
    virtual ~IAgent() = default;
    virtual int chooseMove(const Board& board) const = 0;
};