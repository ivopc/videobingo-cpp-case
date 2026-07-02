#include "SpriteFactory.hpp"

namespace bingo::render {

SpriteFactory::SpriteFactory(const AssetManager& assetManager) : assetManager_(assetManager) {}

sf::Sprite SpriteFactory::createBallSprite(int number) const {
    return sf::Sprite(assetManager_.atlasTexture(), assetManager_.ballFrameRect(number));
}

}
