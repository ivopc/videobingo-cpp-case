#include "LogService.hpp"

#include <stdexcept>

namespace bingo::infra {

LogService::LogService(const std::filesystem::path& filePath) : file_(filePath, std::ios::app) {
    if (!file_.is_open()) {
        throw std::runtime_error("LogService: unable to open log file: " + filePath.string());
    }
}

void LogService::logDraw(const domain::DrawResult& result) {
    file_ << "[" << timeService_.nowDateTime() << "] Sorteio: " << result.numbers[0] << " - " << result.numbers[1]
          << " - " << result.numbers[2] << " - " << result.numbers[3] << '\n';
    file_.flush();
}

}
