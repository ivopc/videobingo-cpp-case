#pragma once

#include <array>

namespace bingo::domain {

struct DrawResult {
    std::array<int, 4> numbers{};
};

}
