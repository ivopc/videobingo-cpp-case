#include "ConfigService.hpp"

#include <nlohmann/json.hpp>

#include <fstream>
#include <stdexcept>

namespace bingo::infra {

namespace {

using nlohmann::json;

[[noreturn]] void fail(const std::string& message) {
    throw std::runtime_error("ConfigService: " + message);
}

const json& requireField(const json& node, const std::string& key, const std::string& context) {
    if (!node.contains(key)) {
        fail("missing required field '" + context + "." + key + "'");
    }
    return node.at(key);
}

void requireFileExists(const std::filesystem::path& path, const std::string& description) {
    if (!std::filesystem::exists(path)) {
        fail(description + " not found at path: " + path.string());
    }
}

WindowConfig parseWindow(const json& root) {
    const json& node = requireField(root, "window", "config");

    WindowConfig config;
    config.title = requireField(node, "title", "window").get<std::string>();
    config.width = requireField(node, "width", "window").get<unsigned int>();
    config.height = requireField(node, "height", "window").get<unsigned int>();
    config.virtualWidth = requireField(node, "virtualWidth", "window").get<unsigned int>();
    config.virtualHeight = requireField(node, "virtualHeight", "window").get<unsigned int>();
    config.framerateLimit = node.value("framerateLimit", 60u);
    config.verticalSync = node.value("verticalSync", true);
    config.resizable = node.value("resizable", true);

    if (config.width == 0 || config.height == 0 || config.virtualWidth == 0 || config.virtualHeight == 0) {
        fail("window dimensions must be greater than zero");
    }

    return config;
}

AssetConfig parseAssets(const json& root) {
    const json& node = requireField(root, "assets", "config");

    AssetConfig config;
    config.atlasImage = requireField(node, "atlasImage", "assets").get<std::string>();
    config.atlasData = requireField(node, "atlasData", "assets").get<std::string>();
    config.font = requireField(node, "font", "assets").get<std::string>();
    config.buttonPlayFrame = requireField(node, "buttonPlay", "assets").get<std::string>();

    const json& balls = requireField(node, "balls", "assets");
    if (!balls.is_array() || balls.size() != config.ballFrames.size()) {
        fail("assets.balls must be an array with exactly 10 entries (index = number)");
    }
    for (std::size_t i = 0; i < config.ballFrames.size(); ++i) {
        config.ballFrames[i] = balls.at(i).get<std::string>();
    }

    requireFileExists(config.atlasImage, "assets.atlasImage");
    requireFileExists(config.atlasData, "assets.atlasData");
    requireFileExists(config.font, "assets.font");

    return config;
}

LayoutConfig parseLayout(const json& root) {
    const json& node = requireField(root, "layout", "config");

    LayoutConfig config;

    const json& button = requireField(node, "button", "layout");
    config.button.x = button.value("x", 0.0f);
    config.button.y = button.value("y", 0.0f);
    config.button.scale = button.value("scale", 1.0f);

    const json& balls = requireField(node, "balls", "layout");
    config.balls.startX = balls.value("startX", 0.0f);
    config.balls.y = balls.value("y", 0.0f);
    config.balls.targetStartX = balls.value("targetStartX", 0.0f);
    config.balls.spacing = balls.value("spacing", 0.0f);
    config.balls.scale = balls.value("scale", 1.0f);

    if (config.balls.spacing <= 0.0f) {
        fail("layout.balls.spacing must be greater than zero");
    }

    if (node.contains("history")) {
        const json& history = node.at("history");
        config.history.x = history.value("x", 0.0f);
        config.history.y = history.value("y", 0.0f);
        config.history.lineHeight = history.value("lineHeight", 0.0f);
        config.history.fontSize = history.value("fontSize", 0.0f);
    }

    return config;
}

AnimationConfig parseAnimation(const json& root) {
    const json& node = requireField(root, "animation", "config");

    AnimationConfig config;
    config.durationSeconds = requireField(node, "durationSeconds", "animation").get<float>();
    config.delayBetweenBallsSeconds = requireField(node, "delayBetweenBallsSeconds", "animation").get<float>();
    config.easing = node.value("easing", std::string("linear"));

    if (config.durationSeconds <= 0.0f) {
        fail("animation.durationSeconds must be greater than zero");
    }
    if (config.delayBetweenBallsSeconds < 0.0f) {
        fail("animation.delayBetweenBallsSeconds must not be negative");
    }

    return config;
}

HistoryConfig parseHistory(const json& root) {
    const json& node = requireField(root, "history", "config");

    HistoryConfig config;
    config.maxVisible = requireField(node, "maxVisible", "history").get<int>();

    if (config.maxVisible <= 0) {
        fail("history.maxVisible must be greater than zero");
    }

    return config;
}

LoggingConfig parseLogging(const json& root) {
    const json& node = requireField(root, "logging", "config");

    LoggingConfig config;
    config.file = requireField(node, "file", "logging").get<std::string>();
    return config;
}

LuaConfig parseLua(const json& root) {
    LuaConfig config;
    if (!root.contains("lua")) {
        return config;
    }

    // Lua is an optional automation layer: a missing/absent script is
    // resolved later by falling back to NoOpScriptingService, not here.
    const json& node = root.at("lua");
    config.enabled = node.value("enabled", false);
    config.entryScript = node.value("entryScript", std::string());
    return config;
}

}

GameConfig ConfigService::load(const std::filesystem::path& configPath) {
    requireFileExists(configPath, "config file");

    std::ifstream stream(configPath);
    if (!stream.is_open()) {
        fail("unable to open config file: " + configPath.string());
    }

    json root;
    try {
        stream >> root;
    } catch (const json::parse_error& error) {
        fail("failed to parse '" + configPath.string() + "': " + error.what());
    }

    GameConfig config;
    config.window = parseWindow(root);
    config.assets = parseAssets(root);
    config.layout = parseLayout(root);
    config.animation = parseAnimation(root);
    config.history = parseHistory(root);
    config.logging = parseLogging(root);
    config.lua = parseLua(root);

    return config;
}

}
