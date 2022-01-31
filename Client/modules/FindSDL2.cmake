#------------------------------------------------------------------------------
# Usage: find_package(SDL2 [REQUIRED] [COMPONENTS main])
#
# Sets variables:
#     SDL2_INCLUDE_DIRS
#     SDL2_LIBS
#     SDL2_DLLS
#------------------------------------------------------------------------------
set(SDL2_VERSION 2.0.20)
include(FindPackageHandleStandardArgs)

# Check if "main" was specified as a component
set(_SDL2_use_main FALSE)
foreach(_SDL2_component ${SDL2_FIND_COMPONENTS})
    if(_SDL2_component STREQUAL "main")
        set(_SDL2_use_main TRUE)
    else()
        message(WARNING "Unrecognized component \"${_SDL2_component}\"")
    endif()
endforeach()

if(WIN32)
    # Search for SDL2 in extern/SDL2-{SDL2_VERSION}
    set(SDL2_ROOT "${CMAKE_CURRENT_LIST_DIR}/../extern/SDL2-${SDL2_VERSION}")
    if(NOT ("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "Win64"))
        set(SDL2_INCLUDE_DIRS "${SDL2_ROOT}/x86_64-w64-mingw32/include/SDL2")
        set(SDL2_DLLS "${SDL2_ROOT}/x86_64-w64-mingw32/bin/SDL2.dll")
        set(SDL2_LIBS "${SDL2_ROOT}/x86_64-w64-mingw32/lib")
    else()
        set(SDL2_INCLUDE_DIRS "${SDL2_ROOT}/i686-w64-mingw32/include/SDL2")
        set(SDL2_LIBS "${SDL2_ROOT}/x86_64-w64-mingw32/lib")
        set(SDL2_DLLS "${SDL2_ROOT}/i686-w64-mingw32/bin/SDL2.dll")
    endif()

    mark_as_advanced(SDL2_ROOT)
    find_package_handle_standard_args(SDL2 DEFAULT_MSG SDL2_INCLUDE_DIRS SDL2_LIBS SDL2_DLLS)
else()
    find_path(SDL2_INCLUDE_DIRS SDL.h PATH_SUFFIXES SDL2)
    find_library(_SDL2_LIB SDL2)
    set(SDL2_LIBS ${SDL2})
    if(_SDL2_use_main)
        find_library(_SDL2main_LIB SDL2)
        list(APPEND SDL2_LIBS ${_SDL2main_LIB})
    endif()
    mark_as_advanced(SDL2_INCLUDE_DIRS _SDL2_LIB _SDL2main_LIB)
    find_package_handle_standard_args(SDL2 DEFAULT_MSG SDL2_INCLUDE_DIRS SDL2_LIBS)
endif()