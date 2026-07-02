#pragma once

#include "GameCoordinator.hpp"
#include "data/BallAnimationPool.hpp"
#include "data/HistoryBuffer.hpp"
#include "domain/BingoDrawService.hpp"
#include "domain/DrawResult.hpp"
#include "domain/RandomEngine.hpp"
#include "infra/GameConfig.hpp"
#include "infra/LogService.hpp"
#include "infra/TimeService.hpp"
#include "render/AssetManager.hpp"
#include "render/HistoryPanel.hpp"
#include "render/SpriteFactory.hpp"
#include "render/ViewportAdapter.hpp"
#include "scripting/IScriptingService.hpp"
#include "systems/AnimationSystem.hpp"
#include "systems/InputSystem.hpp"
#include "systems/RenderSystem.hpp"
#include "ui/PlayButton.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <array>
#include <memory>

namespace bingo::app {

class Application {
public:
    explicit Application(const infra::GameConfig& config);

    void run();

private:
    void processEvents();
    void update(float dtSeconds);
    void render();
    void recordDraw(const domain::DrawResult& result);

    const infra::GameConfig& config_;
    sf::RenderWindow window_;
    render::ViewportAdapter viewportAdapter_;
    render::AssetManager assetManager_;
    render::SpriteFactory spriteFactory_;
    ui::PlayButton playButton_;
    render::HistoryPanel historyPanel_;
    systems::InputSystem inputSystem_;
    systems::RenderSystem renderSystem_;

    domain::RandomEngine randomEngine_;
    domain::BingoDrawService drawService_;
    std::unique_ptr<scripting::IScriptingService> scripting_;
    GameCoordinator gameCoordinator_;

    data::BallAnimationPool ballPool_;
    systems::AnimationSystem animationSystem_;
    std::array<sf::Sprite, data::kBallCount> ballSprites_;

    infra::TimeService timeService_;
    infra::LogService logService_;
    data::HistoryBuffer historyBuffer_;
};

}
