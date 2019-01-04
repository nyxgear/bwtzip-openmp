# --------------------------------------------------------------------------- #
#                              CMake Preamble
# --------------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.10.0)
project(bwtzip-openmp)

# Enable unit testing support
include(CTest)
enable_testing()

# C++ Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)


# --------------------------------------------------------------------------- #
#                              BWTzip - OpenMP
# --------------------------------------------------------------------------- #
#set(CMAKE_VERBOSE_MAKEFILE ON)

# OpenMP dependency
find_package(OpenMP)

# bwtzip executable
set(BWTZIP_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/bwtzip_common.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/bwtzip_suffixtree.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/bwtzip_mtf.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/bwtzip_zle.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/bwtzip_arith.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/bwtzip_file.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/clock.cc)

add_executable(bwtzip
        ${BWTZIP_SRC}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main_bwtzip.cc)

target_include_directories(bwtzip
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/external
        )


# bwtunzip executable
set(BWTUNZIP_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/bwtzip_common.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/bwtzip_mtf.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/bwtzip_zle.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/bwtzip_arith.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/bwtzip_file.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/clock.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/bwtunzip.cc)

add_executable(bwtunzip
        ${BWTUNZIP_SRC}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main_bwtunzip.cc)

target_include_directories(bwtunzip
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/external
        )


# --------------------------------------------------------------------------- #
#                               Unit tests
# --------------------------------------------------------------------------- #
# Prepare "Catch" library for other executables
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/catch)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

set(TEST_SOURCES
        ${BWTZIP_SRC}
        ${CMAKE_CURRENT_SOURCE_DIR}/test/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/human_genome_compression_test.cpp
        )

add_executable(tests ${TEST_SOURCES})

target_include_directories(tests
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/test
        ${CMAKE_CURRENT_SOURCE_DIR}/external
        )


target_link_libraries(tests
        PUBLIC
        Catch
        )

add_test(NAME tests COMMAND tests)
