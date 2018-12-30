#!/bin/bash
set -e

# Compute paths
WORKING_DIR=`pwd`
APPLICATION_ROOT=`git rev-parse --show-toplevel`
BUILD_DIR="cmake-build-profile"

# First clean the build folder (if any)
if [ -d "$APPLICATION_ROOT/$BUILD_DIR" ]; then
    rm -rf $APPLICATION_ROOT/$BUILD_DIR
fi

# Compile application
mkdir -p $APPLICATION_ROOT/$BUILD_DIR
cd $APPLICATION_ROOT/$BUILD_DIR
cmake -DPERF_PROFILING=ON -DCMAKE_BUILD_TYPE=Release ..
make bwtzip -j2
