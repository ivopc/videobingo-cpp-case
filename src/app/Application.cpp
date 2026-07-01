#include "Application.hpp"

#include <SFML/Window/Event.hpp>

#include <optional>

namespace bingo::app {

namespace {

sf::RenderWindow createWindow(const infra::WindowConfig& config) {
    const std::uint32_t style = config.resizable
        ? sf::Style::Default
        : (sf::Style::Titlebar | sf::Style::Close);

    sf::RenderWindow window(sf::VideoMode({config.width, config.height}), config.title, style);
    window.setFramerateLimit(config.framerateLimit);
    window.setVerticalSyncEnabled(config.verticalSync);
    return window;
}

}

Application::Application(const infra::GameConfig& config)
    : config_(config), window_(createWindow(config.window)) {}

void Application::run() {
    sf::Clock clock;

    while (window_.isOpen()) {
        const float dtSeconds = clock.restart().asSeconds();

        processEvents();
        update(dtSeconds);
        render();
    }
}

void Application::processEvents() {
    while (const std::optional event = window_.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window_.close();
        }
    }
}

void Application::update(float /*dtSeconds*/) {
    // Input/animation/scripting systems are wired here in later phases.
}

void Application::render() {
    window_.clear(sf::Color(20, 20, 24));
    window_.display();
}

}
