cmake_minimum_required(VERSION 3.31.6)
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

# Testing
add_executable(resonix_test tests/test_main.cpp)

target_link_libraries(resonix_test PRIVATE resonix)

set_target_properties(resonix_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/tests
)