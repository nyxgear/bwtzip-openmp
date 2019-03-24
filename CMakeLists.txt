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
    add_compile_options(-Werror -Wall -Wextra -Wpedantic)
endif ()

# OpenMP dependency
find_package(OpenMP)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif ()

# Enable unit testing support
include(CTest)
enable_testing()

include_directories(BEFORE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/external)

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
        ${CMAKE_CURRENT_SOURCE_DIR}/src/clock.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/wclock.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/log.cc)

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
#                      pbwtzip - OpenMP parallel version
# --------------------------------------------------------------------------- #
# pbwtzip executable
set(PBWTZIP_SRC
        ${COMMON_SRC}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/pbwtzip.cc)
add_executable(pbwtzip
        ${PBWTZIP_SRC}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main_pbwtzip.cc)

# pbwtzip2 executable
set(PBWTZIP2_SRC
        ${COMMON_SRC}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/pbwtzip2.cc)
add_executable(pbwtzip2
        ${PBWTZIP2_SRC}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main_pbwtzip2.cc)

# --------------------------------------------------------------------------- #
#                               Tests
# --------------------------------------------------------------------------- #
# Prepare "Catch" library for other executables
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/catch)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

set(TEST_SOURCES
        ${COMMON_SRC}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/pbwtzip.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/pbwtzip2.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/bwtunzip.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/test/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/test_pbwtzip.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/test_pbwtzip2.cpp)

add_executable(tests ${TEST_SOURCES})

target_link_libraries(tests
        PUBLIC
        Catch)


add_test(NAME tests COMMAND tests)
