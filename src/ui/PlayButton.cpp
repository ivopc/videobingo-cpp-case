#include "PlayButton.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <cstdint>

namespace bingo::ui {

namespace {

constexpr float kHoverScaleFactor = 1.03f;
constexpr float kPressedScaleFactor = 0.97f;
constexpr std::uint8_t kNormalAlpha = 255;
constexpr std::uint8_t kPressedAlpha = 210;
constexpr std::uint8_t kDisabledAlpha = 120;

}

PlayButton::PlayButton(const sf::Texture& atlasTexture, sf::IntRect frameRect, sf::Vector2f centerPosition,
                        float baseScale)
    : sprite_(atlasTexture, frameRect), baseScale_(baseScale) {
    const sf::Vector2f frameSize(static_cast<float>(frameRect.size.x), static_cast<float>(frameRect.size.y));
    sprite_.setOrigin(frameSize / 2.0f);
    sprite_.setPosition(centerPosition);
    applyVisualState();
}

void PlayButton::setEnabled(bool enabled) {
    enabled_ = enabled;
    pressedInside_ = false;
    state_ = enabled ? UiState::Normal : UiState::Disabled;
    applyVisualState();
}

bool PlayButton::isEnabled() const {
    return enabled_;
}

void PlayButton::handleMouseMoved(sf::Vector2f worldMouse) {
    if (!enabled_ || pressedInside_) {
        return;
    }
    state_ = contains(worldMouse) ? UiState::Hover : UiState::Normal;
    applyVisualState();
}

bool PlayButton::handleMousePressed(sf::Vector2f worldMouse) {
    if (!enabled_ || !contains(worldMouse)) {
        return false;
    }
    pressedInside_ = true;
    state_ = UiState::Pressed;
    applyVisualState();
    return true;
}

bool PlayButton::handleMouseReleased(sf::Vector2f worldMouse) {
    const bool wasPressedInside = pressedInside_;
    pressedInside_ = false;

    if (!enabled_) {
        state_ = UiState::Disabled;
        applyVisualState();
        return false;
    }

    const bool stillInside = contains(worldMouse);
    state_ = stillInside ? UiState::Hover : UiState::Normal;
    applyVisualState();

    return wasPressedInside && stillInside;
}

void PlayButton::draw(sf::RenderTarget& target) const {
    target.draw(sprite_);
}

bool PlayButton::contains(sf::Vector2f worldMouse) const {
    return sprite_.getGlobalBounds().contains(worldMouse);
}

void PlayButton::applyVisualState() {
    float scale = baseScale_;
    std::uint8_t alpha = kNormalAlpha;

    switch (state_) {
        case UiState::Normal:
            break;
        case UiState::Hover:
            scale *= kHoverScaleFactor;
            break;
        case UiState::Pressed:
            scale *= kPressedScaleFactor;
            alpha = kPressedAlpha;
            break;
        case UiState::Disabled:
            alpha = kDisabledAlpha;
            break;
    }

    sprite_.setScale({scale, scale});

    sf::Color color = sprite_.getColor();
    color.a = alpha;
    sprite_.setColor(color);
}

}
