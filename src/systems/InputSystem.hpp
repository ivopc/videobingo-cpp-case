#pragma once

#include "ui/PlayButton.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>

namespace bingo::systems {
class InputSystem {
public:
    bool handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view,
                      ui::PlayButton& playButton) const;
};

}
