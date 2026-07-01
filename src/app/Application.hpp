#pragma once

#include "infra/GameConfig.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace bingo::app {

class Application {
public:
    explicit Application(const infra::GameConfig& config);

    void run();

private:
    void processEvents();
    void update(float dtSeconds);
    void render();

    const infra::GameConfig& config_;
    sf::RenderWindow window_;
};

}
