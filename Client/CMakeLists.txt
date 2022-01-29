cmake_minimum_required(VERSION 3.13)
project(Client C)

set(CMAKE_C_STANDARD 11)
set(SOURCE_FILES src/main.c src/gui.c src/network.c src/gameplay.c src/packer.c)

#sys/socket.h on windows
option(USE_DISTRIBUTED "Use distributed" OFF)

# Find SDL2
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")
find_package(SDL2 REQUIRED COMPONENTS main)

# Define executable target
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2main_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${CMAKE_BINARY_DIR})
add_executable(Client ${SOURCE_FILES})
target_link_libraries(Client ${SDL2_LIBS} ${OPENGL_LIBRARIES})

# Copy SDL2 DLLs to output folder on Windows
if(WIN32)
    foreach(DLL ${SDL2_DLLS})
        add_custom_command(TARGET Client POST_BUILD COMMAND
                ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:CMakeDemo>)
    endforeach()
endif()

file(COPY img DESTINATION ${CMAKE_BINARY_DIR})