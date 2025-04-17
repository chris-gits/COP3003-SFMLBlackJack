cmake_minimum_required(VERSION 3.29)
project(untitled1)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(untitled1
        main.cpp
        logic/cards.cpp
        logic/blackjack.cpp
        graphics/cardsprite.cpp
        graphics/cardsprite.h
        graphics/scoretext.cpp
        graphics/scoretext.h
        graphics/selectable.cpp
        graphics/selectable.h)
target_compile_features(untitled1 PRIVATE cxx_std_17)
target_link_libraries(untitled1 PRIVATE SFML::Graphics SFML::Audio)