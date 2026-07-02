#include "app/GameCoordinator.hpp"
#include "app/GameState.hpp"
#include "domain/BingoDrawService.hpp"
#include "domain/RandomEngine.hpp"
#include "scripting/NoOpScriptingService.hpp"

#include <cassert>
#include <cstdio>
#include <set>

using bingo::app::GameCoordinator;
using bingo::app::GameState;
using bingo::domain::BingoDrawService;
using bingo::domain::RandomEngine;
using bingo::scripting::NoOpScriptingService;

namespace {

void testStartsIdleAndButtonEnabled() {
    RandomEngine randomEngine;
    BingoDrawService drawService(randomEngine);
    NoOpScriptingService scripting;
    GameCoordinator coordinator(drawService, scripting);

    assert(coordinator.state() == GameState::Idle);
    assert(coordinator.isButtonEnabled());
    assert(!coordinator.hasResult());
}

void testClickStartsDrawAndEntersAnimating() {
    RandomEngine randomEngine;
    BingoDrawService drawService(randomEngine);
    NoOpScriptingService scripting;
    GameCoordinator coordinator(drawService, scripting);

    coordinator.onPlayButtonClicked();

    assert(coordinator.state() == GameState::Animating);
    assert(!coordinator.isButtonEnabled() && "button must be disabled during Animating (RF09)");
    assert(coordinator.hasResult());

    const std::set<int> unique(coordinator.lastResult().numbers.begin(), coordinator.lastResult().numbers.end());
    assert(unique.size() == 4 && "draw triggered by the click must yield 4 distinct numbers");
}

void testClickDuringAnimatingIsIgnored() {
    RandomEngine randomEngine;
    BingoDrawService drawService(randomEngine);
    NoOpScriptingService scripting;
    GameCoordinator coordinator(drawService, scripting);

    coordinator.onPlayButtonClicked();
    assert(coordinator.state() == GameState::Animating);

    coordinator.onPlayButtonClicked();
    assert(coordinator.state() == GameState::Animating && "a click while Animating must not restart the draw");
}

void testAnimationCompletedReturnsToResultShownAndReenablesButton() {
    RandomEngine randomEngine;
    BingoDrawService drawService(randomEngine);
    NoOpScriptingService scripting;
    GameCoordinator coordinator(drawService, scripting);

    coordinator.onPlayButtonClicked();
    coordinator.onAnimationCompleted();

    assert(coordinator.state() == GameState::ResultShown);
    assert(coordinator.isButtonEnabled() && "button must be enabled again once ResultShown (RF10)");
}

void testAnimationCompletedIsNoOpOutsideAnimating() {
    RandomEngine randomEngine;
    BingoDrawService drawService(randomEngine);
    NoOpScriptingService scripting;
    GameCoordinator coordinator(drawService, scripting);

    coordinator.onAnimationCompleted();
    assert(coordinator.state() == GameState::Idle && "onAnimationCompleted must be a no-op outside Animating");
}

void testResultShownAllowsANewDraw() {
    RandomEngine randomEngine;
    BingoDrawService drawService(randomEngine);
    NoOpScriptingService scripting;
    GameCoordinator coordinator(drawService, scripting);

    coordinator.onPlayButtonClicked();
    coordinator.onAnimationCompleted();
    assert(coordinator.state() == GameState::ResultShown);

    coordinator.onPlayButtonClicked();
    assert(coordinator.state() == GameState::Animating && "ResultShown + click must start a new draw");
}

}

int main() {
    testStartsIdleAndButtonEnabled();
    testClickStartsDrawAndEntersAnimating();
    testClickDuringAnimatingIsIgnored();
    testAnimationCompletedReturnsToResultShownAndReenablesButton();
    testAnimationCompletedIsNoOpOutsideAnimating();
    testResultShownAllowsANewDraw();

    std::printf("GameCoordinatorTests: all tests passed\n");
    return 0;
}
