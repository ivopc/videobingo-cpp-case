#pragma once

#include "infra/GameConfig.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>
#include <filesystem>
#include <string>
#include <unordered_map>

namespace bingo::render {

class AssetManager {
public:
    explicit AssetManager(const infra::GameConfig& config);

    const sf::Texture& atlasTexture() const;
    const sf::Font& font() const;

    sf::IntRect frameRect(const std::string& frameName) const;
    sf::IntRect ballFrameRect(int number) const;
    sf::IntRect buttonPlayFrameRect() const;

private:
    void loadAtlasFrames(const std::filesystem::path& atlasDataPath);

    sf::Texture atlasTexture_;
    sf::Font font_;
    std::unordered_map<std::string, sf::IntRect> frames_;
    std::array<std::string, 10> ballFrameNames_;
    std::string buttonPlayFrameName_;
};

}
