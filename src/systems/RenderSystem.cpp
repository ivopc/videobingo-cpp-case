#include "RenderSystem.hpp"

namespace bingo::systems {

void RenderSystem::render(sf::RenderWindow& window, const render::HistoryPanel& historyPanel,
                           const std::array<sf::Sprite, data::kBallCount>& ballSprites,
                           const data::BallAnimationPool& ballPool, const ui::PlayButton& playButton) const {
    window.clear(sf::Color(20, 20, 24));

    historyPanel.draw(window);

    for (std::size_t i = 0; i < data::kBallCount; ++i) {
        if (ballPool.active[i]) {
            window.draw(ballSprites[i]);
        }
    }

    playButton.draw(window);

    window.display();
}

}
