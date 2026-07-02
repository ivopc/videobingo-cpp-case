#pragma once

#include <array>

namespace bingo::scripting {

class IScriptingService {
public:
    virtual ~IScriptingService() = default;

    virtual void onAppStarted() = 0;
    virtual void onDrawStarted() = 0;
    virtual void onNumbersDrawn(const std::array<int, 4>& numbers) = 0;
    virtual void onBallAnimationStarted(int index, int number) = 0;
    virtual void onBallLanded(int index, int number) = 0;
    virtual void onDrawCompleted(const std::array<int, 4>& numbers) = 0;
};

}
