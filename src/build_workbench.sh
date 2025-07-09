#!/bin/bash

echo "Building SEP workbench demos..."

# Set base directories
SEP_ROOT="/sep"
SEP_INCLUDE="$SEP_ROOT/src"
SEP_SRC="$SEP_ROOT/src"
SEP_BUILD="$SEP_ROOT/sep-build"

# Create build directory if it doesn't exist
mkdir -p build

# Clean up any old binary
rm -f build/sep_workbench build/sep_demo

# Compile the demo system
g++ -std=c++17 -o build/sep_demo \
    demo_main.cpp \
    window.cpp \
    renderer.cpp \
    ui_manager.cpp \
    demo_manager.cpp \
    sep_engine_wrapper.cpp \
    workbench_demo_adapter.cpp \
    demo_selection.cpp \
    demos/*.cpp \
    -I/usr/src \
    -I$SEP_INCLUDE \
    -I$SEP_SRC \
    -I$SEP_BUILD/src \
    -I. \
    -lglfw -lGL -ldl -lpthread -lsep_core -lsep_quantum -lsep_memory \
    -L$SEP_ROOT/lib \
    -DSEP_NO_CUDA -DSEP_WORKBENCH_BUILD

if [ $? -eq 0 ]; then
    echo "Demo build complete! Run with ./build/sep_demo"
    chmod +x build/sep_demo
else
    echo "Demo build failed!"
    exit 1
fi

# Also build the simple pattern demo for reference
g++ -std=c++17 -o build/sep_workbench \
    pattern_main.cpp window.cpp renderer.cpp ui_manager.cpp \
    -I/usr/src \
    -I$SEP_INCLUDE \
    -I$SEP_SRC \
    -I$SEP_BUILD/src \
    -I. \
    -lglfw -lGL -ldl -lpthread \
    -DSEP_NO_CUDA -DSEP_WORKBENCH_BUILD

if [ $? -eq 0 ]; then
    echo "Pattern demo build complete! Run with ./build/sep_workbench"
    chmod +x build/sep_workbench
else
    echo "Pattern demo build failed!"
fi