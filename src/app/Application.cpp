#include "Application.hpp"

#include "scripting/NoOpScriptingService.hpp"
#ifdef BINGO_ENABLE_LUA
#include "scripting/LuaAutomationService.hpp"
#endif

#include <SFML/Window/Event.hpp>

#include <algorithm>
#include <iostream>
#include <optional>
#include <sstream>

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

ui::PlayButton createPlayButton(const infra::GameConfig& config, const render::AssetManager& assetManager) {
    return ui::PlayButton(
        assetManager.atlasTexture(),
        assetManager.buttonPlayFrameRect(),
        sf::Vector2f(config.layout.button.x, config.layout.button.y),
        config.layout.button.scale
    );
}

std::array<sf::Sprite, data::kBallCount> createBallSprites(const render::SpriteFactory& spriteFactory,
                                                             float scale) {
    std::array<sf::Sprite, data::kBallCount> sprites{
        spriteFactory.createBallSprite(0), spriteFactory.createBallSprite(0), spriteFactory.createBallSprite(0),
        spriteFactory.createBallSprite(0)};

    for (sf::Sprite& sprite : sprites) {
        sprite.setScale({scale, scale});
    }

    return sprites;
}

render::HistoryPanel createHistoryPanel(const infra::GameConfig& config, const render::AssetManager& assetManager) {
    return render::HistoryPanel(
        assetManager.font(),
        sf::Vector2f(config.layout.history.x, config.layout.history.y),
        config.layout.history.lineHeight,
        static_cast<unsigned int>(config.layout.history.fontSize),
        static_cast<std::size_t>(config.history.maxVisible)
    );
}

std::string formatHistoryLine(const data::HistoryEntry& entry) {
    std::ostringstream stream;
    stream << entry.timestamp << " - " << entry.result.numbers[0] << " - " << entry.result.numbers[1] << " - "
           << entry.result.numbers[2] << " - " << entry.result.numbers[3];
    return stream.str();
}

// lua.enabled=false case
std::unique_ptr<scripting::IScriptingService> createScriptingService(const infra::GameConfig& config) {
    if (!config.lua.enabled) {
        return std::make_unique<scripting::NoOpScriptingService>();
    }

#ifdef BINGO_ENABLE_LUA
    try {
        return std::make_unique<scripting::LuaAutomationService>(config.lua.entryScript);
    } catch (const std::exception& error) {
        std::cerr << "Lua scripting disabled: " << error.what() << '\n';
        return std::make_unique<scripting::NoOpScriptingService>();
    }
#else
    std::cerr << "Lua scripting requested in config, but this build was compiled with BINGO_ENABLE_LUA=OFF\n";
    return std::make_unique<scripting::NoOpScriptingService>();
#endif
}

}

Application::Application(const infra::GameConfig& config)
    : config_(config),
      window_(createWindow(config.window)),
      viewportAdapter_(config.window.virtualWidth, config.window.virtualHeight),
      assetManager_(config),
      spriteFactory_(assetManager_),
      playButton_(createPlayButton(config, assetManager_)),
      historyPanel_(createHistoryPanel(config, assetManager_)),
      drawService_(randomEngine_),
      scripting_(createScriptingService(config)),
      gameCoordinator_(drawService_, *scripting_),
      ballSprites_(createBallSprites(spriteFactory_, config.layout.balls.scale)),
      logService_(config.logging.file),
      historyBuffer_(static_cast<std::size_t>(config.history.maxVisible)) {
    window_.setView(viewportAdapter_.computeView(window_.getSize()));
    scripting_->onAppStarted();
}

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
            continue;
        }

        if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            window_.setView(viewportAdapter_.computeView(resized->size));
            continue;
        }

        if (inputSystem_.handleEvent(*event, window_, window_.getView(), playButton_)) {
            gameCoordinator_.onPlayButtonClicked();
            playButton_.setEnabled(gameCoordinator_.isButtonEnabled());

            if (gameCoordinator_.state() == GameState::Animating) {
                const auto& result = gameCoordinator_.lastResult();
                animationSystem_.spawn(ballPool_, result, config_.layout, config_.animation);

                for (std::size_t i = 0; i < data::kBallCount; ++i) {
                    ballSprites_[i].setTextureRect(assetManager_.ballFrameRect(ballPool_.numbers[i]));
                    ballSprites_[i].setPosition({ballPool_.x[i], ballPool_.y[i]});
                    scripting_->onBallAnimationStarted(static_cast<int>(i), ballPool_.numbers[i]);
                }

                recordDraw(result);
            }
        }
    }
}

void Application::update(float dtSeconds) {
    if (gameCoordinator_.state() != GameState::Animating) {
        return;
    }

    const std::array<bool, data::kBallCount> previouslyCompleted = ballPool_.completed;

    animationSystem_.update(ballPool_, dtSeconds);

    for (std::size_t i = 0; i < data::kBallCount; ++i) {
        ballSprites_[i].setPosition({ballPool_.x[i], ballPool_.y[i]});

        if (ballPool_.completed[i] && !previouslyCompleted[i]) {
            scripting_->onBallLanded(static_cast<int>(i), ballPool_.numbers[i]);
        }
    }

    if (ballPool_.allCompleted()) {
        scripting_->onDrawCompleted(gameCoordinator_.lastResult().numbers);
        gameCoordinator_.onAnimationCompleted();
        playButton_.setEnabled(gameCoordinator_.isButtonEnabled());
    }
}

void Application::recordDraw(const domain::DrawResult& result) {
    logService_.logDraw(result);
    historyBuffer_.push(data::HistoryEntry{result, timeService_.nowTime()});

    std::vector<std::string> lines;
    lines.reserve(historyBuffer_.items().size());
    for (const data::HistoryEntry& entry : historyBuffer_.items()) {
        lines.push_back(formatHistoryLine(entry));
    }

    historyPanel_.setLines(lines);
}

void Application::render() {
    renderSystem_.render(window_, historyPanel_, ballSprites_, ballPool_, playButton_);
}

}
