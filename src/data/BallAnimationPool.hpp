#pragma once

#include <array>
#include <cstddef>

namespace bingo::data {

constexpr std::size_t kBallCount = 4;

struct BallAnimationPool {
    std::array<int, kBallCount> numbers{};
    std::array<float, kBallCount> x{};
    std::array<float, kBallCount> y{};
    std::array<float, kBallCount> startX{};
    std::array<float, kBallCount> targetX{};
    std::array<float, kBallCount> elapsed{};
    std::array<float, kBallCount> duration{};
    std::array<float, kBallCount> delay{};
    std::array<bool, kBallCount> active{};
    std::array<bool, kBallCount> completed{};

    bool allCompleted() const;
};

}
