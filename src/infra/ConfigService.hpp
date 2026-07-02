#pragma once

#include "GameConfig.hpp"

#include <filesystem>

namespace bingo::infra {

class ConfigService {
public:
    static GameConfig load(const std::filesystem::path& configPath);
};

}
