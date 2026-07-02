#include "LuaAutomationService.hpp"

#include "ScriptEvents.hpp"

#include <sol/sol.hpp>

#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

namespace bingo::scripting {

namespace {

template <typename... Args>
void callIfPresent(sol::state& lua, const char* functionName, Args&&... args) {
    sol::protected_function fn = lua[functionName];
    if (!fn.valid()) {
        return;
    }

    const sol::protected_function_result result = fn(std::forward<Args>(args)...);
    if (!result.valid()) {
        const sol::error error = result;
        std::cerr << "LuaAutomationService: '" << functionName << "' failed: " << error.what() << '\n';
    }
}

}

LuaAutomationService::LuaAutomationService(const std::filesystem::path& scriptPath)
    : lua_(std::make_unique<sol::state>()) {
    lua_->open_libraries(sol::lib::base, sol::lib::table, sol::lib::string, sol::lib::math);

    const sol::protected_function_result result =
        lua_->safe_script_file(scriptPath.string(), sol::script_pass_on_error);
    if (!result.valid()) {
        const sol::error error = result;
        throw std::runtime_error(
            "LuaAutomationService: failed to load '" + scriptPath.string() + "': " + error.what()
        );
    }
}

LuaAutomationService::~LuaAutomationService() = default;

void LuaAutomationService::onAppStarted() {
    callIfPresent(*lua_, events::kOnAppStarted);
}

void LuaAutomationService::onDrawStarted() {
    callIfPresent(*lua_, events::kOnDrawStarted);
}

void LuaAutomationService::onNumbersDrawn(const std::array<int, 4>& numbers) {
    callIfPresent(*lua_, events::kOnNumbersDrawn, std::vector<int>(numbers.begin(), numbers.end()));
}

void LuaAutomationService::onBallAnimationStarted(int index, int number) {
    callIfPresent(*lua_, events::kOnBallAnimationStarted, index, number);
}

void LuaAutomationService::onBallLanded(int index, int number) {
    callIfPresent(*lua_, events::kOnBallLanded, index, number);
}

void LuaAutomationService::onDrawCompleted(const std::array<int, 4>& numbers) {
    callIfPresent(*lua_, events::kOnDrawCompleted, std::vector<int>(numbers.begin(), numbers.end()));
}

}
