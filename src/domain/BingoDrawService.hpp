#pragma once

#include "DrawResult.hpp"
#include "RandomEngine.hpp"

namespace bingo::domain {

// Pure domain rule: no SFML, no assets, no window. Fully unit-testable.
class BingoDrawService {
public:
    explicit BingoDrawService(RandomEngine& randomEngine);

    DrawResult draw();

private:
    RandomEngine& randomEngine_;
};

}
