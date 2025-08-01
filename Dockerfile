# Use fully qualified image reference for better compatibility with Docker
# alternatives like Podman which require explicit registry prefixes.
FROM docker.io/nvidia/cuda:12.9.0-devel-ubuntu22.04

# Set CUDA environment variables
ENV CUDA_HOME=/usr/local/cuda
ENV PATH=${CUDA_HOME}/bin:${PATH}
ENV LD_LIBRARY_PATH=${CUDA_HOME}/lib64:${LD_LIBRARY_PATH}

# Install build essentials, cmake, clang, and ninja
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    build-essential \
    cmake \
    git \
    clang-15 \
    clang-tidy-15 \
    clang-format-15 \
    iwyu \
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
    gdb \
    libpipewire-0.3-dev \
    libspa-0.2-dev \
    fftw3-dev \
    && rm -rf /var/lib/apt/lists/*

# Configure CUDA environment with explicit paths
ENV CUDA_HOME=/usr/local/cuda
ENV CUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda
ENV CUDA_BIN_PATH=/usr/local/cuda/bin
ENV CMAKE_CUDA_COMPILER=/usr/local/cuda/bin/nvcc
ENV PATH=${CUDA_HOME}/bin:${PATH}
ENV LD_LIBRARY_PATH=${CUDA_HOME}/lib64:${LD_LIBRARY_PATH}

# Verify CUDA installation and create necessary symlinks
RUN ls -la /usr/local/cuda/bin/nvcc && \
    ls -la /usr/local/cuda/include && \
    ls -la /usr/local/cuda/lib64 && \
    mkdir -p /usr/local/include/cuda && \
    mkdir -p /usr/local/lib/cuda && \
    ln -sf /usr/local/cuda/bin/nvcc /usr/bin/nvcc && \
    ln -sf /usr/local/cuda/include/* /usr/local/include/cuda/ && \
    ln -sf /usr/local/cuda/lib64/* /usr/local/lib/cuda/ && \
    ln -sf /usr/bin/clang-tidy-15 /usr/bin/clang-tidy && \
    ln -sf /usr/bin/clang-format-15 /usr/bin/clang-format && \
    echo "CUDA environment verification complete"

# Install Python packages for analysis
RUN pip3 install pandas numpy matplotlib

# Install PyTorch with CUDA support
RUN pip3 install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu121

# Install CodeChecker for static analysis
RUN pip3 install codechecker

# Set the default working directory to the project root
WORKDIR /project

# Default command
CMD ["bash"]
