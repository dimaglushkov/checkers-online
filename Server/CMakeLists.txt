cmake_minimum_required(VERSION 3.13)
project(Server C)

set(CMAKE_C_STANDARD 11)

add_executable(Server src/packer.c src/network.c src/main.c)