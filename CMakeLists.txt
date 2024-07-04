cmake_minimum_required(VERSION 3.10)

project(PWR_Checkers)

set(CMAKE_CXX_STANDARD 17)

# Path to SFML
set(SFML_DIR "C:/SFML/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

include_directories(include)

add_executable(PWR_Checkers
        src/main.cpp
        src/Board.cpp
        src/Game.cpp
        src/MinMaxAlgorithm.cpp
)

target_link_libraries(PWR_Checkers sfml-system sfml-window sfml-graphics)
