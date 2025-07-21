FROM nvidia/cuda:12.9.0-devel-ubuntu22.04

# Install build essentials, cmake, clang, and ninja
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    clang-15 \
    ninja-build \
    libfmt-dev \
    libtbb-dev \
    libbenchmark-dev \
    nlohmann-json3-dev \
    pkg-config \
    libhiredis-dev \
    libgtest-dev \
    libspdlog-dev \
    libglm-dev \
    libyaml-cpp-dev \
    libimgui-dev \
    libgl1-mesa-dev \
    libglfw3-dev \
    libcurl4-openssl-dev \
    curl \
    python3 \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

# Install Python dependencies
RUN pip3 install pandas numpy matplotlib

# Set the working directory
WORKDIR /project

# Copy the project files into the container
COPY . .

# The container is now just for the environment. The build script will handle the build process.