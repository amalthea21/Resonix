cmake_minimum_required(VERSION 3.31.6)
project(Resonix VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(resonix STATIC
        ../src/Resonix.cpp
        ../src/generator/Trigonometric.cpp
        ../src/generator/Primitives.cpp
        ../src/generator/Hann.cpp
        ../include/Math.hpp
        ../src/math/Trigonometry.cpp
        ../src/math/Utils.cpp
        ../src/math/NaN.cpp
)

target_include_directories(resonix PUBLIC
        ../include
)

target_compile_options(resonix PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wconversion
        -Wno-unused-parameter
        -O3
        -ffast-math
        -march=native
        -mtune=native
        -funroll-loops
        -flto
        -fno-signed-zeros
        -fno-trapping-math
        -fassociative-math
        -fno-math-errno
        -frename-registers
        -fdevirtualize-speculatively
        -fdevirtualize-at-ltrans
        -fipa-ra
        -fipa-pta
        -fipa-cp-clone
        -fpredictive-commoning
        -fsched-pressure
        -ftree-vectorize
        -fopt-info-vec
        -fvariable-expansion-in-unroller
        -ftree-loop-if-convert
        -ftree-loop-distribution
        -ftree-loop-im
        -ftree-loop-ivcanon
        -fivopts
        -ftree-parallelize-loops=4
        -funsafe-math-optimizations
        -ffinite-math-only
        -fno-rounding-math
        -fno-signaling-nans
        -fcx-limited-range
        -fexcess-precision=fast
)

target_link_options(resonix PRIVATE
        -flto
        -fuse-linker-plugin
)

set_property(TARGET resonix PROPERTY
        INTERPROCEDURAL_OPTIMIZATION TRUE
)

option(BUILD_PYTHON_BINDINGS "Build Python bindings" ON)

if(BUILD_PYTHON_BINDINGS)
    execute_process(
            COMMAND python3 -c "import pybind11; print(pybind11.get_cmake_dir())"
            OUTPUT_VARIABLE PYBIND11_CMAKE_DIR
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
    )

    if(PYBIND11_CMAKE_DIR)
        list(APPEND CMAKE_PREFIX_PATH ${PYBIND11_CMAKE_DIR})
    endif()

    find_package(pybind11 CONFIG)

    if(pybind11_FOUND)
        pybind11_add_module(resonix_python ../python/bindings.cpp)

        target_link_libraries(resonix_python PRIVATE resonix)

        set_target_properties(resonix_python PROPERTIES
                OUTPUT_NAME resonix
                POSITION_INDEPENDENT_CODE ON
        )

        target_compile_options(resonix_python PRIVATE
                -O3
                -ffast-math
                -march=native
                -flto
        )

        target_link_options(resonix_python PRIVATE
                -flto
                -fuse-linker-plugin
        )

        set_property(TARGET resonix_python PROPERTY
                INTERPROCEDURAL_OPTIMIZATION TRUE
        )

        message(STATUS "Python bindings will be built")
    else()
        message(WARNING "pybind11 not found. Python bindings will not be built.")
    endif()
endif()