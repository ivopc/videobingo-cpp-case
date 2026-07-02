#include "domain/DrawResult.hpp"
#include "infra/LogService.hpp"

#include <cassert>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>

using bingo::domain::DrawResult;
using bingo::infra::LogService;

namespace {

std::filesystem::path makeScratchLogPath() {
    return std::filesystem::temp_directory_path() / "bingo_log_service_tests.log";
}

std::string readWholeFile(const std::filesystem::path& path) {
    std::ifstream stream(path);
    std::ostringstream contents;
    contents << stream.rdbuf();
    return contents.str();
}

void testLogDrawWritesExpectedLineFormat() {
    const std::filesystem::path path = makeScratchLogPath();
    std::filesystem::remove(path);

    {
        LogService logService(path);
        logService.logDraw(DrawResult{{3, 7, 0, 9}});
    }

    const std::string contents = readWholeFile(path);
    const std::regex expected(R"(\[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\] Sorteio: 3 - 7 - 0 - 9\n)");
    assert(std::regex_search(contents, expected) && "log line must match '[date time] Sorteio: n - n - n - n'");

    std::filesystem::remove(path);
}

void testLogDrawAppendsAcrossInstances() {
    const std::filesystem::path path = makeScratchLogPath();
    std::filesystem::remove(path);

    {
        LogService first(path);
        first.logDraw(DrawResult{{1, 2, 3, 4}});
    }
    {
        LogService second(path);
        second.logDraw(DrawResult{{5, 6, 7, 8}});
    }

    const std::string contents = readWholeFile(path);
    const auto firstLinePos = contents.find("1 - 2 - 3 - 4");
    const auto secondLinePos = contents.find("5 - 6 - 7 - 8");

    assert(firstLinePos != std::string::npos && "first round must still be present");
    assert(secondLinePos != std::string::npos && "second round must be appended, not overwriting the first");
    assert(firstLinePos < secondLinePos);

    std::filesystem::remove(path);
}

void testConstructorCreatesFileIfMissing() {
    const std::filesystem::path path = makeScratchLogPath();
    std::filesystem::remove(path);
    assert(!std::filesystem::exists(path));

    { LogService logService(path); }

    assert(std::filesystem::exists(path) && "LogService must create the file if it doesn't exist yet");
    std::filesystem::remove(path);
}

}

int main() {
    testConstructorCreatesFileIfMissing();
    testLogDrawWritesExpectedLineFormat();
    testLogDrawAppendsAcrossInstances();

    std::printf("LogServiceTests: all tests passed\n");
    return 0;
}
