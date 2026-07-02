#include "GameCoordinator.hpp"

namespace bingo::app {

GameCoordinator::GameCoordinator(domain::BingoDrawService& drawService, scripting::IScriptingService& scripting)
    : drawService_(drawService), scripting_(scripting) {}

GameState GameCoordinator::state() const {
    return state_;
}

bool GameCoordinator::isButtonEnabled() const {
    return state_ == GameState::Idle || state_ == GameState::ResultShown;
}

const domain::DrawResult& GameCoordinator::lastResult() const {
    return lastResult_;
}

bool GameCoordinator::hasResult() const {
    return hasResult_;
}

void GameCoordinator::onPlayButtonClicked() {
    if (state_ != GameState::Idle && state_ != GameState::ResultShown) {
        return;
    }

    state_ = GameState::Drawing;

    scripting_.onDrawStarted();
    lastResult_ = drawService_.draw();
    hasResult_ = true;
    scripting_.onNumbersDrawn(lastResult_.numbers);

    state_ = GameState::Animating;
}

void GameCoordinator::onAnimationCompleted() {
    if (state_ != GameState::Animating) {
        return;
    }
    state_ = GameState::ResultShown;
}

}
