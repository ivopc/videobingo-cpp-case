#include "AssetManager.hpp"

#include <nlohmann/json.hpp>

#include <fstream>
#include <stdexcept>

namespace bingo::render {

namespace {

[[noreturn]] void fail(const std::string& message) {
    throw std::runtime_error("AssetManager: " + message);
}

}

AssetManager::AssetManager(const infra::GameConfig& config)
    : ballFrameNames_(config.assets.ballFrames), buttonPlayFrameName_(config.assets.buttonPlayFrame) {
    if (!atlasTexture_.loadFromFile(config.assets.atlasImage)) {
        fail("failed to load atlas texture: " + config.assets.atlasImage);
    }

    if (!font_.openFromFile(config.assets.font)) {
        fail("failed to load font: " + config.assets.font);
    }

    loadAtlasFrames(config.assets.atlasData);

    for (const std::string& name : ballFrameNames_) {
        frameRect(name);
    }
    frameRect(buttonPlayFrameName_);
}

void AssetManager::loadAtlasFrames(const std::filesystem::path& atlasDataPath) {
    std::ifstream stream(atlasDataPath);
    if (!stream.is_open()) {
        fail("unable to open atlas data file: " + atlasDataPath.string());
    }

    nlohmann::json root;
    try {
        stream >> root;
    } catch (const nlohmann::json::parse_error& error) {
        fail("failed to parse atlas data '" + atlasDataPath.string() + "': " + error.what());
    }

    if (!root.contains("frames")) {
        fail("atlas data is missing the 'frames' field: " + atlasDataPath.string());
    }

    for (const auto& [name, entry] : root.at("frames").items()) {
        const auto& frame = entry.at("frame");
        const int x = frame.at("x").get<int>();
        const int y = frame.at("y").get<int>();
        const int w = frame.at("w").get<int>();
        const int h = frame.at("h").get<int>();
        frames_.emplace(name, sf::IntRect({x, y}, {w, h}));
    }
}

const sf::Texture& AssetManager::atlasTexture() const {
    return atlasTexture_;
}

const sf::Font& AssetManager::font() const {
    return font_;
}

sf::IntRect AssetManager::frameRect(const std::string& frameName) const {
    const auto it = frames_.find(frameName);
    if (it == frames_.end()) {
        fail("frame not found in atlas: " + frameName);
    }
    return it->second;
}

sf::IntRect AssetManager::ballFrameRect(int number) const {
    if (number < 0 || static_cast<std::size_t>(number) >= ballFrameNames_.size()) {
        fail("ball number out of range: " + std::to_string(number));
    }
    return frameRect(ballFrameNames_[static_cast<std::size_t>(number)]);
}

sf::IntRect AssetManager::buttonPlayFrameRect() const {
    return frameRect(buttonPlayFrameName_);
}

}
