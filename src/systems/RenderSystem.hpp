#pragma once

#include "data/BallAnimationPool.hpp"
#include "render/HistoryPanel.hpp"
#include "ui/PlayButton.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <array>

namespace bingo::systems {
    
class RenderSystem {
public:
    void render(sf::RenderWindow& window, const render::HistoryPanel& historyPanel,
                const std::array<sf::Sprite, data::kBallCount>& ballSprites, const data::BallAnimationPool& ballPool,
                const ui::PlayButton& playButton) const;
};

}
