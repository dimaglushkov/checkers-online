cmake_minimum_required(VERSION 3.13)
project(Client C)

set(CMAKE_C_STANDARD 11)
set(SOURCE_FILES src/main.c src/gui.c src/network.c src/gameplay.c src/packer.c)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Find SDL2
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")
find_package(SDL2 REQUIRED COMPONENTS main)

# Define executable target
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2main_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})
add_executable(Client ${SOURCE_FILES})
target_link_libraries(Client ${SDL2_LIBS})
message(WARNING ${SDL2_LIBS})

# Copy SDL2 DLLs to output folder on Windows
if(WIN32)
    option(USE_DISTRIBUTED "Use distributed" OFF)
    target_link_libraries(Client wsock32 ws2_32)

    foreach(DLL ${SDL2_DLLS})
        add_custom_command(TARGET Client POST_BUILD COMMAND
                ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:Client>)
    endforeach()
    file(COPY img DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

endif()
file(COPY img DESTINATION ${CMAKE_BINARY_DIR})

