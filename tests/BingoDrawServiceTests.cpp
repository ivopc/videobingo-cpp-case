#include "domain/BingoDrawService.hpp"
#include "domain/RandomEngine.hpp"

#include <cassert>
#include <cstdio>
#include <set>

using bingo::domain::BingoDrawService;
using bingo::domain::RandomEngine;

namespace {

void testDrawReturnsFourDistinctNumbersInRange() {
    RandomEngine randomEngine;
    BingoDrawService service(randomEngine);

    for (int round = 0; round < 1000; ++round) {
        const auto result = service.draw();
        const std::set<int> unique(result.numbers.begin(), result.numbers.end());

        assert(unique.size() == 4 && "draw() must return 4 distinct numbers");
        for (const int number : result.numbers) {
            assert(number >= 0 && number <= 9 && "draw() must stay within [0, 9]");
        }
    }
}

}

int main() {
    testDrawReturnsFourDistinctNumbersInRange();
    std::printf("BingoDrawServiceTests: all tests passed\n");
    return 0;
}
