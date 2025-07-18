#!/bin/bash
set -e
rm -rf build
mkdir build
cd build
cmake -GNinja ..
cd ..
ninja
ctest -R pattern_metric_engine_test