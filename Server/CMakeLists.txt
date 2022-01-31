cmake_minimum_required(VERSION 3.13)
project(Server C)

set(CMAKE_C_STANDARD 11)

add_executable(Server src/packer.c src/network.c src/main.c)

if(WIN32)
    option(USE_DISTRIBUTED "Use distributed" OFF)
    target_link_libraries(Server wsock32 ws2_32)
endif()