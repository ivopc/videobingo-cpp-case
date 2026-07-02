#pragma once

#include "IScriptingService.hpp"

#include <filesystem>
#include <memory>

namespace sol {
class state;
}

namespace bingo::scripting {
    
class LuaAutomationService final : public IScriptingService {
public:
    explicit LuaAutomationService(const std::filesystem::path& scriptPath);
    ~LuaAutomationService() override;

    void onAppStarted() override;
    void onDrawStarted() override;
    void onNumbersDrawn(const std::array<int, 4>& numbers) override;
    void onBallAnimationStarted(int index, int number) override;
    void onBallLanded(int index, int number) override;
    void onDrawCompleted(const std::array<int, 4>& numbers) override;

private:
    std::unique_ptr<sol::state> lua_;
};

}
