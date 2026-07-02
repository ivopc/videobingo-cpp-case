#include "data/BallAnimationPool.hpp"
#include "domain/DrawResult.hpp"
#include "infra/GameConfig.hpp"
#include "systems/AnimationSystem.hpp"

#include <cassert>
#include <cmath>
#include <cstdio>

using bingo::data::BallAnimationPool;
using bingo::data::kBallCount;
using bingo::domain::DrawResult;
using bingo::infra::AnimationConfig;
using bingo::infra::LayoutConfig;
using bingo::systems::AnimationSystem;

namespace {

bool nearlyEqual(float a, float b, float epsilon = 0.001f) {
    return std::fabs(a - b) < epsilon;
}

LayoutConfig makeLayout() {
    LayoutConfig layout;
    layout.balls.startX = -180.0f;
    layout.balls.y = 260.0f;
    layout.balls.targetStartX = 310.0f;
    layout.balls.spacing = 170.0f;
    layout.balls.scale = 0.45f;
    return layout;
}

AnimationConfig makeAnimation() {
    AnimationConfig animation;
    animation.durationSeconds = 0.85f;
    animation.delayBetweenBallsSeconds = 0.28f;
    animation.easing = "easeOutCubic";
    return animation;
}

void testSpawnSeedsSequentialTargetsAndDelays() {
    const AnimationSystem system;
    BallAnimationPool pool;
    const DrawResult result{{3, 7, 0, 9}};
    const LayoutConfig layout = makeLayout();
    const AnimationConfig animation = makeAnimation();

    system.spawn(pool, result, layout, animation);

    for (std::size_t i = 0; i < kBallCount; ++i) {
        assert(pool.numbers[i] == result.numbers[i]);
        assert(pool.active[i]);
        assert(!pool.completed[i]);
        assert(nearlyEqual(pool.x[i], layout.balls.startX));
        assert(nearlyEqual(pool.targetX[i], layout.balls.targetStartX + layout.balls.spacing * static_cast<float>(i)));
        assert(nearlyEqual(pool.delay[i], animation.delayBetweenBallsSeconds * static_cast<float>(i)));
        assert(nearlyEqual(pool.duration[i], animation.durationSeconds));
    }
}

void testUpdateRespectsPerBallDelay() {
    const AnimationSystem system;
    BallAnimationPool pool;
    const DrawResult result{{1, 2, 3, 4}};
    const LayoutConfig layout = makeLayout();
    const AnimationConfig animation = makeAnimation();

    system.spawn(pool, result, layout, animation);

    system.update(pool, 0.9f);

    assert(pool.completed[0] && "ball 0 has no delay, so 0.9s > duration must finish it");
    assert(nearlyEqual(pool.x[0], pool.targetX[0]));

    assert(!pool.completed[3] && "ball 3 starts at delay 0.84s, should barely have moved");
    assert(nearlyEqual(pool.x[3], pool.startX[3], 5.0f) && "ball 3 should still be near its start position");
}

void testUpdateEventuallyCompletesAllBalls() {
    const AnimationSystem system;
    BallAnimationPool pool;
    const DrawResult result{{0, 1, 2, 3}};
    const LayoutConfig layout = makeLayout();
    const AnimationConfig animation = makeAnimation();

    system.spawn(pool, result, layout, animation);

    const float totalDuration = animation.durationSeconds + animation.delayBetweenBallsSeconds * 3.0f;
    system.update(pool, totalDuration + 0.01f);

    assert(pool.allCompleted());
    for (std::size_t i = 0; i < kBallCount; ++i) {
        assert(nearlyEqual(pool.x[i], pool.targetX[i]) && "completed ball must sit exactly on targetX");
    }
}

void testUpdateIgnoresInactiveBalls() {
    const AnimationSystem system;
    BallAnimationPool pool;

    system.update(pool, 10.0f);

    assert(!pool.allCompleted());
    for (std::size_t i = 0; i < kBallCount; ++i) {
        assert(!pool.active[i]);
        assert(!pool.completed[i]);
    }
}

}

int main() {
    testSpawnSeedsSequentialTargetsAndDelays();
    testUpdateRespectsPerBallDelay();
    testUpdateEventuallyCompletesAllBalls();
    testUpdateIgnoresInactiveBalls();

    std::printf("AnimationSystemTests: all tests passed\n");
    return 0;
}
