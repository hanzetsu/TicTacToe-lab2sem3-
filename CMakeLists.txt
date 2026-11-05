cmake_minimum_required(VERSION 3.16)
project(TicTacToe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Включаем генерацию compile_commands.json для VS Code
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =========================================================
# 1. Старая лабораторная (структуры)
# =========================================================
file(GLOB_RECURSE OLD_LAB_SOURCES "old_lab/source/*.cpp")

add_library(core STATIC
    src/core/Board.cpp
    ${OLD_LAB_SOURCES}
)
target_include_directories(core PUBLIC 
    src 
    old_lab/include
    old_lab
)

# =========================================================
# 2. Агенты
# =========================================================
add_library(agents STATIC
    src/agents/MinimaxAgent.cpp
    src/agents/AlphaBetaAgent.cpp
)
target_link_libraries(agents PUBLIC core)
target_include_directories(agents PUBLIC src)

# =========================================================
# 3. Бенчмарк
# =========================================================
add_library(benchmark STATIC
    src/benchmark/Benchmark.cpp
)
target_link_libraries(benchmark PUBLIC agents)
target_include_directories(benchmark PUBLIC src)

add_executable(console src/main.cpp)
target_link_libraries(console PRIVATE agents benchmark)


find_package(Qt6 COMPONENTS Widgets QUIET)
if (NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
endif()

add_executable(gui
    src/gui/main_gui.cpp
    src/gui/MainWindow.cpp
    src/gui/MainWindow.hpp
)
if (Qt6_FOUND)
    target_link_libraries(gui PRIVATE agents Qt6::Widgets)
else()
    target_link_libraries(gui PRIVATE agents Qt5::Widgets)
endif()
target_include_directories(gui PRIVATE src)

enable_testing()

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(tests
    tests/test_board.cpp
    tests/test_agents.cpp
    tests/test_dictionary.cpp
)

target_link_libraries(tests PRIVATE core agents GTest::gtest_main)
target_include_directories(tests PRIVATE src old_lab/include)

include(GoogleTest)
gtest_discover_tests(tests)