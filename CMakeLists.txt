cmake_minimum_required(VERSION 3.21)
project(BingoSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(nlohmann_json 3.11 QUIET CONFIG)
if(NOT nlohmann_json_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(json)
endif()

set(SFML_STATIC_LIBRARIES ON)
find_package(SFML 3 REQUIRED COMPONENTS System Window Graphics
    PATHS /usr/local/lib/cmake/SFML
    NO_DEFAULT_PATH
)

option(BINGO_ENABLE_LUA "Enable Lua scripting via Sol2" ON)

if(BINGO_ENABLE_LUA)
    find_package(Lua REQUIRED)

    include(FetchContent)
    FetchContent_Declare(
        sol2
        GIT_REPOSITORY https://github.com/ThePhD/sol2.git
        GIT_TAG v3.3.0
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(sol2)
endif()

set(BINGO_CORE_SOURCES
    src/app/Application.cpp
    src/app/GameCoordinator.cpp
    src/infra/ConfigService.cpp
    src/domain/RandomEngine.cpp
    src/domain/BingoDrawService.cpp
    src/render/AssetManager.cpp
    src/ui/PlayButton.cpp
    src/systems/InputSystem.cpp
    src/data/BallAnimationPool.cpp
    src/systems/AnimationSystem.cpp
    src/render/ViewportAdapter.cpp
    src/render/SpriteFactory.cpp
    src/render/HistoryPanel.cpp
    src/systems/RenderSystem.cpp
    src/infra/TimeService.cpp
    src/infra/LogService.cpp
    src/data/HistoryBuffer.cpp
)

if(BINGO_ENABLE_LUA)
    list(APPEND BINGO_CORE_SOURCES src/scripting/LuaAutomationService.cpp)
endif()

add_library(bingo_core STATIC ${BINGO_CORE_SOURCES})
target_include_directories(bingo_core PUBLIC src)
target_link_libraries(bingo_core PUBLIC SFML::System SFML::Window SFML::Graphics nlohmann_json::nlohmann_json)

if(BINGO_ENABLE_LUA)
    target_compile_definitions(bingo_core PUBLIC BINGO_ENABLE_LUA)
    target_include_directories(bingo_core PRIVATE ${LUA_INCLUDE_DIR})
    target_link_libraries(bingo_core PUBLIC sol2::sol2 ${LUA_LIBRARIES})
endif()

add_executable(BingoSimulator src/main.cpp)
target_link_libraries(BingoSimulator PRIVATE bingo_core)

enable_testing()

add_executable(BingoDrawServiceTests tests/BingoDrawServiceTests.cpp)
target_link_libraries(BingoDrawServiceTests PRIVATE bingo_core)
add_test(NAME BingoDrawServiceTests COMMAND BingoDrawServiceTests)

add_executable(PlayButtonTests tests/PlayButtonTests.cpp)
target_link_libraries(PlayButtonTests PRIVATE bingo_core)
add_test(NAME PlayButtonTests COMMAND PlayButtonTests)

add_executable(GameCoordinatorTests tests/GameCoordinatorTests.cpp)
target_link_libraries(GameCoordinatorTests PRIVATE bingo_core)
add_test(NAME GameCoordinatorTests COMMAND GameCoordinatorTests)

add_executable(AnimationSystemTests tests/AnimationSystemTests.cpp)
target_link_libraries(AnimationSystemTests PRIVATE bingo_core)
add_test(NAME AnimationSystemTests COMMAND AnimationSystemTests)

add_executable(ViewportAdapterTests tests/ViewportAdapterTests.cpp)
target_link_libraries(ViewportAdapterTests PRIVATE bingo_core)
add_test(NAME ViewportAdapterTests COMMAND ViewportAdapterTests)

add_executable(HistoryBufferTests tests/HistoryBufferTests.cpp)
target_link_libraries(HistoryBufferTests PRIVATE bingo_core)
add_test(NAME HistoryBufferTests COMMAND HistoryBufferTests)

add_executable(LogServiceTests tests/LogServiceTests.cpp)
target_link_libraries(LogServiceTests PRIVATE bingo_core)
add_test(NAME LogServiceTests COMMAND LogServiceTests)
