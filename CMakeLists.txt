cmake_minimum_required(VERSION 3.10.0)

project(bwtzip-openmp)

# C++ Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

#set(CMAKE_VERBOSE_MAKEFILE ON)

set(BWT_OPENMP_SRC
        src/bwtzip_common.cc
        src/bwtzip_suffixtree.cc
        src/bwtzip_mtf.cc
        src/bwtzip_zle.cc
        src/bwtzip_arith.cc
        src/bwtzip_file.cc
        src/clock.cc)


add_executable(bwtzip
        src/bwtzip_common.cc
        src/bwtzip_suffixtree.cc
        src/bwtzip_mtf.cc
        src/bwtzip_zle.cc
        src/bwtzip_arith.cc
        src/bwtzip_file.cc
        src/clock.cc
        src/main_bwtzip.cc)

add_executable(bwtunzip
        src/bwtzip_common.cc
        src/bwtzip_mtf.cc
        src/bwtzip_zle.cc
        src/bwtzip_arith.cc
        src/bwtzip_file.cc
        src/clock.cc
        src/bwtunzip.cc
        src/main_bwtunzip.cc)


target_include_directories(bwtzip
        PUBLIC
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
        ${CMAKE_CURRENT_SOURCE_DIR}/test/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/test_unit1.cpp
        )

add_executable(tests ${TEST_SOURCES})

target_include_directories(tests
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/test
        ${CMAKE_CURRENT_SOURCE_DIR}/external
        )
target_link_libraries(tests
        PUBLIC
        Catch
        )

add_test(NAME tests COMMAND tests)
