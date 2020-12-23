cmake_minimum_required(VERSION 3.16)
project(minesweeper)

set(CMAKE_CXX_STANDARD 20)

set(EXECUTABLE_NAME minesweeper)
set(SOURCES main.cpp Tile.cpp Game.cpp Textures.cpp Button.cpp Counter.cpp)
set(HEADERS Tile.h Game.h Textures.h Button.h Counter.h)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -ggdb")

# Link SFML 2.5.1 to project
set(SFML_DIR "/home/michael/.local/lib/SFML2.5/lib/cmake/SFML") # Install path
find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)

add_executable(${EXECUTABLE_NAME} ${SOURCES} ${HEADERS})
target_link_libraries (${EXECUTABLE_NAME} sfml-graphics sfml-window sfml-system)
