#pragma once

#include "TimeService.hpp"
#include "domain/DrawResult.hpp"

#include <filesystem>
#include <fstream>

namespace bingo::infra {

class LogService {
public:
    explicit LogService(const std::filesystem::path& filePath);

    void logDraw(const domain::DrawResult& result);

private:
    TimeService timeService_;
    std::ofstream file_;
};

}
