#pragma once

#include "data/BallAnimationPool.hpp"
#include "domain/DrawResult.hpp"
#include "infra/GameConfig.hpp"

namespace bingo::systems {
class AnimationSystem {
public:
    void spawn(data::BallAnimationPool& pool, const domain::DrawResult& result, const infra::LayoutConfig& layout,
               const infra::AnimationConfig& animation) const;

    void update(data::BallAnimationPool& pool, float dtSeconds) const;
};

}
