#pragma once

#include <random>

namespace bingo::domain {

class RandomEngine {
public:
    RandomEngine();

    std::mt19937_64& engine();

private:
    std::mt19937_64 engine_;
};

}
