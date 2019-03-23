# --------------------------------------------------------------------------- #
#                              CMake Preamble
# --------------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.10.0)
project(bwtzip-openmp)

set(CMAKE_VERBOSE_MAKEFILE ON)

# C++ Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Enable warnings
if (MSVC)
    add_compile_options(/W4)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-variable)
endif ()

# OpenMP dependency
find_package(OpenMP)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif ()

# Boost dependency
find_package(Boost 1.65.0 REQUIRED COMPONENTS filesystem)

# Enable unit testing support
include(CTest)
enable_testing()

include_directories(BEFORE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/external
        ${Boost_INCLUDE_DIRS})
# --------------------------------------------------------------------------- #
#                     bwtzip - Unmodified serial version
# --------------------------------------------------------------------------- #
set(COMMON_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/bwtzip_common.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/bwtzip_suffixtree.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/bwtzip_mtf.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/bwtzip_zle.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/bwtzip_arith.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/bwtzip_file.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/clock.cc)

# bwtzip executable
set(BWTZIP_SRC
        ${COMMON_SRC}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/bwtzip.cc)
add_executable(bwtzip
        ${BWTZIP_SRC}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main_bwtzip.cc)


# bwtunzip executable
set(BWTUNZIP_SRC
        ${COMMON_SRC}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/bwtunzip.cc)
add_executable(bwtunzip
        ${BWTUNZIP_SRC}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main_bwtunzip.cc)


# --------------------------------------------------------------------------- #
#                               Tests
# --------------------------------------------------------------------------- #
# Prepare "Catch" library for other executables
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/catch)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

set(TEST_SOURCES
        ${BWTZIP_SRC}
        ${CMAKE_CURRENT_SOURCE_DIR}/test/main.cpp
        #        ${CMAKE_CURRENT_SOURCE_DIR}/test/human_genome_compression_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/large_english_text_compression_test.cpp
        )

add_executable(tests ${TEST_SOURCES})

target_link_libraries(tests
        PUBLIC
        Catch
        ${Boost_LIBRARIES})


add_test(NAME tests COMMAND tests)
