#include "ui/PlayButton.hpp"

#include <SFML/Graphics/Texture.hpp>

#include <cassert>
#include <cstdio>
#include <cstdlib>

using bingo::ui::PlayButton;

namespace {

sf::Texture makeDummyTexture() {
    sf::Texture texture;
    if (!texture.resize({100u, 50u})) {
        std::fprintf(stderr, "failed to create dummy texture\n");
        std::exit(1);
    }
    return texture;
}

void testClickRequiresPressAndReleaseInsideBounds() {
    const sf::Texture texture = makeDummyTexture();
    PlayButton button(texture, sf::IntRect({0, 0}, {100, 50}), sf::Vector2f(200.f, 200.f), 1.0f);

    const sf::Vector2f inside(200.f, 200.f);
    const sf::Vector2f outside(2000.f, 2000.f);

    assert(button.handleMousePressed(inside) && "press inside must hit the button");
    assert(button.handleMouseReleased(inside) && "press+release inside must count as a click");

    assert(button.handleMousePressed(inside) && "press inside must hit the button again");
    assert(!button.handleMouseReleased(outside) && "release outside must not count as a click");

    assert(!button.handleMousePressed(outside) && "press outside must not hit the button");
}

void testDisabledButtonIgnoresClicks() {
    const sf::Texture texture = makeDummyTexture();
    PlayButton button(texture, sf::IntRect({0, 0}, {100, 50}), sf::Vector2f(200.f, 200.f), 1.0f);

    button.setEnabled(false);
    assert(!button.isEnabled());

    const sf::Vector2f inside(200.f, 200.f);
    assert(!button.handleMousePressed(inside) && "disabled button must ignore press");
    assert(!button.handleMouseReleased(inside) && "disabled button must ignore release");

    button.setEnabled(true);
    assert(button.isEnabled());
    assert(button.handleMousePressed(inside) && "re-enabled button must accept press again");
    assert(button.handleMouseReleased(inside) && "re-enabled button must register a click");
}

}

int main() {
    testClickRequiresPressAndReleaseInsideBounds();
    testDisabledButtonIgnoresClicks();
    std::printf("PlayButtonTests: all tests passed\n");
    return 0;
}
