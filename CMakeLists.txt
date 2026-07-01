cmake_minimum_required(VERSION 3.21)
project(BingoSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

include(FetchContent)
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(json)

# Two SFML installs can coexist on this machine: an apt-provided SFML 2.6
# (shared, /usr) and a locally built SFML 3.2 (static-only, /usr/local).
# This project targets SFML 3, so pin the search to /usr/local and request
# its static libraries explicitly.
set(SFML_STATIC_LIBRARIES ON)
find_package(SFML 3 REQUIRED COMPONENTS System Window Graphics
    PATHS /usr/local/lib/cmake/SFML
    NO_DEFAULT_PATH
)

add_library(bingo_core STATIC
    src/app/Application.cpp
    src/infra/ConfigService.cpp
    src/domain/RandomEngine.cpp
    src/domain/BingoDrawService.cpp
)
target_include_directories(bingo_core PUBLIC src)
target_link_libraries(bingo_core PUBLIC SFML::System SFML::Window SFML::Graphics nlohmann_json::nlohmann_json)

add_executable(BingoSimulator src/main.cpp)
target_link_libraries(BingoSimulator PRIVATE bingo_core)

enable_testing()

add_executable(BingoDrawServiceTests tests/BingoDrawServiceTests.cpp)
target_link_libraries(BingoDrawServiceTests PRIVATE bingo_core)
add_test(NAME BingoDrawServiceTests COMMAND BingoDrawServiceTests)
