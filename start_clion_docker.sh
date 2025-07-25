#!/bin/bash

set -e

echo "Setting up Docker container for CLion..."

# Check if Docker is running
if ! docker info > /dev/null 2>&1; then
  echo "Error: Docker is not running. Please start Docker and try again."
  exit 1
fi

# Build the Docker image if it doesn't exist
if ! docker image inspect sep-engine-builder > /dev/null 2>&1; then
  echo "Building Docker image..."
  docker build -t sep-engine-builder .
fi

# Check if container already exists and is running
if docker ps | grep -q sepengine; then
  echo "Docker container is already running."
else
  # Check if container exists but is stopped
  if docker ps -a | grep -q sepengine; then
    echo "Starting existing Docker container..."
    docker start sepengine
  else
    echo "Creating and starting new Docker container..."
    docker-compose up -d
  fi
fi

echo "\nDocker container is ready for CLion integration."
echo "\nTo run CLion with this container:"
echo "1. Open CLion"
echo "2. Select 'Docker Debug' configuration from the CMake profiles"
echo "3. Use the 'Debug in Docker' run configuration for debugging"

echo "\nYou can also build using the 'Build with build.sh' run configuration."

echo "\nContainer status:"
docker ps | grep sepengine

echo "\nSetup complete!"
