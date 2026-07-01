#include "RandomEngine.hpp"

#include <chrono>

namespace bingo::domain {

namespace {

std::mt19937_64 makeSeededEngine() {
    std::random_device rd;
    std::seed_seq seed{
        rd(), rd(), rd(), rd(),
        static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count())
    };
    return std::mt19937_64(seed);
}

}

RandomEngine::RandomEngine() : engine_(makeSeededEngine()) {}

std::mt19937_64& RandomEngine::engine() {
    return engine_;
}

}
