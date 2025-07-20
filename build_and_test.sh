#!/bin/bash
set -e

# Define the Docker image name
IMAGE_NAME="sep-engine-builder"

# Build the Docker image
echo "Building Docker image: $IMAGE_NAME..."
docker build -t $IMAGE_NAME .

# Run the build and test process inside the container
echo "Running build and test inside Docker container..."
docker run --rm -v "$(pwd)":/project $IMAGE_NAME /bin/bash -c "
    set -e
    echo 'Cleaning up previous build artifacts...'
    rm -rf build
    mkdir build
    cd build
    
    echo 'Configuring project with CMake (CUDA Disabled)...'
    cmake .. -G Ninja \
        -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_C_COMPILER=clang-15 \
        -DCMAKE_CXX_COMPILER=clang++-15 \
        -DSEP_USE_CUDA=ON

    echo 'Building project with Ninja...'
    ninja

    echo 'Running tests with CTest...'
    ctest --output-on-failure
"

echo "Build and test process completed successfully."