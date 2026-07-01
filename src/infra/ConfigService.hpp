#pragma once

#include "GameConfig.hpp"

#include <filesystem>

namespace bingo::infra {

// Reads and validates config/game.json. Throws std::runtime_error with a
// clear, human-readable message on any missing file/field or invalid value —
// acceptable for a small technical-test app (see 14_CODING_STANDARDS.md).
class ConfigService {
public:
    static GameConfig load(const std::filesystem::path& configPath);
};

}
