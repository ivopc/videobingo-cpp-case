#pragma once

#include "IScriptingService.hpp"

namespace bingo::scripting {

// lua.enabled=false
class NoOpScriptingService final : public IScriptingService {
public:
    void onAppStarted() override {}
    void onDrawStarted() override {}
    void onNumbersDrawn(const std::array<int, 4>&) override {}
    void onBallAnimationStarted(int, int) override {}
    void onBallLanded(int, int) override {}
    void onDrawCompleted(const std::array<int, 4>&) override {}
};

}
