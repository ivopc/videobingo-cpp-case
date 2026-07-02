#pragma once

#include "UiState.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf {
class RenderTarget;
}

namespace bingo::ui {

class PlayButton {
public:
    PlayButton(const sf::Texture& atlasTexture, sf::IntRect frameRect, sf::Vector2f centerPosition, float baseScale);

    void setEnabled(bool enabled);
    bool isEnabled() const;

    void handleMouseMoved(sf::Vector2f worldMouse);
    bool handleMousePressed(sf::Vector2f worldMouse);
    bool handleMouseReleased(sf::Vector2f worldMouse);

    void draw(sf::RenderTarget& target) const;

private:
    bool contains(sf::Vector2f worldMouse) const;
    void applyVisualState();

    sf::Sprite sprite_;
    UiState state_ = UiState::Normal;
    bool enabled_ = true;
    bool pressedInside_ = false;
    float baseScale_ = 1.0f;
};

}
