#pragma once

#include <random>

namespace bingo::domain {

// Owns a well-seeded Mersenne Twister engine shared by domain services.
// Kept separate from BingoDrawService so the seeding strategy can be
// reused/tested independently of the draw rules.
class RandomEngine {
public:
    RandomEngine();

    std::mt19937_64& engine();

private:
    std::mt19937_64 engine_;
};

}
