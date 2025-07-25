# CLion Setup for SEP Engine

This document explains how to set up JetBrains CLion to work with the SEP Engine project.

## Prerequisites

1. Install CLion
2. Install Docker
3. Install the Docker plugin for CLion

## Setup Instructions

### 1. Open the Project in CLion

Open the SEP Engine project in CLion.

### 2. Configure Docker Integration

1. Go to **Settings/Preferences** > **Build, Execution, Deployment** > **Docker**
2. Click the **+** button to add a Docker connection
3. Select the appropriate connection type (Unix socket or TCP)
4. Test the connection and apply

### 3. Configure Docker Toolchain

1. Go to **Settings/Preferences** > **Build, Execution, Deployment** > **Toolchains**
2. Click the **+** button and select **Docker**
3. Choose the Docker connection you just configured
4. Set the Docker image to **sep-engine-builder:latest**
5. Move this toolchain to the top of the list

### 4. CMake Configuration

The project includes a pre-configured CMake profile called "Docker Debug" that sets all the necessary environment variables and compiler settings.

### 5. Building the Project

You can build the project using one of these methods:

1. Use the **Build with build.sh** run configuration to run the custom build script
2. Use CLion's built-in CMake build system with the Docker toolchain

### 6. Debugging

1. Make sure the Docker container is running (you can start it with `docker-compose up -d`)
2. Use the **Debug in Docker** run configuration to start a debugging session

### Troubleshooting

#### CUDA_HOME Not Set

If you encounter a "CUDA_HOME environment variable not set" error:

1. Make sure Docker is running
2. Verify that the Docker image has CUDA installed
3. Check that the environment variables are set correctly in the CMake configuration

#### Compile Commands Database

For code completion and navigation, the project uses the `compile_commands.json` file. This file is generated during the build process and automatically adjusted for local paths by the build script.

#### Manual Docker Container Setup

If you prefer to manually set up the Docker container for debugging:

```bash
# Build the Docker image
docker build -t sep-engine-builder .

# Run the container
docker run --gpus all -d --name sep_debug -v $(pwd):/sep sep-engine-builder

# Configure CLion to connect to this container for remote debugging
```
