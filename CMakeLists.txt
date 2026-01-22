cmake_minimum_required(VERSION 4.1)
project(Resonix VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(resonix STATIC
        src/Resonix.cpp
        src/generator/Trigonometric.cpp
        src/generator/Primitives.cpp
        src/generator/Hann.cpp
        include/Math.hpp
        src/math/Trigonometry.cpp
        src/math/Utils.cpp
        src/math/NaN.cpp
)

target_include_directories(resonix PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(resonix PRIVATE
        -Wall -Wextra -Wpedantic
)