#pragma once

#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>

namespace bingo::render {

class ViewportAdapter {
public:
    ViewportAdapter(unsigned int virtualWidth, unsigned int virtualHeight);

    sf::View computeView(sf::Vector2u windowSize) const;

private:
    sf::Vector2f virtualSize_;
};

}
