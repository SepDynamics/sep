#!/bin/bash
set -e
rm -rf build
cmake -Bbuild -S. -DCMAKE_BUILD_TYPE=Debug
cmake --build build
cd build
ctest --output-on-failure