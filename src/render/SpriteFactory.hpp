#pragma once

#include "AssetManager.hpp"

#include <SFML/Graphics/Sprite.hpp>

namespace bingo::render {
class SpriteFactory {
public:
    explicit SpriteFactory(const AssetManager& assetManager);

    sf::Sprite createBallSprite(int number) const;

private:
    const AssetManager& assetManager_;
};

}
