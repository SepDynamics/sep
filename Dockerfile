FROM nvidia/cuda:12.9.0-devel-ubuntu22.04

# Set CUDA environment variables
ENV CUDA_HOME=/usr/local/cuda
ENV PATH=${CUDA_HOME}/bin:${PATH}
ENV LD_LIBRARY_PATH=${CUDA_HOME}/lib64:${LD_LIBRARY_PATH}

# Install build essentials, cmake, clang, and ninja
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
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
    gdb \
    sudo \
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
    echo "CUDA environment verification complete"

# Install Python dependencies
RUN pip3 install pandas numpy matplotlib

# Install CodeChecker for static analysis
RUN pip3 install codechecker

# Create a non-root user for CodeChecker
RUN useradd -m -s /bin/bash codecheck && \
   echo "codecheck ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

# Set the default working directory to the project root
WORKDIR /project

# Set up entrypoint script to handle user permissions
COPY --chmod=755 <<-"EOF" /usr/local/bin/docker-entrypoint.sh
#!/bin/bash
if [ ! -z "$USER_ID" ] && [ ! -z "$GROUP_ID" ]; then
    # Update user/group IDs
    usermod -u $USER_ID codecheck
    groupmod -g $GROUP_ID codecheck
    
    # Ensure CodeChecker directories exist with correct permissions
    mkdir -p /home/codecheck/.codechecker /home/codecheck/.cache
    chown -R codecheck:codecheck /home/codecheck /sep
fi

# Switch to codecheck user and execute command
if [ "$1" = "bash" ]; then
    exec sudo -u codecheck "$@"
else
    exec sudo -E -u codecheck "$@"
fi
EOF

ENTRYPOINT ["/usr/local/bin/docker-entrypoint.sh"]
CMD ["bash"]
