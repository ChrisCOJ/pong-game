cmake_minimum_required(VERSION 3.30)
project(pong_game_JB)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(pong_game_JB src/main.cpp
        src/Ball.cpp
        include/Ball.h
        src/Paddle.cpp
        include/Paddle.h
        src/Game.cpp
        include/Game.h
        src/Button.cpp
        include/Button.h)

target_link_libraries(pong_game_JB PRIVATE SFML::Graphics)
