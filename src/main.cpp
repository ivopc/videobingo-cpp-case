#include "app/Application.hpp"
#include "infra/ConfigService.hpp"

#include <cstdlib>
#include <iostream>

int main() {
    try {
        const auto config = bingo::infra::ConfigService::load("config/game.json");

        bingo::app::Application application(config);
        application.run();

        return EXIT_SUCCESS;
    } catch (const std::exception& error) {
        std::cerr << "Fatal error: " << error.what() << '\n';
        return EXIT_FAILURE;
    }
}
