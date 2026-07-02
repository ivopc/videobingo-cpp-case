#include "ViewportAdapter.hpp"

namespace bingo::render {

ViewportAdapter::ViewportAdapter(unsigned int virtualWidth, unsigned int virtualHeight)
    : virtualSize_(static_cast<float>(virtualWidth), static_cast<float>(virtualHeight)) {}

sf::View ViewportAdapter::computeView(sf::Vector2u windowSize) const {
    sf::View view(sf::FloatRect({0.0f, 0.0f}, virtualSize_));

    const float windowAspect = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
    const float virtualAspect = virtualSize_.x / virtualSize_.y;

    float viewportWidth = 1.0f;
    float viewportHeight = 1.0f;
    float viewportX = 0.0f;
    float viewportY = 0.0f;

    if (windowAspect > virtualAspect) {
        viewportWidth = virtualAspect / windowAspect;
        viewportX = (1.0f - viewportWidth) / 2.0f;
    } else {
        viewportHeight = windowAspect / virtualAspect;
        viewportY = (1.0f - viewportHeight) / 2.0f;
    }

    view.setViewport(sf::FloatRect({viewportX, viewportY}, {viewportWidth, viewportHeight}));
    return view;
}

}
