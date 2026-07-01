#include "BingoDrawService.hpp"

#include <algorithm>
#include <array>

namespace bingo::domain {

BingoDrawService::BingoDrawService(RandomEngine& randomEngine) : randomEngine_(randomEngine) {}

DrawResult BingoDrawService::draw() {
    std::array<int, 10> numbers{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::shuffle(numbers.begin(), numbers.end(), randomEngine_.engine());

    DrawResult result;
    std::copy_n(numbers.begin(), result.numbers.size(), result.numbers.begin());
    return result;
}

}
