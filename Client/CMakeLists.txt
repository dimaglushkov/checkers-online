cmake_minimum_required(VERSION 3.13)
project(Client C)

set(CMAKE_C_STANDARD 11)

add_executable(Client main.c)
target_link_libraries(Client -lSDL2 -Wall)