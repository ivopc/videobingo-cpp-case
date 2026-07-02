#include "InputSystem.hpp"

#include <SFML/Window/Mouse.hpp>

namespace bingo::systems {

bool InputSystem::handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view,
                               ui::PlayButton& playButton) const {
    if (const auto* moved = event.getIf<sf::Event::MouseMoved>()) {
        playButton.handleMouseMoved(window.mapPixelToCoords(moved->position, view));
        return false;
    }

    if (const auto* pressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (pressed->button == sf::Mouse::Button::Left) {
            playButton.handleMousePressed(window.mapPixelToCoords(pressed->position, view));
        }
        return false;
    }

    if (const auto* released = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (released->button == sf::Mouse::Button::Left) {
            return playButton.handleMouseReleased(window.mapPixelToCoords(released->position, view));
        }
        return false;
    }

    return false;
}

}
