cmake_minimum_required(VERSION 3.5)

project(LibTrace VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra -Wshadow -Wconversion")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Mode: debug")
    if (LINUX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=bounds -fsanitize=undefined -g -DDEBUG")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGLIBCXX_DEBUG=1 -g -DDEBUG")
    endif()
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    message("Mode: release with debug info")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
else()
    message("Mode: release")
endif()

include_directories(include)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(SOURCE_LIBRARY  lib/base.cpp
                    lib/vector.cpp
                    lib/color.cpp
                    lib/ray.cpp
                    lib/bbox.cpp
                    lib/interval.cpp 

                    lib/shapes.cpp 
                    lib/shapes/sphere.cpp 
                    lib/shapes/cube.cpp 
                    
                    lib/materials.cpp 
                    lib/materials/lamberitan.cpp 
                    lib/materials/metal.cpp 
                    lib/materials/glass.cpp 
                    lib/materials/light.cpp 
                    
                    lib/textures.cpp 
                    lib/textures/solid.cpp 
                    lib/textures/checker.cpp 
                    
                    lib/list.cpp 
                    lib/viewport.cpp 
                    lib/scene.cpp)

add_library(trace SHARED ${SOURCE_LIBRARY})