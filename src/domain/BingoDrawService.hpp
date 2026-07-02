#pragma once

#include "DrawResult.hpp"
#include "RandomEngine.hpp"

namespace bingo::domain {

class BingoDrawService {
public:
    explicit BingoDrawService(RandomEngine& randomEngine);

    DrawResult draw();

private:
    RandomEngine& randomEngine_;
};

}
