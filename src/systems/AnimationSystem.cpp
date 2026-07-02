#include "AnimationSystem.hpp"

#include <algorithm>

namespace bingo::systems {

namespace {

float easeOutCubic(float t) {
    const float inv = 1.0f - t;
    return 1.0f - inv * inv * inv;
}

float lerp(float start, float end, float t) {
    return start + (end - start) * t;
}

}

void AnimationSystem::spawn(data::BallAnimationPool& pool, const domain::DrawResult& result,
                             const infra::LayoutConfig& layout, const infra::AnimationConfig& animation) const {
    for (std::size_t i = 0; i < data::kBallCount; ++i) {
        pool.numbers[i] = result.numbers[i];
        pool.startX[i] = layout.balls.startX;
        pool.targetX[i] = layout.balls.targetStartX + layout.balls.spacing * static_cast<float>(i);
        pool.x[i] = pool.startX[i];
        pool.y[i] = layout.balls.y;
        pool.delay[i] = animation.delayBetweenBallsSeconds * static_cast<float>(i);
        pool.duration[i] = animation.durationSeconds;
        pool.elapsed[i] = 0.0f;
        pool.active[i] = true;
        pool.completed[i] = false;
    }
}

void AnimationSystem::update(data::BallAnimationPool& pool, float dtSeconds) const {
    for (std::size_t i = 0; i < data::kBallCount; ++i) {
        if (!pool.active[i] || pool.completed[i]) {
            continue;
        }

        pool.elapsed[i] += dtSeconds;
        if (pool.elapsed[i] < pool.delay[i]) {
            continue;
        }

        const float localTime = pool.elapsed[i] - pool.delay[i];
        const float t = std::clamp(localTime / pool.duration[i], 0.0f, 1.0f);
        pool.x[i] = lerp(pool.startX[i], pool.targetX[i], easeOutCubic(t));

        if (t >= 1.0f) {
            // fix the exact target to avoid accu float point
            pool.x[i] = pool.targetX[i];
            pool.completed[i] = true;
        }
    }
}

}
