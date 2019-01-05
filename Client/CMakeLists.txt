cmake_minimum_required(VERSION 3.13)
project(Client C)

set(CMAKE_C_STANDARD 11)
set(SOURCE_FILES src/main.c src/ui.c src/game_proc.c src/network.c)

add_executable(Client ${SOURCE_FILES})
target_link_libraries(Client -lSDL2 -Wall)