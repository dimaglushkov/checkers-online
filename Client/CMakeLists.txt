cmake_minimum_required(VERSION 3.13)
project(Client C)

set(CMAKE_C_STANDARD 11)
set(SOURCE_FILES src/main.c src/gui.c src/network.c src/gameplay.c src/packer.c)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")
find_package(SDL2 REQUIRED COMPONENTS main)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2main_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})
add_executable(Client ${SOURCE_FILES})

if(WIN32)
    option(USE_DISTRIBUTED "Use distributed" OFF)
    target_link_libraries(Client -lmingw32 ${SDL2_LIBS}/libSDL2main.a ${SDL2_LIBS}/libSDL2.dll.a -mwindows  wsock32 ws2_32)
    target_link_libraries(Client)

    foreach(DLL ${SDL2_DLLS})
        add_custom_command(TARGET Client POST_BUILD COMMAND
                ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:Client>)
    endforeach()
else()
    target_link_libraries(Client ${SDL2_LIBS})
endif()

file(COPY img DESTINATION ${CMAKE_BINARY_DIR})
file(COPY conf DESTINATION ${CMAKE_BINARY_DIR})

