#pragma once

#include "GameState.hpp"
#include "domain/BingoDrawService.hpp"
#include "domain/DrawResult.hpp"
#include "scripting/IScriptingService.hpp"

namespace bingo::app {


class GameCoordinator {
public:
    GameCoordinator(domain::BingoDrawService& drawService, scripting::IScriptingService& scripting);

    GameState state() const;
    bool isButtonEnabled() const;
    const domain::DrawResult& lastResult() const;
    bool hasResult() const;

    void onPlayButtonClicked();

    void onAnimationCompleted();

private:
    domain::BingoDrawService& drawService_;
    scripting::IScriptingService& scripting_;
    GameState state_ = GameState::Idle;
    domain::DrawResult lastResult_{};
    bool hasResult_ = false;
};

}
