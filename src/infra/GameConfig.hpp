#pragma once

#include <array>
#include <string>

namespace bingo::infra {

struct WindowConfig {
    std::string title;
    unsigned int width = 0;
    unsigned int height = 0;
    unsigned int virtualWidth = 0;
    unsigned int virtualHeight = 0;
    unsigned int framerateLimit = 60;
    bool verticalSync = true;
    bool resizable = true;
};

// The received assets are a single spritesheet (atlasImage) described by a
// Leshy SpriteSheet Tool JSON (atlasData). ballFrames[number] and
// buttonPlayFrame hold the frame names inside that atlas, so real asset
// names never get hardcoded in C++ (see 07_JSON_CONFIG_SPEC.md).
struct AssetConfig {
    std::string atlasImage;
    std::string atlasData;
    std::string font;
    std::array<std::string, 10> ballFrames{};
    std::string buttonPlayFrame;
};

struct ButtonLayout {
    float x = 0.0f;
    float y = 0.0f;
    float scale = 1.0f;
};

struct BallsLayout {
    float startX = 0.0f;
    float y = 0.0f;
    float targetStartX = 0.0f;
    float spacing = 0.0f;
    float scale = 1.0f;
};

struct HistoryLayout {
    float x = 0.0f;
    float y = 0.0f;
    float lineHeight = 0.0f;
    float fontSize = 0.0f;
};

struct LayoutConfig {
    ButtonLayout button;
    BallsLayout balls;
    HistoryLayout history;
};

struct AnimationConfig {
    float durationSeconds = 0.0f;
    float delayBetweenBallsSeconds = 0.0f;
    std::string easing;
};

struct HistoryConfig {
    int maxVisible = 0;
};

struct LoggingConfig {
    std::string file;
};

struct LuaConfig {
    bool enabled = false;
    std::string entryScript;
};

struct GameConfig {
    WindowConfig window;
    AssetConfig assets;
    LayoutConfig layout;
    AnimationConfig animation;
    HistoryConfig history;
    LoggingConfig logging;
    LuaConfig lua;
};

}
