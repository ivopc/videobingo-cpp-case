#include "TimeService.hpp"

#include <ctime>
#include <iomanip>
#include <sstream>

namespace bingo::infra {

namespace {

std::tm currentLocalTime() {
    const std::time_t now = std::time(nullptr);
    // single thread use std::localtime
    return *std::localtime(&now);
}

std::string format(const char* pattern) {
    const std::tm timeInfo = currentLocalTime();
    std::ostringstream stream;
    stream << std::put_time(&timeInfo, pattern);
    return stream.str();
}

}

std::string TimeService::nowDateTime() const {
    return format("%Y-%m-%d %H:%M:%S");
}

std::string TimeService::nowTime() const {
    return format("%H:%M:%S");
}

}
